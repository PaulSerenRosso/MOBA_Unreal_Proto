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

	UPROPERTY()
	float Damage;

	UPROPERTY()
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

	virtual void OnHit(FHitData HitData);
	virtual ETeam GetTeam();

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};