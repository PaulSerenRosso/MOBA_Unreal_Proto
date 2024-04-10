// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unit/Unit.h"
#include "CollectibleBase.generated.h"

class USphereComponent;
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCollectibleCollected, AActor*, AActor*);

UCLASS()
class MOBA_PROTOTYPE_API ACollectibleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectibleBase();
	
	FOnCollectibleCollected OnCollected;

protected:
	USphereComponent* SphereCollision;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Unreliable)
	virtual void Collect(AUnit* Collector);

	UFUNCTION(NetMulticast, Unreliable)
	virtual void OnCollectedClient(AUnit* Collector);

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

};
