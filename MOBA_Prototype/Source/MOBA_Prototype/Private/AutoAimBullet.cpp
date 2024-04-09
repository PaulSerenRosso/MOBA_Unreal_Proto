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
	if (!HasAuthority()) return;
	
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

	if (!bHit) return;

	auto HitActor = ActorHitData.GetActor();
	
	if (!HitActor) return;

	auto HitTarget = Cast<IHitable>(HitActor);
	
	if (HitTarget == nullptr) return;

	if (HitTarget->GetTeam() == HitData->InstigatorTeam) return;
	
	HitTarget->OnHit(*HitData);

	DestroyOnServer();
}

void AAutoAimBullet::DestroyOnServer()
{
	DestroyOnClient();
}

void AAutoAimBullet::DestroyOnClient_Implementation()
{
	Destroy();
}

// Called every frame
void AAutoAimBullet::Tick(float DeltaTime)
{
	if(!HasAuthority()) return;
	Super::Tick(DeltaTime);
	if (!TargetActor.IsValid() || TargetActor == nullptr) {
		Destroy();
		return;
	}
	
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
	if(!HasAuthority()) return;
	if (Trget == nullptr || Hit == nullptr)
	{
		Destroy();
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Bullet Init")));
	
	Target = Trget;
	TargetActor = Cast<AActor>(Target);
	HitData = Hit;
	Speed = Spd;
}

