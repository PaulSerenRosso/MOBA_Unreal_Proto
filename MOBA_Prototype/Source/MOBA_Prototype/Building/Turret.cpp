// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "GameModeBattle.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	auto GM = GetWorld()->GetAuthGameMode();
	if (GM == nullptr) return;

	auto GameModeBattle = Cast<AGameModeBattle>(GM);
	if (GameModeBattle == nullptr) return;

	GameModeBattle->AddTurret(this, OwnTeam);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurret::AddHittableTarget(TSubclassOf<AActor> Target, const ETeam Team)
{
	IHitable* Hittable = Cast<IHitable>(Target);

	if (Hittable == nullptr) return;
	if (Team == ETeam::Neutral || Team == OwnTeam) return;
	if (HittableTargets.Contains(Hittable)) return;
	
	HittableTargets.Add(Hittable);
}

void ATurret::RemoveHittableTarget(TSubclassOf<AActor> Target)
{
	IHitable* Hittable = Cast<IHitable>(Target);
	if (Hittable == nullptr) return;

	HittableTargets.Remove( Hittable );
}

