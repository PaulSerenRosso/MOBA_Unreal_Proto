// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/PlayerCharacter.h"
#include "GameModeBattle.h"
#include "GameStateBattle.h"
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
	//TODO remove, debug til we have UI
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Golds: " + FString::FromInt(PlayerBattleState->Gold)));
	
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

void APlayerCharacter::SetTeamClients_Implementation(ETeam InTeam)
{
	Team = InTeam;
	HealthWidget->UpdateUI(Team);
}


void APlayerCharacter::SetPlayerBattleStateOnServer()
{
	OnBattlePlayerState();
	
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
	
	SetActorLocation(TeamSpawner->GetActorLocation());
	
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnBattlePlayerState();
}

void APlayerCharacter::OnBattlePlayerState()
{
	PlayerBattleState = Cast<APlayerBattleState>(GetPlayerState());
	if (PlayerBattleState == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerBattleState is nullptr"));
		return;
	}
	
	PlayerBattleState->SetTeam();
	if (HasAuthority())
	{
		Team = PlayerBattleState->Team;
		SetTeamClients(PlayerBattleState->Team);
	}
	
	PlayerBattleState->OnUpdatePlayerStatClients.AddUFunction(this, "UpdateMaxSpeed");
	PlayerBattleState->OnUpdatePlayerStatClients.AddUFunction(this, "UpdateMaxHealth");
	PlayerBattleState->SetGold(0);
}

void APlayerCharacter::UpdateMaxSpeed(const EPlayerStatType Type, float Amount)
{
	if(Type == EPlayerStatType::MoveSpeed)
	{
		GetCharacterMovement()->MaxWalkSpeed = ChampionData->MaxSpeed*PlayerBattleState->GetStatValue(EPlayerStatType::MoveSpeed);
	}
}

void APlayerCharacter::UpdateMaxHealth(EPlayerStatType Type, float Amount)
{
	if(Type == EPlayerStatType::MaxHealth)
	{
		UpdateMaxHealthClients(Amount);
		Execute_CallbackUpdateHealth(this);
	}
}

void APlayerCharacter::UpdateMaxHealthClients_Implementation(float NewAmount)
{
	LocalMaxHealthAmount = NewAmount;
	Execute_CallbackUpdateHealth(this);
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
	SetPlayerBattleStateOnServer();
}

int APlayerCharacter::GetHealth()
{
	return CurrentHealth;
}

int APlayerCharacter::GetMaxHealth()
{
	return ChampionData->MaxHealth  * (PlayerBattleState == nullptr ? LocalMaxHealthAmount : PlayerBattleState->GetStatValue(EPlayerStatType::MaxHealth));
}

float APlayerCharacter::GetPercentageHealth()
{
	return static_cast<float>(GetHealth())/static_cast<float>(GetMaxHealth());
}

float APlayerCharacter::GetPlayerStatValue(const EPlayerStatType PlayerStat)
{
	return PlayerBattleState->GetStatValue(PlayerStat);
}

bool APlayerCharacter::IsPlayerDead() const
{
	return IsDead;
}

FVector APlayerCharacter::GetPlayerVelocity()
{
	return GetCharacterMovement()->Velocity;
}

void APlayerCharacter::GainGold(const float Amount) const
{
	PlayerBattleState->SetGold(PlayerBattleState->Gold + Amount);
	
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

