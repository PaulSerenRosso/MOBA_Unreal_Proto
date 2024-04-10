// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ChampionData.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API UChampionData : public UDataAsset
{
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHealth = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed = 600;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 100;
	GENERATED_BODY()
};
