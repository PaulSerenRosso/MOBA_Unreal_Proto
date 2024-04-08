// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "Components/WidgetComponent.h"
#include "HealthWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MOBA_PROTOTYPE_API UHealthWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateUI(ETeam Team);
};
