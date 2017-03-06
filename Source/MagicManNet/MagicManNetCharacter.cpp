// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MagicManNet.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MagicManNetCharacter.h"
#include "UnrealNetwork.h"
#include "Pickup.h"
#include "BatteryPickup.h"

//////////////////////////////////////////////////////////////////////////
// AMagicManNetCharacter

AMagicManNetCharacter::AMagicManNetCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ColelctionSphereRadius = 200.0f;

	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(ColelctionSphereRadius);

	InitialPower = 2000.0;
	CurrentPower = InitialPower;

	BaseSpeed = 10.0f;
	SpeedFactor = 0.75f;
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AMagicManNetCharacter::OnPlayerDead_Implementation()
{
	DetachFromControllerPendingDestroy();

	if (GetMesh())
	{
		static FName CollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetCollisionProfileName(CollisionProfileName);
	}

	SetActorEnableCollision(true);

	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AMagicManNetCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMagicManNetCharacter, ColelctionSphereRadius);
	DOREPLIFETIME(AMagicManNetCharacter, InitialPower);
	DOREPLIFETIME(AMagicManNetCharacter, CurrentPower);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMagicManNetCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMagicManNetCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMagicManNetCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMagicManNetCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMagicManNetCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMagicManNetCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMagicManNetCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMagicManNetCharacter::OnResetVR);

	PlayerInputComponent->BindAction("CollectPickups", IE_Pressed, this, &AMagicManNetCharacter::CollectPickups);
}

void AMagicManNetCharacter::CollectPickups()
{
	ServerCollectPickups();
}

void AMagicManNetCharacter::OnRep_CurrentPower()
{
	PowerChangeEvent();
}

bool AMagicManNetCharacter::ServerCollectPickups_Validate()
{
	return true;
}

void AMagicManNetCharacter::ServerCollectPickups_Implementation()
{
	if (Role == ROLE_Authority)
	{

		float TotalPower = 0.0f;


		TArray<AActor*> CollectedActors;
		CollectionSphere->GetOverlappingActors(CollectedActors);
		for (int i =0; i <CollectedActors.Num(); i++)
		{
			APickup* const TestPickup = Cast<APickup>(CollectedActors[i]);
			if (TestPickup != NULL && !TestPickup->IsPendingKillPending() && TestPickup->IsActive())
			{
				if (ABatteryPickup* const TestBattery = Cast<ABatteryPickup>(TestPickup))
				{
					TotalPower += TestBattery->GetPower();
				}

				TestPickup->PickedUpBy(this);
				TestPickup->setActive(false);
			}
		}
		if (!FMath::IsNearlyZero(TotalPower, 0.001f))
		{
			UpdatePower(TotalPower);
		}
	}
}


float AMagicManNetCharacter::GetInitialPower()
{
	return InitialPower;
}

float AMagicManNetCharacter::GetCurrentPower()
{
	return CurrentPower;
}

void AMagicManNetCharacter::UpdatePower(float DeltaPower)
{
	if (Role == ROLE_Authority)
	{
		CurrentPower += DeltaPower;
		GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + SpeedFactor * CurrentPower;

		OnRep_CurrentPower();
	}
}

void AMagicManNetCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMagicManNetCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMagicManNetCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMagicManNetCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMagicManNetCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMagicManNetCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMagicManNetCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
