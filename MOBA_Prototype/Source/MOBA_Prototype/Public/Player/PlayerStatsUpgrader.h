// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "DesignData/PlayerStatsUpgraderInfo.h"
#include "GameFramework/Actor.h"
#include "PlayerStatsUpgrader.generated.h"

UCLASS()
class MOBA_PROTOTYPE_API APlayerStatsUpgrader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerStatsUpgrader();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPlayerStatsUpgraderInfo* PlayerStatsUpgraderInfo;
	UPROPERTY(EditAnywhere)
	ETeam Team;

public:	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
