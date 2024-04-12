// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/PlayerState.h"
#include "PlayerBattleState.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdatePlayerStat,EPlayerStatType, float);
DECLARE_MULTICAST_DELEGATE(FOnGoldChanged);
UCLASS()
class MOBA_PROTOTYPE_API APlayerBattleState : public APlayerState
{
	GENERATED_BODY()
public:
	FOnUpdatePlayerStat OnUpdatePlayerStatClients;
	FOnGoldChanged OnGoldChangedClients;
	
	virtual void BeginPlay() override;
	UPROPERTY(Replicated)
	ETeam Team;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Gold;
	
	void SetTeam();
	UPROPERTY(EditAnywhere)
	TMap<EPlayerStatType, float> PlayerStats;
	UFUNCTION(NetMulticast, Reliable)
	void IncreaseStatValueClients(EPlayerStatType Type, float Amount);
	UFUNCTION(BlueprintCallable)
	float GetStatValue(EPlayerStatType Type);

	void ChangeGold(int Amount);
	void SetGold(int Amount);

	UFUNCTION(Server, Unreliable)
	virtual void SetGoldServer(int NewGold);
	
	UFUNCTION(NetMulticast, Unreliable)
	virtual void SetGoldClient(int NewGold);
};
