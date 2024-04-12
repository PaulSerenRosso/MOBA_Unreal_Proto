// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Healthable.h"
#include "Interfaces/Hitable.h"
#include "Unit.generated.h"

UCLASS()
class MOBA_PROTOTYPE_API AUnit : public ACharacter, public IHitable, public IHealthable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnit();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Meta = (ExposeOnSpawn = true))
	int CurrentHealth;
	
	UPROPERTY(Replicated)
	bool IsDead = false;

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual ETeam GetTeam() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsUnitDead() const;

	virtual void Heal(float HealAmount, bool IsPercent);
	virtual int GetHealth() override;
	virtual int GetMaxHealth() override;
	virtual float GetPercentageHealth() override;
	virtual void OnHit(FHitData HitData) override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTeamColor();

	UFUNCTION(BlueprintNativeEvent)
	void ChangedTeam();
	
	UFUNCTION(NetMulticast,Reliable)
	virtual void UpdateHealthClients(int InHealth);
};
