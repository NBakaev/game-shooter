// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "SpawnVolume.h"
#include "MyGameState.h"
#include "MagicManNetGameMode.generated.h"

UCLASS(minimalapi)
class AMagicManNetGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = true))
	float DecayRate;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = true))
	float PowerToWinMultiplie;

	int32 DeadPlayerCount;

public:
	AMagicManNetGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Power")
	float PowerDrainDelay;

	FTimerHandle PowerDrainTimer;

	UFUNCTION(BlueprintPure, Category = "Power")
	float getDelayRate();

	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWinMultiplier();

private:

	void DrainOverOvertime();

	TArray<class ASpawnVolume*> SpawnVolumeActors;


	void HandleNewState(GlobalGameState state);
};



