// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerBattleState.h"
#include "Net/UnrealNetwork.h"




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

void APlayerBattleState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerBattleState, Team);
}
