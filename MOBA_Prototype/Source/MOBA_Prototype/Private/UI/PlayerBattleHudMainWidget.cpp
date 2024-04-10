// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerBattleHudMainWidget.h"


void UPlayerBattleHudMainWidget::Init(UChampionData* InChampionData, APlayerBattleState* InPlayerBattleState, int InShootDamage)
{
	ChampionData = InChampionData;
	ShootDamage = InShootDamage;
	PlayerBattleState = InPlayerBattleState;
	OnUpdatePlayerStatsWidget(EPlayerStatType::Damage);
	OnUpdatePlayerStatsWidget(EPlayerStatType::MaxHealth);
	OnUpdatePlayerStatsWidget(EPlayerStatType::MoveSpeed);
}
