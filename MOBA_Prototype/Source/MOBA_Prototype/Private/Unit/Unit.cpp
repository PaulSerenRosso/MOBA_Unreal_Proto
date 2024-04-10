// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Unit.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AUnit::AUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	UpdateTeamColor();
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUnit, CurrentHealth);
	DOREPLIFETIME(AUnit, IsDead);
}

ETeam AUnit::GetTeam()
{
	return ETeam::Neutral;
}

bool AUnit::IsUnitDead() const
{
	return IsDead;
}

void AUnit::Heal(int HealAmount)
{
	CurrentHealth += HealAmount;
	int Max = GetMaxHealth();
	
	if (CurrentHealth > Max)
	{
		CurrentHealth = Max;
	}
	UpdateHealthClients(CurrentHealth);

	
}

int AUnit::GetHealth()
{
	return CurrentHealth;
}

int AUnit::GetMaxHealth()
{
	return 0;
}

float AUnit::GetPercentageHealth()
{
	return 0;
}

void AUnit::OnHit(FHitData HitData)
{
}

void AUnit::UpdateHealthClients_Implementation(int InHealth)
{
	CurrentHealth = InHealth;
	Execute_CallbackUpdateHealth(this);
}

void AUnit::ChangedTeam_Implementation()
{
	UpdateTeamColor();
}

