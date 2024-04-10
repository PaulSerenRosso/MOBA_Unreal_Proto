// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerPawnComponentHandlable.h"
#include "UObject/Interface.h"
#include "PlayerAttackable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerAttackable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MOBA_PROTOTYPE_API IPlayerAttackable
{
	GENERATED_BODY()
public:
	virtual void SetUp(IPlayerPawnComponentHandlable* PlayerHandler) = 0;
	
	virtual void OnAttackServer() = 0;
	virtual void OnCancelAttackServer() = 0;
	virtual int GetDamage() =0;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
