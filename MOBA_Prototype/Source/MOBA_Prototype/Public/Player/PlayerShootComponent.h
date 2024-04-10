// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerShootInfo.h"
#include "Components/ActorComponent.h"
#include "Interfaces/PlayerAttackable.h"
#include "PlayerShootComponent.generated.h"


UCLASS( ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent) )
class MOBA_PROTOTYPE_API UPlayerShootComponent : public UActorComponent, public IPlayerAttackable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UPlayerShootComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UPlayerShootInfo* PlayerShootInfo;
	IPlayerPawnComponentHandlable* PlayerPawnComponentHandlable;
	float FireRate;
	
	
	bool TryShooting;
	virtual void SetUp(IPlayerPawnComponentHandlable* PlayerHandler) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:	
	// Called every frame

	
	UFUNCTION(Server, Reliable)
	virtual void OnAttackServer() override;

	UFUNCTION(Server, Reliable)
virtual void OnCancelAttackServer() override;

	virtual int GetDamage() override;
};
