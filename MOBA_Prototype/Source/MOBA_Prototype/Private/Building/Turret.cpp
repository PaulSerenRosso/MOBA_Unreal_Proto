// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Turret.h"

#include "AutoAimBullet.h"
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
	CurrentHealth = MaxHealth;

	auto GM = GetWorld()->GetAuthGameMode();
	if (GM == nullptr) return;

	const auto GameModeBattle = Cast<AGameModeBattle>(GM);
	if (GameModeBattle == nullptr) return;

	GameModeBattle->AddTurret(this, OwnTeam);
	Execute_CallbackUpdateHealth(this);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurret::AddHittableTarget(AActor* Target)
{
	IHitable* Hittable = Cast<IHitable>(Target);

	if (Hittable == nullptr) return;

	if (const ETeam Team = Hittable->GetTeam(); Team == ETeam::Neutral || Team == OwnTeam) return;
	if (HittableTargets.Contains(Hittable)) return;
	
	HittableTargets.Add(Hittable);

	targetCount = HittableTargets.Num();

	if (targetCount > 0 && !IsAttacking)
	{
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
	
	FHitData* HitData = new FHitData();
	HitData->Damage = 20;
	HitData->HitBy = this;
	HitData->InstigatorTeam = OwnTeam;
	//if (Activated) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Turret %s is attacking %s"), *GetName(), *Cast<AActor>(Target)->GetName()));

	if (Activated)
	{
		const auto Bullet = SpawnBullet(GetActorLocation() + FVector(0, 0, 500));
		Bullet->Init(Target, HitData);
	}
	
	GetWorldTimerManager().SetTimer(AttackTimer, this, &ATurret::Attack, AttackInterval);
}

void ATurret::RemoveHittableTarget(AActor* Target)
{
	IHitable* Hittable = Cast<IHitable>(Target);
	if (Hittable == nullptr) return;

	HittableTargets.Remove( Hittable );

	targetCount = HittableTargets.Num();
}

ETeam ATurret::GetTeam()
{
	return OwnTeam;
}

void ATurret::OnHit(FHitData HitData)
{
	CurrentHealth -= HitData.Damage;
	Execute_CallbackUpdateHealth(this);
	if (CurrentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("A turret has been destroyed!")));
		Destroy();
	}
}

void ATurret::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

int ATurret::GetHealth()
{
	return CurrentHealth;
}

int ATurret::GetMaxHealth()
{
	return MaxHealth;
}

float ATurret::GetPercentageHealth()
{
	if (MaxHealth == 0) return 0.0f;
	
	return static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);
}

AAutoAimBullet* ATurret::SpawnBullet_Implementation(FVector TargetLocation)
{
	if (BulletClass == nullptr) return nullptr;

	const auto Bullet = GetWorld()->SpawnActor<AAutoAimBullet>(BulletClass, GetActorLocation(), FRotator::ZeroRotator);
	if (Bullet == nullptr) return nullptr;

	return Bullet;
}

