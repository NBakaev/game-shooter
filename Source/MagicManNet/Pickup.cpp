// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicManNet.h"
#include "Pickup.h"
#include  "Net/UnrealNetwork.h"

void APickup::PickedUpBy(APawn* Pawn)
{
	if (Role == ROLE_Authority)
	{
		PickupInvestigator = Pawn;

		ClientOnPickedBy(Pawn);
	}
}

void APickup::WasCollected_Implementation()
{
	UE_LOG(LogClass, Log, TEXT("WasCollected_Implementation %s"), *GetName());
}

APickup::APickup()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	GetStaticMeshComponent()->bGenerateOverlapEvents = true;

	if (Role == ROLE_Authority)
	{
		bIsActive = true;
	}
}

void APickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APickup, bIsActive);
}

void APickup::setActive(bool newState)
{
	if (Role == ROLE_Authority)
	{
		bIsActive = newState;
	}
}

void APickup::OnRep_IsActive()
{
}

void APickup::ClientOnPickedBy_Implementation(APawn* Pawn)
{
	PickupInvestigator = Pawn;
	WasCollected();
}


bool APickup::IsActive()
{
	return bIsActive;
}
