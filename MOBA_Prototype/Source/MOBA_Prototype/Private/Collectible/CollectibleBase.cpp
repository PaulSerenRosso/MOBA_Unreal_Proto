// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible/CollectibleBase.h"
#include "Components/SphereComponent.h"
#include "Player/PlayerCharacter.h"

// Sets default values
ACollectibleBase::ACollectibleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACollectibleBase::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision = FindComponentByClass<USphereComponent>();

	if (SphereCollision)
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACollectibleBase::OnOverlapBegin);
	}
	
}

// Called every frame
void ACollectibleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectibleBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto Unit = Cast<AUnit>(OtherActor);
	Collect(Unit);
}

void ACollectibleBase::OnCollectedClient_Implementation(AUnit* Collector)
{
	// Move collectible to collector with lerp and then destroy
	OnCollected.Broadcast(this, Collector);
	Destroy();
}

void ACollectibleBase::Collect_Implementation(AUnit* Collector)
{
	OnCollectedClient(Collector);
}

