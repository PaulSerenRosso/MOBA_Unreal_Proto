// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChampionData.h"
#include "Enums.h"
#include "PlayerBattleState.h"
#include "GameFramework/Character.h"
#include "Interfaces/Healthable.h"
#include "Interfaces/Hitable.h"
#include "Interfaces/PlayerAttackable.h"
#include "Interfaces/PlayerPawnComponentHandlable.h"
#include "UI/HealthWidgetComponent.h"
#include "PlayerCharacter.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnDie);
UCLASS()
class MOBA_PROTOTYPE_API APlayerCharacter : public ACharacter, public IPlayerPawnComponentHandlable, public IHitable, public IHealthable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	FOnDie OnDieServer;
	FOnDie OnRespawnServer;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<IPlayerAttackable*> PlayerAttackables;
	UPROPERTY(EditAnywhere)
	UChampionData* ChampionData;
	APlayerBattleState* PlayerBattleState;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(Replicated)
	ETeam Team;
	AActor* TeamSpawner;
	USkeletalMeshComponent* SkeletalMesh;
	UHealthWidgetComponent* HealthWidget;
	FTimerHandle RespawnTimer;
	UPROPERTY(Replicated)
	bool IsDie = false;
	int CurrentHealth;
	virtual void DieServer();
	virtual void RespawnPlayerServer();
	UFUNCTION(NetMulticast, Reliable)
	virtual void DieClients();
	UFUNCTION(NetMulticast, Reliable)
	virtual void RespawnPlayerClients();
	UFUNCTION(NetMulticast,Reliable)
	virtual void UpdateHealthClients(int InHealth);
	
public:
	virtual void Move(FVector2D Direction);
	UPROPERTY(Replicated)
	FRotator CurrentRotation;
	UFUNCTION(Server, Unreliable)
	virtual void RotateServer(FVector Direction);
	virtual FVector GetPlayerPosition() override;
	virtual FRotator GetPlayerRotation() override;
	virtual ETeam GetPlayerTeam() override;
	UFUNCTION(Server, Unreliable)
	virtual void AttackServer();
	UFUNCTION(Server, Unreliable)
	virtual void CancelAttackServer();
	virtual void OnHit(FHitData HitData) override;
	virtual ETeam GetTeam() override;
	virtual void OnSpawnedServer();
	virtual int GetHealth() override;
	virtual int GetMaxHealth() override;
	virtual float GetPercentageHealth() override;
	
	


	
};
