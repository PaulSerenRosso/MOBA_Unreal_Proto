// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyAttackInfo.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API UEnemyAttackInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int MaxHealth;

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	float Range;

	UPROPERTY(EditAnywhere)
	float AttackCooldown;
	
};
