// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/PlayerAttackable.h"
#include "PlayerShootComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOBA_PROTOTYPE_API UPlayerShootComponent : public UActorComponent, public IPlayerAttackable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerShootComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnAttack() override;
	virtual void SetUp(IPlayerPawnComponentHandlable* PlayerHandler) override;
		
};
