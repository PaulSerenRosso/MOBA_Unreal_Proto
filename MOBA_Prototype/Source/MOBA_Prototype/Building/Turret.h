// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	TArray<IHitable*> HittableTargets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETeam OwnTeam;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AddHittableTarget(TSubclassOf<AActor> Target, const ETeam Team);
	
	UFUNCTION(BlueprintCallable)
	void RemoveHittableTarget(TSubclassOf<AActor> Target);
};
