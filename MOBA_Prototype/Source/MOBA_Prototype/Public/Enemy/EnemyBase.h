// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "Interfaces/PlayerJoin.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Interfaces/Hitable.h"
#include "EnemyBase.generated.h"

UCLASS()
class MOBA_PROTOTYPE_API AEnemyBase : public ACharacter, public IHitable, public IPlayerJoin
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackRange = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCooldown = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanAttack = true;

	FTimerHandle AttackTimerHandle;

	void ResetAttackCooldown();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UActorComponent* TargetActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	ETeam Team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Meta = (ExposeOnSpawn = true))
	int CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHealth;

	void SetTeam(ETeam NewTeam); 
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	bool FindPointAroundTarget(FVector Target, float Radius);

	UFUNCTION(BlueprintNativeEvent)
	void MoveCloseToTarget(FVector Target, float AcceptanceRadius);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateHealth();

	UFUNCTION(BlueprintCallable)
	float GetPercentHealth();

	UFUNCTION(BlueprintNativeEvent)
	void ChangedTeam();

	UFUNCTION(BlueprintCallable)
	void TryAttack(AActor* Target);

	ETeam GetTeam() override;

	virtual void OnHit(FHitData HitData) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayerJoin_Implementation();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
