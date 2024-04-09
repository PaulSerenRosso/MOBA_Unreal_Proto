// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "DesignData/EnemyAttackInfo.h"
#include "Interfaces/PlayerJoin.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Interfaces/Hitable.h"
#include "Interfaces/Healthable.h"
#include "EnemyBase.generated.h"

UCLASS()
class MOBA_PROTOTYPE_API AEnemyBase : public ACharacter, public IHitable, public IPlayerJoin, public IHealthable
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
	UEnemyAttackInfo* EnemyAttackInfo;

	void SetTeam(ETeam NewTeam); 
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	bool FindPointAroundTarget(FVector Target, float Radius);

	UFUNCTION(BlueprintNativeEvent)
	void MoveCloseToTarget(FVector Target, float AcceptanceRadius);

	UFUNCTION(BlueprintNativeEvent)
	void ChangedTeam();

	UFUNCTION(BlueprintCallable)
	virtual void TryAttack(AActor* Target);

	UFUNCTION(BlueprintImplementableEvent)
	void CallbackCanAttack(bool CanAttackValue);

	virtual ETeam GetTeam() override;

	virtual void OnHit(FHitData HitData) override;


	
	virtual void DieOnServer();
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void DieOnClients();
	
	UFUNCTION(NetMulticast,Reliable)
	virtual void UpdateHealthClients(int InHealth);

	


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void PlayerJoin_Implementation();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual int GetHealth() override;
	virtual int GetMaxHealth() override;
	virtual float GetPercentageHealth() override;
};
