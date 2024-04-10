// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/PlayerCharacter.h"
#include "GameModeBattle.h"
#include "GameStateBattle.h"
#include "Helpers.h"
#include "GameFramework/CharacterMovementComponent.h"
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
		IPlayerAttackable* InteractableComponent = Cast<IPlayerAttackable>(GetComponentsByInterface(UPlayerAttackable::StaticClass())[0]);
		PlayerAttackable = (InteractableComponent);
		InteractableComponent->SetUp(this);
		ShootDamage = PlayerAttackable->GetDamage();
	UpdateHealthClients(GetMaxHealth());
	SkeletalMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	HealthWidget = Cast<UHealthWidgetComponent>(GetComponentByClass(UHealthWidgetComponent::StaticClass()));
	
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	HealthWidget->UpdateUI(Team);
	GetCharacterMovement()->MaxWalkSpeed = ChampionData->MaxSpeed;
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(CurrentRotation);
}

void APlayerCharacter::DieServer()
{
	IsDead = true;
	GetWorldTimerManager().SetTimer(RespawnTimer,this,
		&APlayerCharacter::RespawnPlayerServer,
		Cast<AGameModeBattle>(GetWorld()->GetAuthGameMode())->RespawnTime, false);
	CancelAttackServer();
	if(OnDieServer.IsBound())
		OnDieServer.Broadcast();
	DieClients();
}

void APlayerCharacter::RespawnPlayerServer()
{
	IsDead = false;
	SetActorLocation(TeamSpawner->GetActorLocation());
	if(OnRespawnServer.IsBound())
		OnRespawnServer.Broadcast();
	UpdateHealthClients(GetMaxHealth());
	RespawnPlayerClients();
}

void APlayerCharacter::DieClients_Implementation()
{
	SkeletalMesh->SetVisibility(false);
	HealthWidget->SetVisibility(false);
}


void APlayerCharacter::RespawnPlayerClients_Implementation()
{
	SkeletalMesh->SetVisibility(true);
	HealthWidget->SetVisibility(true);
}

void APlayerCharacter::UpdateHealthClients_Implementation(int InHealth)
{
	CurrentHealth = InHealth;
	Execute_CallbackUpdateHealth(this);
}

void APlayerCharacter::SetTeamClients_Implementation(ETeam InTeam)
{
	Team = InTeam;
	HealthWidget->UpdateUI(Team);
	
}

void APlayerCharacter::SetPlayerBattleStateClients_Implementation(APlayerBattleState* InPlayerBattleState)
{
	if(InPlayerBattleState != nullptr)
	{
		PlayerBattleState = InPlayerBattleState;
		PlayerBattleState->OnUpdatePlayerStatClients.AddUFunction(this, "UpdateMaxSpeed");
		PlayerBattleState->OnUpdatePlayerStatClients.AddUFunction(this, "UpdateMaxHealth");
	}
}



void APlayerCharacter::UpdateMaxSpeed(const EPlayerStatType Type)
{
	if(Type == EPlayerStatType::MoveSpeed)
	{
		GetCharacterMovement()->MaxWalkSpeed = ChampionData->MaxSpeed*PlayerBattleState->GetStatValue(EPlayerStatType::MoveSpeed);
	}
}

void APlayerCharacter::UpdateMaxHealth(EPlayerStatType Type)
{
	if(Type == EPlayerStatType::MaxHealth)
	{
		Execute_CallbackUpdateHealth(this);
	}
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
	if(IsDead) return;
	UpdateHealthClients(CurrentHealth-HitData.Damage);
	if(CurrentHealth <= 0)
	{
		DieServer();
	}
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

	
	SetTeamClients(PlayerBattleState->Team);
	
	SetActorLocation(TeamSpawner->GetActorLocation());
	SetPlayerBattleStateClients(PlayerBattleState);
		
}

int APlayerCharacter::GetHealth()
{
	return CurrentHealth;
}

int APlayerCharacter::GetMaxHealth()
{
	return PlayerBattleState == nullptr ? ChampionData->MaxHealth :ChampionData->MaxHealth*PlayerBattleState->GetStatValue(EPlayerStatType::MaxHealth);
}

float APlayerCharacter::GetPercentageHealth()
{
	return static_cast<float>(GetHealth())/static_cast<float>(GetMaxHealth());
}

float APlayerCharacter::GetPlayerStatValue(EPlayerStatType PlayerStatType)
{
	return PlayerBattleState->GetStatValue(PlayerStatType);
}

bool APlayerCharacter::IsPlayerDead() const
{
	return IsDead;
}

void APlayerCharacter::CancelAttackServer_Implementation()
{
	
		PlayerAttackable->OnCancelAttackServer();
	
}

void APlayerCharacter::AttackServer_Implementation()
{
	PlayerAttackable->OnAttackServer();
}


void APlayerCharacter::RotateServer_Implementation(FVector Direction)
{
	CurrentRotation = Direction.Rotation();
}


void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, CurrentRotation);
	DOREPLIFETIME(APlayerCharacter, Team);
}

