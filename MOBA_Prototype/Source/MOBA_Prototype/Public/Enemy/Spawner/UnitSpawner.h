// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "Enemy/EnemyBase.h"
#include "GameFramework/Actor.h"
#include "UnitSpawner.generated.h"

USTRUCT(Blueprintable)
struct FUnitToSpawn
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECustomUnitType UnitType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClass* UnitClass;
};

UCLASS()
class MOBA_PROTOTYPE_API AUnitSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitSpawner();

protected:
	FTimerHandle WaveTimer;
	FTimerHandle EachUnitTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Enabled = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WaveInterval = 10.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float UnitInterval = 0.5f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETeam Team;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SpawnAmount;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FUnitToSpawn> UnitToSpawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* SpawnPoint;

	float CurrentSpawnAmount = 0;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnWave();
	void SpawnRandomUnit();
	void SpawnUnit(const FUnitToSpawn& UnitDetails) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
