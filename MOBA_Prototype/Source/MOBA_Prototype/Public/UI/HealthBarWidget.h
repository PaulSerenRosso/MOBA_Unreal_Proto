// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateProgressBar(float Value);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetTeamColor(FLinearColor Color);
};
