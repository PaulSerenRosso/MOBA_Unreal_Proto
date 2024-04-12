// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/CollectibleSpawner.h"

#include "Collectible/CollectibleBase.h"

// Sets default values
ACollectibleSpawner::ACollectibleSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACollectibleSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) SpawnCollectibleAfterDelay();
}

// Called every frame
void ACollectibleSpawner::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectibleSpawner::SpawnCollectible()
{
	SpawnCollectibleClients();
}

void ACollectibleSpawner::SpawnCollectibleAfterDelay()
{
	if (!HasAuthority()) return;

	//RespawnTimer
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ACollectibleSpawner::SpawnCollectible, RespawnTime, false);
}

void ACollectibleSpawner::OnCollectibleCollected(AActor* Collectible, AActor* Collector)
{
	SpawnCollectibleAfterDelay();
}

void ACollectibleSpawner::SpawnCollectibleClients_Implementation()
{
	if (CollectibleClass == nullptr) return;

	auto Location = GetActorLocation();
	Location.Z += 50.0f;
	
	ACollectibleBase* Collectible = GetWorld()->SpawnActor<ACollectibleBase>(CollectibleClass, Location, GetActorRotation());

	if (Collectible == nullptr) return;
	if (!HasAuthority()) return;

	Collectible->OnCollected.AddUFunction(this, "OnCollectibleCollected");
}

