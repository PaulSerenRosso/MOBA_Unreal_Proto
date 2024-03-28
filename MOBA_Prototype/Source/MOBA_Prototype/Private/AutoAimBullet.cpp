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

// Called every frame
void AAutoAimBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target == nullptr || HitData == nullptr) return;

	const FVector TargetLocation = Cast<AActor>(Target)->GetActorLocation();
	FVector Direction = TargetLocation - GetActorLocation();
	Direction.Normalize();

	auto NewLocation = GetActorLocation() + Direction * Speed * DeltaTime;

	// Check if the bullet will hit the target
	SetActorLocation(NewLocation);
	

	if (FVector::Dist(GetActorLocation(), TargetLocation) < 50.0f)
	{
		Target->OnHit(*HitData);
	}

	Destroy();
}

void AAutoAimBullet::Init(IHitable* Trget, FHitData* Hit, const float Spd)
{
	if (Trget == nullptr || Hit == nullptr)
	{
		Destroy();
		return;
	}
	
	Target = Trget;
	HitData = Hit;
	Speed = Spd;
}

