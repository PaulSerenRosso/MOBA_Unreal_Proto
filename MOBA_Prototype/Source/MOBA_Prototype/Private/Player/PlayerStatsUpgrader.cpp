// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerStatsUpgrader.h"

#include "Helpers.h"
#include "Player/PlayerCharacter.h"

// Sets default values
APlayerStatsUpgrader::APlayerStatsUpgrader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerStatsUpgrader::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if(!HasAuthority()) return;
	auto PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	UHelpers::LogScreenMessage(TEXT("Overlap"));
	if(PlayerCharacter == nullptr) return;
	if(PlayerCharacter->GetTeam() != Team) return;

	if (PlayerCharacter->PlayerBattleState->Gold < PlayerStatsUpgraderInfo->GoldCost) return;

	UHelpers::LogScreenMessage(TEXT("Increase"));
	PlayerCharacter->PlayerBattleState->Gold -= PlayerStatsUpgraderInfo->GoldCost;
	PlayerCharacter->PlayerBattleState->IncreaseStatValueClients(PlayerStatsUpgraderInfo->PlayerStatType, PlayerStatsUpgraderInfo->UpgradeValue);
	
}



