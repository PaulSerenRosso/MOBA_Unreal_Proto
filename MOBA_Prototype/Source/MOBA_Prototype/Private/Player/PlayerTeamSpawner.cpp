// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerTeamSpawner.h"

#include "GameModeBattle.h"

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
	auto GM = GetWorld()->GetAuthGameMode();
	if (GM == nullptr) return;
	const auto GameModeBattle = Cast<AGameModeBattle>(GM);
	if (GameModeBattle == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("Add spawner to gamemode"));
	GameModeBattle->AddPlayerTeamSpawner(this, Team);
	
}

// Called every frame
void APlayerTeamSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

