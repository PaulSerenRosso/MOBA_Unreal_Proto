// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerPawnComponentHandlable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerPawnComponentHandlable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MOBA_PROTOTYPE_API IPlayerPawnComponentHandlable
{
	GENERATED_BODY()
	virtual FVector GetPlayerPosition() = 0;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
