// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/BattleCamera.h"

#include "GameModeBattle.h"
#include "GameStateBattle.h"
#include "Kismet/GameplayStatics.h"

void ABattleCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(Target)
	{
		SetActorLocation(Target->GetActorLocation()+directionFromTarget*distanceFromTarget);
	}
}

void ABattleCamera::BeginPlay()
{
	Super::BeginPlay();
	directionFromTarget.Normalize();
	UE_LOG(LogTemp, Warning, TEXT("Begin play"));
	Controller = Cast<APlayerBattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(Controller != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller not null"));
		Controller->OnRemoveInputMapOwnerClient.AddUFunction(this, "OnRemoveInputClientOwner");
		Controller->OnPawnChangedOwnerClient.AddUFunction(this, "OnPawnChangedClientOwner");
		Controller->OnAddInputMapOwnerClient.AddUFunction(this, "OnAddInputClientOwner");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller null"));
		
	}
}

void ABattleCamera::OnAddInputClientOwner()
{
	UE_LOG(LogTemp, Warning, TEXT("AddInputClientOwner %s "), *Controller->GetPawn()->GetName());
	Target = Controller->GetPawn();
	Controller->SetViewTarget(this);
}

void ABattleCamera::OnPawnChangedClientOwner(APawn* pawn)
{
	if(pawn != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("FALSEEE %s controller %s controllerCharacter %s"), *pawn->GetName(), *Controller->GetName(), *Controller->GetPawn()->GetName());
		
		Target = pawn;
		//Controller->SetCameraToControllerServer(this);
		Controller->SetViewTarget(this);
	}
}

void ABattleCamera::OnRemoveInputClientOwner()
{
	Target = Cast<AGameStateBattle>(GetWorld()->GetGameState())
->TeamTurrets[UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerState<APlayerBattleState>()->Team];
	//Controller->SetCameraToControllerServer(this);
		
	Controller->SetViewTarget(this);
	UE_LOG(LogTemp, Warning, TEXT("camera null %s"), *Controller->GetViewTarget()->GetName());
}
