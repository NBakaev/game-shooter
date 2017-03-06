// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MagicManNet.h"
#include "MagicManNetGameMode.h"
#include "MagicManNetCharacter.h"
#include "MyGameState.h"
#include "SpawnVolume.h"
#include "AssertionMacros.h"

void AMagicManNetGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	check(world);

	AMyGameState* MyGameState = Cast<AMyGameState>(GameState);
	check(MyGameState);


	DeadPlayerCount = 0;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(world, ASpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		if (ASpawnVolume* TestSpawnVol = Cast<ASpawnVolume>(Actor))
		{
			SpawnVolumeActors.AddUnique(TestSpawnVol);
		}
	}

	HandleNewState(GlobalGameState::EPlaying);

	for (FConstControllerIterator It = world->GetControllerIterator(); It; ++It)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(*It))
		{
			if (AMagicManNetCharacter* BatteryCharacter = Cast<AMagicManNetCharacter>(PlayerController->GetPawn()))
			{
				MyGameState->PowerToWin = BatteryCharacter->GetInitialPower() * PowerToWinMultiplie;
				break;
			}
		}
	}
}

void AMagicManNetGameMode::HandleNewState(GlobalGameState NewState)
{
	UWorld* world = GetWorld();
	check(world);

	AMyGameState* MyGameState = Cast<AMyGameState>(GameState);
	check(MyGameState);

	if (NewState != MyGameState->GetGameState())
	{
		MyGameState->SetNewState(NewState);

		switch (NewState)
		{
		case GlobalGameState::EPlaying:
			for (ASpawnVolume* SpawnVol : SpawnVolumeActors)
			{
				SpawnVol->setSpawningActive(true);
			}
			GetWorldTimerManager().SetTimer(PowerDrainTimer, this, &AMagicManNetGameMode::DrainOverOvertime, PowerDrainDelay, true);

		break;

		case GlobalGameState::EGameOver:
			for (ASpawnVolume* SpawnVol : SpawnVolumeActors)
			{
				SpawnVol->setSpawningActive(false);
			}
			GetWorldTimerManager().ClearTimer(PowerDrainTimer);
		break;
		
		case GlobalGameState::EWon:
			for (ASpawnVolume* SpawnVol : SpawnVolumeActors)
			{
				SpawnVol->setSpawningActive(false);
			}
		break;

		default:
		case GlobalGameState::EUnknown:
		break;

		}
	}
}


AMagicManNetGameMode::AMagicManNetGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDClass(TEXT("/Game/BP_MyGameHUD"));
	if (PlayerHUDClass.Class != NULL)
	{
		HUDClass = PlayerHUDClass.Class;
	}

	GameStateClass = AMyGameState::StaticClass();

	DecayRate = 0.02f;
	PowerDrainDelay = 0.025f;

	PowerToWinMultiplie = 1.25f;

	DeadPlayerCount = 0;
}

float AMagicManNetGameMode::getDelayRate()
{
	return DecayRate;
}

float AMagicManNetGameMode::GetPowerToWinMultiplier()
{
	return PowerToWinMultiplie;
}

void AMagicManNetGameMode::DrainOverOvertime()
{
	UWorld* world = GetWorld();
	check(world);

	AMyGameState* MyGameState = Cast<AMyGameState>(GameState);


	for (FConstControllerIterator It = world->GetControllerIterator(); It; ++It)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(*It))
		{
			if (AMagicManNetCharacter* BatteryCharacter = Cast<AMagicManNetCharacter>(PlayerController->GetPawn()))
			{
				if (BatteryCharacter->GetCurrentPower() > MyGameState->PowerToWin)
				{
					MyGameState->WiningPlayerName = BatteryCharacter->GetName();
					HandleNewState(GlobalGameState::EWon);
				}

				else if (BatteryCharacter->GetCurrentPower() > 0)
				{
					BatteryCharacter->UpdatePower(-PowerDrainDelay * DecayRate * (BatteryCharacter->GetInitialPower()));
				}
				else
				{

					BatteryCharacter->OnPlayerDead();

					BatteryCharacter->DetachFromControllerPendingDestroy();
					++DeadPlayerCount;

					if (DeadPlayerCount >= GetNumPlayers())
					{
						HandleNewState(GlobalGameState::EGameOver);
					}
				}
			}
		}
	}
}