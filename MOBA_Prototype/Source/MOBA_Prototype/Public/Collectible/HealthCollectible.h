// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectible/CollectibleBase.h"
#include "HealthCollectible.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API AHealthCollectible : public ACollectibleBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HealAmount = 100.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool OnlyHealPlayer = true;

	virtual void Collect(AUnit* Collector) override;
	virtual void OnCollectedClient(AUnit* Collector) override;
};
