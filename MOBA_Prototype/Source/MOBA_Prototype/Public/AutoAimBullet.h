// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Hitable.h"
#include "AutoAimBullet.generated.h"

UCLASS()
class MOBA_PROTOTYPE_API AAutoAimBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutoAimBullet();

protected:
	IHitable* Target;
	FHitData* HitData;
	float Speed = 600.0f;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void Ray(float Distance);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(IHitable* Trget, FHitData* Hit, float Spd = 600.0f);

};
