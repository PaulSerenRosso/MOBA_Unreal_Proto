// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TurretInfo.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API UTurretInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int MaxHealth;
	
	UPROPERTY(EditAnywhere)
	int Damage;
	
	UPROPERTY(EditAnywhere)
	float AttackCooldown;
};
