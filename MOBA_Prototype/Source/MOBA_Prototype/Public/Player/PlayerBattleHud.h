// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "PlayerCharacter.h"
#include "GameFramework/HUD.h"
#include "UI/PlayerBattleHudMainWidget.h"
#include "PlayerBattleHud.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API APlayerBattleHud : public AHUD
{
	GENERATED_BODY()
public:
	
	void SetPawn(APawn* Pawn);
	void SetPlayerState(APlayerBattleState* PlayerBattleState);
	UFUNCTION(BlueprintCallable)
	void InitHud(APlayerBattleState* InPlayerBattleState);

protected:
	APlayerBattleState* PlayerBattleState;
	APlayerCharacter* PlayerCharacter;
	UPROPERTY(EditAnywhere)
	UClass* WidgetClass;
	UPlayerBattleHudMainWidget* PlayerBattleHudMainWidget;
	UFUNCTION(BlueprintCallable)
	void UpdateWidgetStats(EPlayerStatType PlayerStatType);
};
