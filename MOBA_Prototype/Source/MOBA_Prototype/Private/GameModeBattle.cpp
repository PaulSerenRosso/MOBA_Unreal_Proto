// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBattle.h"


void AGameModeBattle::AddTurret(ATurret* Turret, const ETeam Team)
{
	if(Turret == nullptr) return;
	
	TeamTurrets.Add(Team, Turret);
}

UMaterial* AGameModeBattle::GetTeamMaterial(const ETeam Team)
{
	if (TeamMaterials.Contains(Team)) return TeamMaterials[Team];
	return nullptr;
}

ATurret* AGameModeBattle::GetTeamTurret(const ETeam Team)
{
	if (TeamTurrets.Contains(Team)) return TeamTurrets[Team];
	return nullptr;
}
