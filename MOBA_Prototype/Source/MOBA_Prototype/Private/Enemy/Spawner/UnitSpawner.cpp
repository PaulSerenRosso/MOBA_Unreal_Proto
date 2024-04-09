// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Spawner/UnitSpawner.h"

#include "Enemy/EnemyBase.h"

// Sets default values
AUnitSpawner::AUnitSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnitSpawner::BeginPlay()
{
	Super::BeginPlay();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Start spawning units")));
	GetWorld()->GetTimerManager().SetTimer(WaveTimer, this, &AUnitSpawner::SpawnWave, WaveInterval, false);
}

void AUnitSpawner::SpawnWave()
{
	if (UnitToSpawn.Num() == 0) return;

	CurrentSpawnAmount = 0;
	
	GetWorld()->GetTimerManager().SetTimer(EachUnitTimer, this, &AUnitSpawner::SpawnRandomUnit, UnitInterval, false);
}

void AUnitSpawner::SpawnRandomUnit()
{
	const int SpawnIndex = FMath::RandRange(0, UnitToSpawn.Num() - 1);
	SpawnUnit(UnitToSpawn[SpawnIndex]);
	CurrentSpawnAmount++;

	if (CurrentSpawnAmount < SpawnAmount)
	{
		GetWorld()->GetTimerManager().SetTimer(EachUnitTimer, this, &AUnitSpawner::SpawnRandomUnit, UnitInterval, false);
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimer(WaveTimer, this, &AUnitSpawner::SpawnWave, WaveInterval, false);
}

void AUnitSpawner::SpawnUnit(const FUnitToSpawn& UnitDetails) const
{
	if (!HasAuthority()) return;
	if (!Enabled) return;
	

	if (UnitDetails.UnitClass == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Unit class is null")));
		return;
	}
	const auto UnitScale = FVector(0.6f, 0.6f, 0.6f);
	AEnemyBase* NewUnit = GetWorld()->SpawnActor<AEnemyBase>(UnitDetails.UnitClass, SpawnPoint->GetComponentLocation(), GetActorRotation());
	NewUnit->SetActorScale3D(UnitScale);
	NewUnit->SetTeam(Team);
}

// Called every frame
void AUnitSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

