// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateBattle.h"

void AGameStateBattle::AddPlayerTeamSpawner(APlayerTeamSpawner* PlayerSpawner, ETeam Team)
{
	if(PlayerSpawner == nullptr) return;
	
	PlayerTeamSpawners.Add(Team, PlayerSpawner);
}

void AGameStateBattle::AddTurret(ATurret* Turret, const ETeam Team)
{
	if(Turret == nullptr) return;
	
	TeamTurrets.Add(Team, Turret);
}

UMaterial* AGameStateBattle::GetTeamMaterial(const ETeam Team)
{
	if (TeamMaterials.Contains(Team)) return TeamMaterials[Team];
	return nullptr;
}

ATurret* AGameStateBattle::GetTeamTurret(const ETeam Team)
{
	if (TeamTurrets.Contains(Team)) return TeamTurrets[Team];
	return nullptr;
}