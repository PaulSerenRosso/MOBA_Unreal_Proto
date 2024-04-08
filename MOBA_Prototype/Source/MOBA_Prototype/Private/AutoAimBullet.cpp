// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoAimBullet.h"

// Sets default values
AAutoAimBullet::AAutoAimBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAutoAimBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAutoAimBullet::Ray(float Distance)
{
	FVector Start = GetActorLocation();
	const FVector TargetLocation = Cast<AActor>(Target)->GetActorLocation();
	FVector Direction = TargetLocation - GetActorLocation();
	Direction.Normalize();

	FVector End = Start + Direction * Distance;

	if (!GetWorld()) return;

	FHitResult ActorHitData;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(ActorHitData, Start, End, ECC_Pawn, CollisionParams);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f, 0, 10.0f);

	if (!bHit) return;

	auto HitActor = ActorHitData.GetActor();
	
	if (!HitActor) return;

	auto HitTarget = Cast<IHitable>(HitActor);
	
	if (HitTarget == nullptr) return;

	if (HitTarget->GetTeam() == HitData->InstigatorTeam) return;
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("BULLET : Hit a HitTarget")));
	HitTarget->OnHit(*HitData);

	// auto Str = "Hit target: " + HitActor->GetName();
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *FString(Str)));
	Destroy();
}

// Called every frame
void AAutoAimBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target == nullptr || HitData == nullptr) return;
	const auto TargetActor = Cast<AActor>(Target);
	if (TargetActor == nullptr) return;
	
	const FVector TargetLocation = TargetActor->GetActorLocation();
	FVector Direction = TargetLocation - GetActorLocation();
	Direction.Normalize();

	auto NewLocation = GetActorLocation() + Direction * Speed * DeltaTime;

	// Check if the bullet will hit the target
	Ray(Speed * DeltaTime);
	SetActorLocation(NewLocation);
}

void AAutoAimBullet::Init(IHitable* Trget, FHitData* Hit, const float Spd)
{
	if (Trget == nullptr || Hit == nullptr)
	{
		Destroy();
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Bullet Init")));
	
	Target = Trget;
	HitData = Hit;
	Speed = Spd;
}

