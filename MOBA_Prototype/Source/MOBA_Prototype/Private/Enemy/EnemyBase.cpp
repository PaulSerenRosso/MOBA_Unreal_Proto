// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	Execute_CallbackUpdateHealth(this);
	CallbackCanAttack(true);
}

void AEnemyBase::ResetAttackCooldown()
{
	CanAttack = true;
	CallbackCanAttack(true);
}

void AEnemyBase::SetTeam(const ETeam NewTeam)
{
	Team = NewTeam;
	ChangedTeam();
	Execute_CallbackUpdateHealth(this);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::TryAttack(AActor* Target)
{
	if (Target == nullptr) return;
	if (!HasAuthority()) return;
	
	const auto Dist = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
	if (Dist > AttackRange) return;

	const auto Hittable = Cast<IHitable>(Target);
	if (Hittable == nullptr) return;

	if (!CanAttack) return;
	if (Hittable->GetTeam() == Team) return;
	if (Hittable == this) return;

	Hittable->OnHit(FHitData( Damage, this, Team ));

	
	auto Str = "Attacked target: " + Target->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *FString(Str)));
	
	CanAttack = false;
	CallbackCanAttack(false);
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyBase::ResetAttackCooldown, AttackCooldown, false);
}

ETeam AEnemyBase::GetTeam()
{
	return Team;
}

void AEnemyBase::OnHit(FHitData HitData)
{
	if(!HasAuthority()) return;
	
	CurrentHealth -= HitData.Damage;
	UpdateHealthClients(CurrentHealth);
	
	if (CurrentHealth <= 0)
	{
		DieOnServer();
	}
}

void AEnemyBase::DieOnServer()
{
	DieOnClients();
}

void AEnemyBase::DieOnClients_Implementation()
{
	Destroy();
}

void AEnemyBase::UpdateHealthClients_Implementation(int InHealth)
{
	CurrentHealth = InHealth;
	Execute_CallbackUpdateHealth(this);
}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

int AEnemyBase::GetHealth()
{
	return CurrentHealth;
}

int AEnemyBase::GetMaxHealth()
{
	return MaxHealth;
}

float AEnemyBase::GetPercentageHealth()
{
	if (MaxHealth == 0) return 0.0f;
	
	return static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);
}

void AEnemyBase::ChangedTeam_Implementation()
{
}

void AEnemyBase::MoveCloseToTarget_Implementation(FVector Target, float AcceptanceRadius)
{
}

bool AEnemyBase::FindPointAroundTarget_Implementation(FVector Target, float Radius)
{
	// Will be implemented in the blueprint child classes
	return false;
}

