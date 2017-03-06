// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class MAGICMANNET_API ABatteryPickup : public APickup
{
	GENERATED_BODY()
	
public:
	ABatteryPickup();
	void WasCollected_Implementation() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const override;


	UFUNCTION(BlueprintAuthorityOnly, Category = "Pickup")
	virtual void PickedUpBy(APawn* Pawn) override;
	
	float GetPower();

protected:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = true))
	float  BatteryPower;
};
