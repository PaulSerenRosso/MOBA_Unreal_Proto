// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerShootComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerBullet.h"

// Sets default values for this component's properties
UPlayerShootComponent::UPlayerShootComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlayerShootComponent::BeginPlay()
{
	Super::BeginPlay();
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

				auto Vector = PlayerPawnComponentHandlable->GetPlayerVelocity();
				Vector.Normalize();

				auto Vector2 = PlayerPawnComponentHandlable->GetPlayerRotation().Vector();
				Vector2.Normalize();

				auto DotDirection = UKismetMathLibrary::Dot_VectorVector(Vector,
					Vector2);
				DotDirection *= PlayerPawnComponentHandlable->GetPlayerVelocity().Length();
				auto Dmg = PlayerShootInfo->BulletDamage*PlayerPawnComponentHandlable->GetPlayerStatValue(EPlayerStatType::Damage);

				DotDirection = DotDirection < 0 ? 0 : DotDirection;
				
				Bullet->SetUp(Dmg, PlayerShootInfo->BulletSpeed, PlayerShootInfo->Range, PlayerPawnComponentHandlable->GetPlayerTeam(), GetOwner(), DotDirection);
				FireRate = 0;
			}
		}
	}
}

int UPlayerShootComponent::GetDamage()
{
	return PlayerShootInfo->BulletDamage;
}

void UPlayerShootComponent::OnCancelAttackServer_Implementation()
{
	TryShooting = false;
}

void UPlayerShootComponent::OnAttackServer_Implementation()
{
	TryShooting = true;
}


void UPlayerShootComponent::SetUp(IPlayerPawnComponentHandlable* PlayerHandler)
{
	PlayerPawnComponentHandlable = PlayerHandler;
	FireRate = PlayerShootInfo->FireRate;

	
}

