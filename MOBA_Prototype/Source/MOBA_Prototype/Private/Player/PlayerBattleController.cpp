// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerBattleController.h"
#include "MobaPrototypeGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"

void APlayerBattleController::BeginPlay()
{
	Super::BeginPlay();
	TryCreateChampionCharacter();
}

void APlayerBattleController::TryCreateChampionCharacter()
{
	auto LocalRole = GetLocalRole();
	if(LocalRole == ROLE_SimulatedProxy) return;
	if (LocalRole == ROLE_Authority && !IsLocalPlayerController()) return;
	
	auto gameInstance = Cast<UMobaPrototypeGameInstance>(GetGameInstance());
	if(gameInstance != nullptr)
	{
		ChampionClass = gameInstance->GetPlayerChampionClass();
		TestServer(ChampionClass);
	}
}

void APlayerBattleController::TestServer_Implementation(UClass* currentChampionClass)
{
	UnPossess();
	auto Character = GetWorld()->SpawnActor<APlayerCharacter>(currentChampionClass,FVector(UKismetMathLibrary::RandomFloat()*100+1000.000000,1810.000000,92.012604), FRotator::ZeroRotator );
	Possess(Character);
}




