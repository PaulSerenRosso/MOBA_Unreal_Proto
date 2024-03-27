// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaPrototypeGameInstance.h"


void UMobaPrototypeGameInstance::SetPlayerChampionClass(int ChampionIndex)
{
	CurrentPlayerChampionClass = AllChampionClasses[ChampionIndex];
}

UClass* UMobaPrototypeGameInstance::GetPlayerChampionClass()
{
	return CurrentPlayerChampionClass;
}
