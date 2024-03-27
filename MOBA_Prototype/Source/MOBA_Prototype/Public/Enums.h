// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Enums.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETeam : uint8
{
	Neutral, Team1, Team2
};

UENUM(BlueprintType)
enum class EUnitType : uint8
{
	MeleeBase, RangedBase
};

UCLASS()
class MOBA_PROTOTYPE_API UEnums : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

};
