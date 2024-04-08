// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/GameModeBase.h"
#include "Building/Turret.h"
#include "Player/PlayerTeamSpawner.h"
#include "GameModeBattle.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API AGameModeBattle : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TeamMaterials")
	TMap<ETeam, UMaterial*> TeamMaterials;
	TMap<ETeam, ATurret*> TeamTurrets;
	TMap<ETeam, APlayerTeamSpawner*> PlayerTeamSpawners;
	void AddPlayerTeamSpawner(APlayerTeamSpawner* PlayerSpawner, ETeam Team);
	void AddTurret(ATurret* Turret, ETeam Team);

	UFUNCTION(BlueprintCallable)
	UMaterial* GetTeamMaterial(ETeam Team);
	
	UFUNCTION(BlueprintCallable)
	ATurret* GetTeamTurret(ETeam Team);
};
