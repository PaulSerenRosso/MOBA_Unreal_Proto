// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectible/CollectibleBase.h"
#include "GameFramework/Actor.h"
#include "CollectibleSpawner.generated.h"

UCLASS()
class MOBA_PROTOTYPE_API ACollectibleSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectibleSpawner();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RespawnTime = 5.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClass* CollectibleClass;

	FTimerHandle RespawnTimer;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void SpawnCollectible();
	
	UFUNCTION(NetMulticast, Unreliable)
	void SpawnCollectibleClients();

	void SpawnCollectibleAfterDelay();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnCollectibleCollected(AActor* Collectible, AActor* Collector);
};
