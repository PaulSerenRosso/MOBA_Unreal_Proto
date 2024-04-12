// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerBattleState.h"
#include "Net/UnrealNetwork.h"


void APlayerBattleState::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerBattleState::SetTeam()
{
	if(!HasAuthority()) return;

	Team = GetPlayerController()->IsLocalController() ? ETeam::Team1 : ETeam::Team2;
}

float APlayerBattleState::GetStatValue(const EPlayerStatType Type)
{
	return PlayerStats[Type];
}

void APlayerBattleState::ChangeGold(const int Amount)
{
	SetGoldServer(Gold + Amount);
}

void APlayerBattleState::SetGold(const int Amount)
{
	SetGoldServer(Amount);
}

void APlayerBattleState::SetGoldServer_Implementation(int NewGold)
{
	SetGoldClient(NewGold);
}

void APlayerBattleState::SetGoldClient_Implementation(int NewGold)
{
	Gold = NewGold;
	OnGoldChangedClients.Broadcast();
}

void APlayerBattleState::IncreaseStatValueClients_Implementation(const EPlayerStatType Type, const float Amount)
{
	PlayerStats[Type] += Amount;
	OnUpdatePlayerStatClients.Broadcast(Type, PlayerStats[Type]);
}

void APlayerBattleState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerBattleState, Team);
}
