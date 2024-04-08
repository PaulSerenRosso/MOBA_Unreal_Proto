// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/Hitable.h"

FHitData::FHitData() : Damage(0), HitBy(nullptr)
{
}

FHitData::FHitData(const float Damage, AActor* HitBy) : Damage(Damage), HitBy(HitBy)
{
}

// Add default functionality here for any IHitable functions that are not pure virtual.
void IHitable::OnHit(FHitData HitData)
{
}

ETeam IHitable::GetTeam()
{
	return ETeam::Neutral;
}
