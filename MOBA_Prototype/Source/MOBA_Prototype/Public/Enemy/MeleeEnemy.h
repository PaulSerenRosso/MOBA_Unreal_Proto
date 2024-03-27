// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "MeleeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API AMeleeEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeleeEnemy")
	UStaticMeshComponent* MeshComponent;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeleeEnemy")
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeleeEnemy")
	UActorComponent* target;
};
