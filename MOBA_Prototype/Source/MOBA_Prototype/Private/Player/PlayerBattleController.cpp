// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerBattleController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Helpers.h"
#include "MobaPrototypeGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerBattleHud.h"
#include "Player/PlayerCharacter.h"

class UEnhancedInputLocalPlayerSubsystem;

void APlayerBattleController::BeginPlay()
{
	Super::BeginPlay();
	SpectatorPawn= GetPawn();
	OnPawnChangedOwnerClient.AddUFunction(this, "SetPawnHud");
	TryCreateChampionCharacter();

	OnPlayerStateReplicated.AddUFunction(this, "SetPlayerStateHud");
	OnPawnReplicated.AddUFunction(this, "SetPawnHud");
}

void APlayerBattleController::AttackInput(const FInputActionValue& ActionValue)
{
	CurrentBattleCharacter->AttackServer();
}

void APlayerBattleController::CancelAttackInput(const FInputActionValue& ActionValue)
{
	CurrentBattleCharacter->CancelAttackServer();
}

void APlayerBattleController::MoveInput(const FInputActionValue& ActionValue)
{
	CurrentBattleCharacter->Move(ActionValue.Get<FVector2D>());
	
}

void APlayerBattleController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &APlayerBattleController::MoveInput);
	EnhancedInputComponent->BindAction(AttackInputAction, ETriggerEvent::Triggered, this, &APlayerBattleController::AttackInput);
	EnhancedInputComponent->BindAction(CancelAttackInputAction, ETriggerEvent::Triggered, this, &APlayerBattleController::CancelAttackInput);
}

void APlayerBattleController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	UpdateBattleCharacter();
	OnPawnReplicated.Broadcast(GetPawn());
}

void APlayerBattleController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!CheckOwningClient()) return;
	if(CurrentBattleCharacter)
	{	
		if(GetDirectionFromCharacterPositionToMousePosition(MouseDirection))
		{
			CurrentBattleCharacter->RotateServer(MouseDirection);
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

void APlayerBattleController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	const auto State = GetPlayerState<APlayerBattleState>();

	OnPlayerStateReplicated.Broadcast(State);
}


void APlayerBattleController::RemoveInputMap_Implementation()
{
	RemoveBattleInputMapping();
	OnDeactivateBattleCharacterClientOwner();
	OnRemoveInputMapOwnerClient.Broadcast();
}

void APlayerBattleController::AddInputMap_Implementation()
{
	AddBattleInputMapping();
	OnActivateBattleCharacterClientOwner();
	OnAddInputMapOwnerClient.Broadcast();
}



void APlayerBattleController::SetPawnHud(APawn* InPawn)
{
	if(CheckOwningClient())
	Cast<APlayerBattleHud>(GetHUD())->SetPawn(InPawn);
}

void APlayerBattleController::SetPlayerStateHud(APlayerBattleState* InPlayerState)
{
	if(CheckOwningClient())
		Cast<APlayerBattleHud>(GetHUD())->SetPlayerState(InPlayerState);
}

void APlayerBattleController::UpdateBattleCharacter()
{
	CurrentBattleCharacter = Cast<APlayerCharacter>(GetPawn());
	if(CheckOwningClient())
	{	
		if(CurrentBattleCharacter == OldPawn) return;
		
		if(OnPawnChangedOwnerClient.IsBound()) OnPawnChangedOwnerClient.Broadcast((CurrentBattleCharacter));

		OldPawn = CurrentBattleCharacter;
		if(CurrentBattleCharacter == nullptr) return;

		AddBattleInputMapping();
		OnActivateBattleCharacterClientOwner();
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
	BattleCharacter = GetWorld()->SpawnActor<APlayerCharacter>(currentChampionClass,FVector(UKismetMathLibrary::RandomFloat()*100+1000.000000,1810.000000,92.012604), FRotator::ZeroRotator );
	BattleCharacter->OnDieServer.AddUFunction(this, "UnPossessBattleCharacterServer");
	BattleCharacter->OnRespawnServer.AddUFunction(this, "PossessBattleCharacterServer");
	Possess(BattleCharacter);
	SetPawnHud(BattleCharacter);
	SetPlayerStateHud(GetPlayerState<APlayerBattleState>());
	UpdateBattleCharacter();
	CurrentBattleCharacter->OnSpawnedServer();
}

void APlayerBattleController::UnPossessBattleCharacterServer()
{
	RemoveInputMap();
}

void APlayerBattleController::PossessBattleCharacterServer()
{
	AddInputMap();
}



