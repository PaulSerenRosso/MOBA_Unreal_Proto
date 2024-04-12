// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible/HealthCollectible.h"

#include "Player/PlayerCharacter.h"
#include "Unit/Unit.h"

void AHealthCollectible::Collect(AUnit* Collector)
{
	if (Collector == nullptr) return;
	const auto Player = Cast<APlayerCharacter>(Collector);

	if (Player == nullptr && OnlyHealPlayer) return;

	if (Player->GetPercentageHealth() == 1.f) return;
	
	OnCollectedClient(Collector);
}

void AHealthCollectible::OnCollectedClient(AUnit* Collector)
{
	if (Collector == nullptr) return;
	Collector->Heal(HealAmount, IsPercentage);
	
	OnCollected.Broadcast(this, Collector);
	Destroy();
}
