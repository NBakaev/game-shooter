// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "MagicManNetCharacter.h"
#include "grpc/UserService.h"
#include "MyGameState.generated.h"


UENUM(BlueprintType)
enum GlobalGameState
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknown
};

/**
 * 
 */
UCLASS()
class MAGICMANNET_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	
public:


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const override;

	AMyGameState();

	UFUNCTION(BlueprintPure, Category = "Current Player")
	AMagicManNetCharacter* GetMyCharacter();

	UFUNCTION(BlueprintPure, Category = "Current Player")
	FString GetMyCharacterId();

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Power")
	FString WiningPlayerName;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Power")
	float PowerToWin;
	
	UFUNCTION(BlueprintPure, Category = "Power")
	GlobalGameState GetGameState() const;

	void SetNewState(GlobalGameState state);

	UFUNCTION()
	void OnRep_CurrentState();
private:

	UPROPERTY(ReplicatedUsing = OnRep_CurrentState)
	TEnumAsByte<enum GlobalGameState> CurrentState;
	UserService	s;
	
};
