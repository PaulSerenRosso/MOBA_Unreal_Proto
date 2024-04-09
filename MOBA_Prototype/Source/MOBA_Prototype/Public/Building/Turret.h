// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AutoAimBullet.h"
#include "Enums.h"
#include "DesignData/TurretInfo.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Healthable.h"
#include "Interfaces/Hitable.h"
#include "Turret.generated.h"

UCLASS()
class MOBA_PROTOTYPE_API ATurret : public AActor, public IHitable, public IHealthable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	FTimerHandle AttackTimer;
	bool IsAttacking = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTurretInfo* TurretInfo;
	
	TArray<IHitable*> HittableTargets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	ETeam OwnTeam;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int targetCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Meta = (ExposeOnSpawn = true))
	int CurrentHealth;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Attack();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTeamMats();

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClass* BulletClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Activated;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AddHittableTarget(AActor* Target);
	
	UFUNCTION(BlueprintCallable)
	void RemoveHittableTarget(AActor* Target);

	UFUNCTION(BlueprintNativeEvent)
	AAutoAimBullet* SpawnBullet(FVector TargetLocation);
	
	virtual ETeam GetTeam() override;

	virtual void OnHit(FHitData HitData) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual int GetHealth() override;
	virtual int GetMaxHealth() override;
	virtual float GetPercentageHealth() override;

	virtual void DieOnServer();
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void DieOnClients();
	
	UFUNCTION(NetMulticast,Reliable)
	virtual void UpdateHealthClients(int InHealth);
	UFUNCTION(Server, Reliable)
	virtual void SpawnBulletsServer(FVector TargetLocation, int Damage, IHitable* Target);

	UFUNCTION(BlueprintNativeEvent)
	void MoveToEndGame();
};
