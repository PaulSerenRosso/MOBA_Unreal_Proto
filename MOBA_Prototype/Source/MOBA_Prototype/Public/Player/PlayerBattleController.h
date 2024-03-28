// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "PlayerBattleController.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnPawnChanged, APawn*);
UCLASS()
class MOBA_PROTOTYPE_API APlayerBattleController : public APlayerController
{
	GENERATED_BODY()


public:
	bool GetDirectionFromCharacterPositionToMousePosition(FVector& Direction);
	FOnPawnChanged OnPawnChanged;
private:
	UClass* ChampionClass;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* BattleInputMappingContext;
	UPROPERTY(EditAnywhere)
	UInputAction* MoveInputAction;
	UPROPERTY(EditAnywhere)
	UInputAction* ShootInputAction;
	APlayerCharacter* BattleCharacter;
	APawn* OldPawn;
	FVector MouseDirection;
	
protected:
	virtual void UpdateInputMappingClient();
	virtual void AddBattleInputMapping();
	virtual void RemoveBattleInputMapping();
	bool CheckOwningClient();
	UFUNCTION(BlueprintCallable)
	void TryCreateChampionCharacter();
	UFUNCTION(Server, Reliable)
	void SpawnPlayerChampionCharacterServer(UClass* currentChampionClass);
	virtual void BeginPlay() override;
	UFUNCTION(Server, Reliable)
	virtual void SendInputShootServer(const FInputActionValue& ActionValue);
	virtual void MoveInput(const FInputActionValue& ActionValue);
	virtual void SetupInputComponent() override;
	virtual void OnRep_Pawn() override;
	virtual void Tick(float DeltaSeconds) override;
	
	
	
};
