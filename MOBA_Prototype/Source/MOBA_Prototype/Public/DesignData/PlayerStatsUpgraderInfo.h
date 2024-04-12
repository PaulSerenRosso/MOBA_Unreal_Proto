// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "Engine/DataAsset.h"
#include "PlayerStatsUpgraderInfo.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API UPlayerStatsUpgraderInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EPlayerStatType PlayerStatType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UpgradeValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GoldCost;
};
