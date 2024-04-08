// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Helpers.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API UHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void LogScreenMessage(const FString& Text, float Duration= 2 );
	static ETeam GetEnemyTeam(ETeam Team);
};
