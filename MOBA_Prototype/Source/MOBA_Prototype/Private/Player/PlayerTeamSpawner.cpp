// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerTeamSpawner.h"

#include "GameModeBattle.h"
#include "GameStateBattle.h"

// Sets default values
APlayerTeamSpawner::APlayerTeamSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerTeamSpawner::BeginPlay()
{
	Super::BeginPlay();
	auto GameState = GetWorld()->GetGameState();
	if (GameState == nullptr) return;
	const auto GameStateBattle = Cast<AGameStateBattle>(GameState);
	if (GameStateBattle == nullptr) return;
	GameStateBattle->AddPlayerTeamSpawner(this, Team);
	
}

// Called every frame
void APlayerTeamSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

