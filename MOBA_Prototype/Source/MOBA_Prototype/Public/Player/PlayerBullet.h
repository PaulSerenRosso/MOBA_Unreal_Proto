// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PlayerBullet.generated.h"

UCLASS(Blueprintable)
class MOBA_PROTOTYPE_API APlayerBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerBullet();
	virtual void SetUp(float InDamage, float InSpeed, float InRange, ETeam InTeam, AActor* InHitBy, float DotDirection);
protected:
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
	int MeshRenderer3;
	ETeam Team;
	UPROPERTY(Replicated)
	float Speed;
	float Damage;
	AActor* HitBy;
	float LifeTime;
	bool IsSetup;
	FTimerHandle DestructionTimerHandle;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void DestroyBullet();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;


};


