// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "MagicManNetCharacter.generated.h"

UCLASS(config=Game)
class AMagicManNetCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Battery, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;

public:
	AMagicManNetCharacter();

	UFUNCTION(NetMulticast, Reliable)
	void OnPlayerDead();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintPure, Category = "Power")
	float GetInitialPower();
	
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetCurrentPower();

	/**
	 * @param amount to change
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Power")
	void UpdatePower(float DeltaPower);

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void CollectPickups();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCollectPickups();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class USphereComponent* GetCollectionSphere() const { return CollectionSphere; }

protected:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Power, Meta = (BlueprintProtected = "true"))
	float InitialPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintPotected = "true"))
	float BaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintPotected = "true" ))
	float SpeedFactor;

	UFUNCTION(BlueprintImplementableEvent, Category = "Power")
	void PowerChangeEvent();

private:

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category=Battery, Meta= (AllowPrivateAccess = "true"))
	float ColelctionSphereRadius;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentPower, VisibleAnywhere, Category = "Power")
	float CurrentPower;

	UFUNCTION()
	void OnRep_CurrentPower();
};
