// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChampionData.h"
#include "Blueprint/UserWidget.h"
#include "Player/PlayerBattleState.h"
#include "PlayerBattleHudMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API UPlayerBattleHudMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(UChampionData* InChampionData, APlayerBattleState* InPlayerBattleState, int InShootDamage);
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdatePlayerStatsWidget(EPlayerStatType PlayerStatType);
protected:
	UPROPERTY(BlueprintReadWrite)
	int ShootDamage;
	UPROPERTY(BlueprintReadWrite)
	UChampionData* ChampionData;
	UPROPERTY(BlueprintReadWrite)
	APlayerBattleState* PlayerBattleState; 
};
