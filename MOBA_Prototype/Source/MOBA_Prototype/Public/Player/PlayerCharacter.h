// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Character.h"
#include "Interfaces/PlayerPawnComponentHandlable.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MOBA_PROTOTYPE_API APlayerCharacter : public ACharacter, public IPlayerPawnComponentHandlable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETeam Team;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Move(FVector2D Direction);

	UPROPERTY(Replicated)
	FRotator CurrentRotation;
	UFUNCTION(Server, Unreliable)
	virtual void RotateServer(FVector Direction);
	virtual FVector GetPlayerPosition() override;
	
};
