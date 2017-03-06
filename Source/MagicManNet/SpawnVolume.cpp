// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicManNet.h"
#include "SpawnVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pickup.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (Role == ROLE_Authority)
	{
		WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
		RootComponent = WhereToSpawn;

		SpawnDelayRangeLow = 1.0f;
		SpawnDelayRangeHigh = 4.5f;
	}
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ASpawnVolume::GetRandomPoint()
{
	if (WhereToSpawn != NULL)
	{
		FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
		FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;

		return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
	}

	return FVector();
}

void ASpawnVolume::setSpawningActive(bool bShouldSpawn)
{
	if (Role == ROLE_Authority)
	{
		if (bShouldSpawn)
		{
			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
		}else
		{
			GetWorldTimerManager().ClearTimer(SpawnTimer);
		}
	}
}

void ASpawnVolume::SpawnPickup()
{
	if (Role == ROLE_Authority && WhatToSpawn != NULL)
	{
		if (UWorld* const world = GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			auto SpawnLocation = GetRandomPoint();

			FRotator SpawnRotation;
			SpawnRotation.Yaw = FMath::FRand() * 360.f;
			SpawnRotation.Pitch = FMath::FRand() * 360.f;
			SpawnRotation.Roll = FMath::FRand() * 360.f;

			APickup* const  SpawnedPickup = world->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

			setSpawningActive(true);
		}
	}
}
