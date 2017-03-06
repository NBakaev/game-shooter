// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicManNet.h"
#include "MyGameState.h"
#include "Net/UnrealNetwork.h"
#include "grpc/UserService.h"


void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyGameState, PowerToWin);
	DOREPLIFETIME(AMyGameState, CurrentState);
	DOREPLIFETIME(AMyGameState, WiningPlayerName);
}

AMyGameState::AMyGameState()
{
	CurrentState = GlobalGameState::EUnknown;
	s = UserService();
}

AMagicManNetCharacter* AMyGameState::GetMyCharacter()
{
	return Cast<AMagicManNetCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

FString AMyGameState::GetMyCharacterId()
{
//	s.print();

	return s.getUsernameById("57da8d66ee69784410714e7c");
//
//	auto CurrentPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
//	if (CurrentPlayer && CurrentPlayer != NULL)
//	{
//		auto Player = Cast<AMagicManNetCharacter>(CurrentPlayer);
//		if (Player && Player != NULL && !Player->IsPendingKillPending())
//		{
//			return FString::FromInt(Player->GetUniqueID());
//		}
//	}
//
//	// when player is loose, actor is killed with GetUniqueID
//	return FString("null");
}

GlobalGameState AMyGameState::GetGameState() const
{
	return CurrentState;
}

void AMyGameState::SetNewState(GlobalGameState state)
{
	if (Role == ROLE_Authority)
	{
		CurrentState = state;
	}
}

void AMyGameState::OnRep_CurrentState()
{
}
