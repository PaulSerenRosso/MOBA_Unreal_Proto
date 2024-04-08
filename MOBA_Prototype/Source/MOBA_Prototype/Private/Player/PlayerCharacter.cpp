// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/PlayerCharacter.h"
#include "GameModeBattle.h"
#include "GameStateBattle.h"
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
	CurrentHealth = GetMaxHealth();
	OnHealthChangedClient();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(CurrentRotation);
}

void APlayerCharacter::DieServer()
{
	IsDie = true;
	CurrentHealth = GetMaxHealth();
	OnHealthChangedClient();
	GetWorldTimerManager().SetTimer(RespawnTimer,this,
		&APlayerCharacter::RespawnPlayerServer,
		Cast<AGameModeBattle>(GetWorld()->GetAuthGameMode())->RespawnTime, false);
	if(OnDieServer.IsBound())
		OnDieServer.Execute();
}

void APlayerCharacter::RespawnPlayerServer()
{
	IsDie = false;
	SetActorLocation(TeamSpawner->GetActorLocation());
	if(OnRespawnServer.IsBound())
		OnRespawnServer.Execute();
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
	
	return Team;
}

void APlayerCharacter::OnHit(FHitData HitData)
{
	if(IsDie) return;
	CurrentHealth -= HitData.Damage;
	UE_LOG(LogTemp, Warning, TEXT("hit by %s the target is %s"), *HitData.HitBy->GetName(), *GetName());
	if(CurrentHealth < 0)
	{
		DieServer();
	}
	OnHealthChangedClient();
}

ETeam APlayerCharacter::GetTeam()
{
	return Team;
}

void APlayerCharacter::OnSpawnedServer()
{
	PlayerBattleState = Cast<APlayerBattleState>(GetPlayerState());
	if (PlayerBattleState == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerBattleState is nullptr"));
		return;
	}
	PlayerBattleState->SetTeam();
	auto GameState = Cast<AGameStateBattle>(GetWorld()->GetGameState());
	if (GameState == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGameStateBattle is nullptr"));
		return;
	}
	if (!GameState->PlayerTeamSpawners.Contains(PlayerBattleState->Team))
	{
		if(PlayerBattleState->Team == ETeam::Neutral)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid TeamIndex: is neutral "));
		}
		UE_LOG(LogTemp, Warning, TEXT("Invalid TeamIndex: "));
		return;
	}
	TeamSpawner = GameState->PlayerTeamSpawners[PlayerBattleState->Team];
	if (TeamSpawner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TeamSpawner for Team is nullptr"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("TeamSpawner for Team : %s"), *TeamSpawner->GetName());
	Team = PlayerBattleState->Team;
	SetActorLocation(TeamSpawner->GetActorLocation());
		
}

int APlayerCharacter::GetHealth()
{
	return CurrentHealth;
}

int APlayerCharacter::GetMaxHealth()
{
	return ChampionData->MaxHealth;
}

float APlayerCharacter::GetPercentageHealth()
{
	return static_cast<float>(GetHealth())/static_cast<float>(GetMaxHealth());
}

void APlayerCharacter::OnHealthChangedClient_Implementation()
{
	Execute_CallbackUpdateHealth(this);
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
	DOREPLIFETIME(APlayerCharacter, CurrentHealth);
	DOREPLIFETIME(APlayerCharacter, Team);
}

