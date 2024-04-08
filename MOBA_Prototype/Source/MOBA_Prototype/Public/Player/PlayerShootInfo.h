// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerShootInfo.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API UPlayerShootInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float BulletSpeed;
	UPROPERTY(EditAnywhere)
	float BulletDamage;
	UPROPERTY(EditAnywhere)
	float FireRate;
	UPROPERTY(EditAnywhere)
	float Range;
	UPROPERTY(EditAnywhere)
	UClass* BulletClass;
};
