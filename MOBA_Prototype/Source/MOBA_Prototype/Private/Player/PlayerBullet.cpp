// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerBullet.h"
#include "Helpers.h"
#include "Interfaces/Hitable.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APlayerBullet::APlayerBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerBullet::SetUp(float InDamage, float InSpeed, float InRange, ETeam InTeam, AActor* InHitBy)
{
	this->Damage = InDamage;
	this->Speed = InSpeed;
	this->Team = InTeam;
	this->HitBy = InHitBy;
	LifeTime = InRange/InSpeed;
	GetWorldTimerManager().SetTimer(DestructionTimerHandle,this,  &APlayerBullet::DestroyBullet, LifeTime, false);
	IsSetup = true;
	BoxCollision = Cast<UBoxComponent>(GetRootComponent());
	BoxCollision->SetPhysicsLinearVelocity(GetActorForwardVector()*Speed, false);
}
void APlayerBullet::DestroyBullet()
{
	if(IsSetup)
	Destroy();
}

void APlayerBullet::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if(!HasAuthority()) return;
	IHitable* Hitable = Cast<IHitable>(OtherActor);
	if(Hitable != nullptr)
	{
		if(UHelpers::GetEnemyTeam(Team) == Hitable->GetTeam())
		{
			FHitData HitData = FHitData();
			HitData.Damage = Damage;
			HitData.HitBy = HitBy;
			Hitable->OnHit(FHitData(HitData));
			GetWorldTimerManager().ClearTimer(DestructionTimerHandle);
			DestroyBullet();
		}
	}
	
	
}

void APlayerBullet::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	GetWorldTimerManager().ClearTimer(DestructionTimerHandle);
	DestroyBullet();
}

// Called when the game starts or when spawned
void APlayerBullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerBullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerBullet, Speed);
}
