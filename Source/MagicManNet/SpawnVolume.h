// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class MAGICMANNET_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UBoxComponent* GetWhereToSpawn() { return  WhereToSpawn; };

	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPoint();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void setSpawningActive(bool bShouldSpawn);

protected:

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class APickup> WhatToSpawn;

	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayRangeLow;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayRangeHigh;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", Meta = (AllowPrivateAccess="true"))
	class UBoxComponent* WhereToSpawn;

	void SpawnPickup();



	float SpawnDelay;
	
};
