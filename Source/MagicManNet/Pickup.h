// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class MAGICMANNET_API APickup : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	APickup();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void setActive(bool newState);


	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();

	UFUNCTION(BlueprintNativeEvent, Category = "Pickup")
	void WasCollected();

	UFUNCTION(BlueprintAuthorityOnly, Category = "Pickup")
	virtual void PickedUpBy(APawn* Pawn);

protected:

	virtual void WasCollected_Implementation();


	UPROPERTY(ReplicatedUsing = OnRep_IsActive)
	bool bIsActive;
	
	UFUNCTION()
	virtual void OnRep_IsActive();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	APawn* PickupInvestigator;

	private:
	UFUNCTION(NetMulticast, Unreliable)
	void ClientOnPickedBy(APawn* Pawn);
};
