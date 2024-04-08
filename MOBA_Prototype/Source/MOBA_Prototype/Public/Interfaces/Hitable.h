// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "UObject/Interface.h"
#include "Hitable.generated.h"


USTRUCT(Blueprintable)
struct FHitData
{
	GENERATED_BODY()

	FHitData();
	
	FHitData(const float Damage, AActor* HitBy);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* HitBy;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UHitable : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class MOBA_PROTOTYPE_API IHitable
{
	GENERATED_BODY()
public:
	virtual void OnHit(FHitData HitData) = 0;
	virtual ETeam GetTeam() = 0;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};