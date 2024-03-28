// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

UCLASS()
class MOBA_PROTOTYPE_API AEnemyBase : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UActorComponent* TargetActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETeam Team;
	

	void SetTeam(ETeam NewTeam); 
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	bool FindPointAroundTarget(FVector Target, float Radius);

	UFUNCTION(BlueprintNativeEvent)
	void MoveCloseToTarget(FVector Target, float AcceptanceRadius);

	UFUNCTION(BlueprintNativeEvent)
	void ChangedTeam();

};
