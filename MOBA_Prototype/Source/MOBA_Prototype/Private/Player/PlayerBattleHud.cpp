// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerBattleHud.h"

#include "Helpers.h"
#include "Player/PlayerCharacter.h"
#include "UI/PlayerBattleHudMainWidget.h"

void APlayerBattleHud::SetPawn(APawn* Pawn)
{
	PlayerCharacter = Cast<APlayerCharacter>(Pawn);
	if (PlayerBattleState == nullptr) return;
	InitHud(PlayerBattleState);
}

void APlayerBattleHud::SetPlayerState(APlayerBattleState* InPlayerBattleState)
{
	PlayerBattleState = InPlayerBattleState;
	if (PlayerCharacter == nullptr) return;
	InitHud(PlayerBattleState);
}

void APlayerBattleHud::InitHud(APlayerBattleState* InPlayerBattleState)
{
	if (PlayerBattleHudMainWidget != nullptr) return;
	if(InPlayerBattleState== nullptr) return;
	
	auto ChampionData = PlayerCharacter->ChampionData;
	PlayerBattleHudMainWidget = CreateWidget<UPlayerBattleHudMainWidget>(GetOwningPlayerController(),WidgetClass );
	PlayerBattleHudMainWidget->AddToViewport();
	PlayerBattleHudMainWidget->Init(ChampionData, InPlayerBattleState, PlayerCharacter->ShootDamage);
	InPlayerBattleState->OnUpdatePlayerStatClients.AddUFunction(this, "UpdateWidgetStats");
}

void APlayerBattleHud::UpdateWidgetStats(EPlayerStatType PlayerStatType, float Amount)
{
	PlayerBattleHudMainWidget->OnUpdatePlayerStatsWidget(PlayerStatType);
}


