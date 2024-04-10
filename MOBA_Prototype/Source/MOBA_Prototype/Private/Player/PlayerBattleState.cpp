// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerBattleState.h"
#include "Net/UnrealNetwork.h"


void APlayerBattleState::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerBattleState::SetTeam()
{
	if(HasAuthority())
	{
		if(GetPlayerController()->IsLocalController())
		{
			UE_LOG(LogTemp, Warning, TEXT("EQUIPE 1"));
			Team =  ETeam::Team1;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("EQUIPE 2"));
			Team = ETeam::Team2;
		}
	}
}

float APlayerBattleState::GetStatValue(EPlayerStatType Type)
{
	return PlayerStats[Type];
}

void APlayerBattleState::IncreaseStatValueClients_Implementation(EPlayerStatType Type, float Amount)
{
	PlayerStats[Type] += Amount;
	OnUpdatePlayerStatClients.Broadcast(Type);
}

void APlayerBattleState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerBattleState, Team);
}
