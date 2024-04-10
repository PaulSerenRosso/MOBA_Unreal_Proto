// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:


protected:
	TWeakObjectPtr<AActor> TargetActor;
	TArray<AActor*> TrueTargets;

	virtual void BeginPlay() override;

	void MakeTrueTargets(TArray<AActor*> Targets);

	AActor* GetClosestTarget(TArray<AActor*> Targets) const;

	void SetBlackboardTarget(AActor* Target, UBlackboardComponent* BlackboardComponent);

	UFUNCTION(BlueprintCallable)
	void OnUpdatePerception(TArray<AActor*> Targets, UBlackboardComponent* BlackboardComponent);
};
