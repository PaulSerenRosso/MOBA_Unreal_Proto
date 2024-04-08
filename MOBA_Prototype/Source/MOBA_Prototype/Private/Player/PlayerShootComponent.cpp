// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerShootComponent.h"
#include "Player/PlayerBullet.h"

// Sets default values for this component's properties
UPlayerShootComponent::UPlayerShootComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
	// ...
}


// Called when the game starts
void UPlayerShootComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerShootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(GetOwner()->HasAuthority())
	{
		if(FireRate < PlayerShootInfo->FireRate)
		{
			FireRate += DeltaTime;
		
		}
		else
		{
			if(TryShooting)
			{
				APlayerBullet* Bullet =	GetWorld()->SpawnActor<APlayerBullet>(PlayerShootInfo->BulletClass,PlayerPawnComponentHandlable->GetPlayerPosition(), PlayerPawnComponentHandlable->GetPlayerRotation());
				Bullet->SetUp(PlayerShootInfo->BulletDamage, PlayerShootInfo->BulletSpeed, PlayerShootInfo->Range, PlayerPawnComponentHandlable->GetPlayerTeam());
				FireRate = 0;
			}
		}
	}
	
	// ...
}

void UPlayerShootComponent::OnCancelAttackServer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("CANCEL False"));
	TryShooting = false;
}

void UPlayerShootComponent::OnAttackServer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack TRUE"));
	TryShooting = true;
}


void UPlayerShootComponent::SetUp(IPlayerPawnComponentHandlable* PlayerHandler)
{
	PlayerPawnComponentHandlable = PlayerHandler;
	FireRate = PlayerShootInfo->FireRate;

	
}

