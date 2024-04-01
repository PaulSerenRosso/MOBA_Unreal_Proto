// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AutoAimBullet.h"
#include "Enums.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Hitable.h"
#include "Turret.generated.h"

UCLASS()
class MOBA_PROTOTYPE_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	FTimerHandle AttackTimer;
	bool IsAttacking = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AttackInterval = 2.0f;
	
	TArray<IHitable*> HittableTargets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETeam OwnTeam;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int targetCount = 0;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Attack();

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
};
