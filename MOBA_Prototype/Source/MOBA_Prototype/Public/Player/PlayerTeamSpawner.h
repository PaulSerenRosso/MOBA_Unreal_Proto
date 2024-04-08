// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Actor.h"
#include "PlayerTeamSpawner.generated.h"

UCLASS()
class MOBA_PROTOTYPE_API APlayerTeamSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerTeamSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	ETeam Team;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
