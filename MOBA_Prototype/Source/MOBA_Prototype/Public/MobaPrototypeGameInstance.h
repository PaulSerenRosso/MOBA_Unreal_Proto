// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "Engine/GameInstance.h"
#include "MobaPrototypeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API UMobaPrototypeGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerChampionClass(int ChampionIndex);

	UFUNCTION(BlueprintCallable)
	UClass* GetPlayerChampionClass();

	void SetWinnerTeam(ETeam Team);

	UFUNCTION(BlueprintCallable)
	ETeam GetWinnerTeam();
private:
	ETeam WinnerTeam;
	
	UPROPERTY(EditAnywhere)
	TArray<UClass*> AllChampionClasses;
	UClass* CurrentPlayerChampionClass;
	
};
