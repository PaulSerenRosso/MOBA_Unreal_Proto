// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "Player/PlayerBattleController.h"
#include "BattleCamera.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_PROTOTYPE_API ABattleCamera : public ACameraActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float distanceFromTarget;
	AActor* Target;
	UPROPERTY(EditAnywhere)
	FVector directionFromTarget;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	APlayerBattleController* Controller;
	UFUNCTION(BlueprintCallable)
	void OnPawnChanged(APawn* pawn);
};
