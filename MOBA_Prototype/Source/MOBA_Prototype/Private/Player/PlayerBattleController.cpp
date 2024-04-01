// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerBattleController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MobaPrototypeGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"

class UEnhancedInputLocalPlayerSubsystem;

void APlayerBattleController::BeginPlay()
{
	Super::BeginPlay();
	TryCreateChampionCharacter();
}

void APlayerBattleController::MoveInput(const FInputActionValue& ActionValue)
{
	BattleCharacter->Move(ActionValue.Get<FVector2D>());
	
}

void APlayerBattleController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &APlayerBattleController::MoveInput);
	EnhancedInputComponent->BindAction(ShootInputAction, ETriggerEvent::Triggered, this, &APlayerBattleController::SendInputShootServer);
}

void APlayerBattleController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	
	UE_LOG(LogTemp, Warning, TEXT("TEST"));
	UpdateInputMappingClient();
}


void APlayerBattleController::SendInputShootServer_Implementation(const FInputActionValue& ActionValue)
{
	
}

bool APlayerBattleController::CheckOwningClient()
{
	auto LocalRole = GetLocalRole();
	if(LocalRole == ROLE_SimulatedProxy) return false;
	if (LocalRole == ROLE_Authority && !IsLocalPlayerController()) return false;
	return true;
}

void APlayerBattleController::TryCreateChampionCharacter()
{
	if (!CheckOwningClient()) return;
	auto gameInstance = Cast<UMobaPrototypeGameInstance>(GetGameInstance());
	if(gameInstance != nullptr)
	{
		ChampionClass = gameInstance->GetPlayerChampionClass();
		SpawnPlayerChampionCharacterServer(ChampionClass);
	}
}




void APlayerBattleController::UpdateInputMappingClient()
{
	
	BattleCharacter = Cast<APlayerCharacter>(GetPawn());
	if(BattleCharacter == OldPawn) return;
	OldPawn = BattleCharacter;
	if(!CheckOwningClient()) return;
	if(BattleCharacter)
	{
		//UE_LOG(LogTemp, Warning, TEXT("TESTaa"));
		AddBattleInputMapping();
	}
	else
	{
		RemoveBattleInputMapping();
	}
}

void APlayerBattleController::AddBattleInputMapping()
{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(BattleInputMappingContext, 0);
		
		}
}

void APlayerBattleController::RemoveBattleInputMapping()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
	}
}

void APlayerBattleController::SpawnPlayerChampionCharacterServer_Implementation(UClass* currentChampionClass)
{
	UnPossess();
	auto CurrentCharacter = GetWorld()->SpawnActor<APlayerCharacter>(currentChampionClass,FVector(UKismetMathLibrary::RandomFloat()*100+1000.000000,1810.000000,92.012604), FRotator::ZeroRotator );
	Possess(CurrentCharacter);

	UpdateInputMappingClient();
}




