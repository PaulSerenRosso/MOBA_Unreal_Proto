// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerBattleController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MobaPrototypeGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"

class UEnhancedInputLocalPlayerSubsystem;

void APlayerBattleController::BeginPlay()
{
	Super::BeginPlay();
	TryCreateChampionCharacter();
}

void APlayerBattleController::AttackInput(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Je test l'attack"));
	
	BattleCharacter->AttackServer();
}

void APlayerBattleController::CancelAttackInput(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Je test le cancel"));
	BattleCharacter->CancelAttackServer();
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
	EnhancedInputComponent->BindAction(AttackInputAction, ETriggerEvent::Triggered, this, &APlayerBattleController::AttackInput);
	EnhancedInputComponent->BindAction(CancelAttackInputAction, ETriggerEvent::Triggered, this, &APlayerBattleController::CancelAttackInput);
	cpt = 0;
}

void APlayerBattleController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
//	UE_LOG(LogTemp, Warning, TEXT("TEST"));
	UpdateInputMappingClient();
}

void APlayerBattleController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!CheckOwningClient()) return;
	if(BattleCharacter)
	{	
		if(GetDirectionFromCharacterPositionToMousePosition(MouseDirection))
		{
			BattleCharacter->RotateServer(MouseDirection);
		}
		
	}
}


bool APlayerBattleController::GetDirectionFromCharacterPositionToMousePosition(FVector& Direction)
{
		APawn* CurrentPawn = GetPawn();
		if (!CurrentPawn)
			return false;

		FVector2D MousePosition;
		if (!GetMousePosition(MousePosition.X, MousePosition.Y))
			return false;
	
		FVector WorldLocation, WorldDirection;
		if (!DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
			return false;
	FVector PawnLocation = CurrentPawn->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Mouse Position X: %f  Y: %f PlayerPosition : X: %f  Y: %f "), WorldLocation.X, WorldLocation.Y,  PawnLocation.X, PawnLocation.Y);
	
		Direction.X = WorldLocation.X - PawnLocation.X;
		Direction.Y = WorldLocation.Y - PawnLocation.Y;
		Direction.Normalize();
		return true;
	
}

void APlayerBattleController::SetCameraToControllerServer_Implementation(AActor* Camera)
{
	SetViewTargetWithBlend(Camera);
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
	if(CheckOwningClient())
	{	
	if(BattleCharacter == OldPawn) return;
	if(OnPawnChangedOwnerClient.IsBound())
	OnPawnChangedOwnerClient.Execute((BattleCharacter));
	OldPawn = BattleCharacter;
	if(BattleCharacter)
	{
		BattleCharacter->OnSpawnedServer();
		//UE_LOG(LogTemp, Warning, TEXT("TESTaa"));
		AddBattleInputMapping();
		OnActivateBattleCharacterClientOwner();
	
	}
	else
	{
		BattleCharacter->OnSpawnedServer();
		RemoveBattleInputMapping();
		OnDeactivateBattleCharacterClientOwner();
	}
			
		};
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
	BattleCharacter->OnSpawnedServer();
}




