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

AAutoAimBullet* ATurret::SpawnBullet_Implementation(FVector TargetLocation)
{
	if (BulletClass == nullptr) return nullptr;

	const auto Bullet = GetWorld()->SpawnActor<AAutoAimBullet>(BulletClass, GetActorLocation(), FRotator::ZeroRotator);
	if (Bullet == nullptr) return nullptr;

	return Bullet;
}

