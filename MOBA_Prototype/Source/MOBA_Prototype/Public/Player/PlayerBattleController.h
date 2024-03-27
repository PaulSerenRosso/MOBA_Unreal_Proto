// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerBattleController.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API APlayerBattleController : public APlayerController
{
	UClass* ChampionClass;
	GENERATED_BODY()
	virtual void BeginPlay() override;
	FTimerHandle handle;
	UFUNCTION(BlueprintCallable)
	void TryCreateChampionCharacter();
	UFUNCTION(Server, Reliable)
	void TestServer(UClass* currentChampionClass);
	
};
