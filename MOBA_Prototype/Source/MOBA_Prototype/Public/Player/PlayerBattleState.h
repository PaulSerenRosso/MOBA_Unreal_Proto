// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/PlayerState.h"
#include "PlayerBattleState.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdatePlayerStat,EPlayerStatType);
UCLASS()
class MOBA_PROTOTYPE_API APlayerBattleState : public APlayerState
{
	GENERATED_BODY()
public:
	FOnUpdatePlayerStat OnUpdatePlayerStatClients;
	virtual void BeginPlay() override;
	UPROPERTY(Replicated)
	ETeam Team;
	void SetTeam();
	UPROPERTY(EditAnywhere)
	TMap<EPlayerStatType, float> PlayerStats;
	UFUNCTION(NetMulticast, Reliable)
	void IncreaseStatValueClients(EPlayerStatType Type, float Amount);
	UFUNCTION(BlueprintCallable)
	float GetStatValue(EPlayerStatType Type);
};
