// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerBattleState.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	for (auto Component : GetComponentsByInterface(UPlayerAttackable::StaticClass()))
	{
		IPlayerAttackable* InteractableComponent = Cast<IPlayerAttackable>(Component);
		PlayerAttackables.Add(InteractableComponent);
		InteractableComponent->SetUp(this);
	}
}



// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(CurrentRotation);
}

void APlayerCharacter::Move(FVector2D Direction)
{
	AddMovementInput(FVector::RightVector, Direction.X);
	AddMovementInput(FVector::ForwardVector, Direction.Y);
}


FVector APlayerCharacter::GetPlayerPosition()
{
	return GetActorLocation();
}

FRotator APlayerCharacter::GetPlayerRotation()
{
	return GetActorRotation();
}

ETeam APlayerCharacter::GetPlayerTeam()
{
	
	return PlayerBattleState->Team;
}

void APlayerCharacter::OnHit(FHitData HitData)
{
	
}

ETeam APlayerCharacter::GetTeam()
{
	return PlayerBattleState->Team;
}

void APlayerCharacter::OnSpawnedServer()
{
		PlayerBattleState = Cast<APlayerBattleState>(GetPlayerState());
}


void APlayerCharacter::CancelAttackServer_Implementation()
{
	for (auto Attackable : PlayerAttackables)
	{
		Attackable->OnCancelAttackServer();
	}
}

void APlayerCharacter::AttackServer_Implementation()
{
	for (auto Attackable : PlayerAttackables)
	{
		Attackable->OnAttackServer();
	}
}


void APlayerCharacter::RotateServer_Implementation(FVector Direction)
{
	CurrentRotation = Direction.Rotation();
}


void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, CurrentRotation);
}

