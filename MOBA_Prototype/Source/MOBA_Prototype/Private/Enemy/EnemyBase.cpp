// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBase::SetTeam(const ETeam NewTeam)
{
	Team = NewTeam;
	ChangedTeam();
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ETeam AEnemyBase::GetTeam()
{
	return Team;
}

void AEnemyBase::OnHit(FHitData HitData)
{
}

void AEnemyBase::ChangedTeam_Implementation()
{
}

void AEnemyBase::MoveCloseToTarget_Implementation(FVector Target, float AcceptanceRadius)
{
}

bool AEnemyBase::FindPointAroundTarget_Implementation(FVector Target, float Radius)
{
	// Will be implemented in the blueprint child classes
	return false;
}

