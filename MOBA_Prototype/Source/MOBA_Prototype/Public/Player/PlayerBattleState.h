// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/PlayerState.h"
#include "PlayerBattleState.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API APlayerBattleState : public APlayerState
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated)
	ETeam Team;
	void SetTeam();
};
