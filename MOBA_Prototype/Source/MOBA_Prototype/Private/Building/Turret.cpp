// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Turret.h"

#include "AutoAimBullet.h"
#include "EngineServiceMessages.h"
#include "GameModeBattle.h"
#include "GameStateBattle.h"
#include "MobaPrototypeGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerBattleController.h"

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
	CurrentHealth = TurretInfo->MaxHealth;

	auto GameState = GetWorld()->GetGameState();
	if (GameState == nullptr) return;

	const auto GameStateBattle = Cast<AGameStateBattle>(GameState);
	if (GameStateBattle == nullptr) return;

	GameStateBattle->AddTurret(this, OwnTeam);
	
	Execute_CallbackUpdateHealth(this);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurret::AddHittableTarget(AActor* Target)
{
	if (!HasAuthority()) return;
	IHitable* Hittable = Cast<IHitable>(Target);

	if (Hittable == nullptr) return;

	if (const ETeam Team = Hittable->GetTeam(); Team == ETeam::Neutral || Team == OwnTeam) return;
	if (HittableTargets.Contains(Hittable)) return;

	auto Player = Cast<APlayerCharacter>(Target);
	if (Player != nullptr)
	{
		if (Player->IsPlayerDead())
		{
			return;
		}
	}
	
	HittableTargets.Add(Hittable);

	targetCount = HittableTargets.Num();

	if (targetCount > 0 && !IsAttacking)
	{
		IsAttacking = true;
		Attack();
	}
}

void ATurret::Attack()
{
	//if (Activated) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Attacking")));
	if (HittableTargets.Num() == 0)
	{
		IsAttacking = false;
		return;
	}
	
	IHitable* Target = HittableTargets[0];
	if (Target == nullptr)
	{
		HittableTargets.RemoveAt(0);
		Attack();
		return;
	}
	auto Player = Cast<APlayerCharacter>(Target);
	if (Player != nullptr)
	{
		if (Player->IsPlayerDead())
		{
			HittableTargets.RemoveAt(0);
			Attack();
			return;
		}
	}
	
	FHitData* HitData = new FHitData();
	HitData->Damage = TurretInfo->Damage;
	HitData->HitBy = this;
	HitData->InstigatorTeam = OwnTeam;
	//if (Activated) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Turret %s is attacking %s"), *GetName(), *Cast<AActor>(Target)->GetName()));

	if (Activated)
	{
		const auto Location = GetActorLocation() + FVector(0, 0, 500);
		SpawnBulletsOnClients(Location, HitData, Target);
	}
	
	GetWorldTimerManager().SetTimer(AttackTimer, this, &ATurret::Attack, TurretInfo->AttackCooldown);
}

void ATurret::RemoveHittableTarget(AActor* Target)
{
	if (!HasAuthority()) return;
	
	IHitable* Hittable = Cast<IHitable>(Target);
	if (Hittable == nullptr) return;

	HittableTargets.Remove( Hittable );

	targetCount = HittableTargets.Num();
}

ETeam ATurret::GetTeam()
{
	return OwnTeam;
}

void ATurret::OnHit(const FHitData HitData)
{
	CurrentHealth -= HitData.Damage;
	UpdateHealthClients(CurrentHealth);
	if (CurrentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("A turret has been destroyed!")));
		DieOnServer();
	}
}

void ATurret::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATurret, CurrentHealth);
	DOREPLIFETIME(ATurret, OwnTeam);
}

int ATurret::GetHealth()
{
	return CurrentHealth;
}

int ATurret::GetMaxHealth()
{
	return TurretInfo->MaxHealth;
}

float ATurret::GetPercentageHealth()
{
	if (TurretInfo->MaxHealth == 0) return 0.0f;
	
	return static_cast<float>(CurrentHealth) / static_cast<float>(TurretInfo->MaxHealth);
}

void ATurret::DieOnServer()
{
	DieOnClients();
}

void ATurret::SpawnBulletsOnClients(FVector TargetLocation, FHitData* HitData, IHitable* Target)
{
	const auto Bullet = SpawnBullet(TargetLocation);
	Bullet->Init(Target, HitData);
}

void ATurret::MoveToEndGame_Implementation()
{
}

void ATurret::UpdateHealthClients_Implementation(const int InHealth)
{
	CurrentHealth = InHealth;
	Execute_CallbackUpdateHealth(this);
}

void ATurret::DieOnClients_Implementation()
{
	const auto GameInstance = Cast<UMobaPrototypeGameInstance>(GetGameInstance());
	const ETeam WinnerTeam = OwnTeam == ETeam::Team1 ? ETeam::Team2 : ETeam::Team1;
	
	if (GameInstance != nullptr) GameInstance->SetWinnerTeam(WinnerTeam);
	
	SetHidden(true);

	// Execute command :
	// FString Final = "disconnect";
	// GetWorld()->Exec(GetWorld(), *Final);
	// UGameplayStatics::OpenLevel(GetWorld(), "EndGame");
	MoveToEndGame();
	//Destroy();
}

AAutoAimBullet* ATurret::SpawnBullet_Implementation(FVector TargetLocation)
{
	if (BulletClass == nullptr) return nullptr;

	const auto Bullet = GetWorld()->SpawnActor<AAutoAimBullet>(BulletClass, GetActorLocation(), FRotator::ZeroRotator);
	if (Bullet == nullptr) return nullptr;

	return Bullet;
}

