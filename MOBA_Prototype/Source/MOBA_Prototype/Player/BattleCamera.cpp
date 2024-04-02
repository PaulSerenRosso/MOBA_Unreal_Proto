// Fill out your copyright notice in the Description page of Project Settings.
#include "../Player/BattleCamera.h"
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
		Controller->OnPawnChangedOwnerClient.BindUFunction(this, "OnPawnChangedClientOwner");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller null"));
	}
	
}

void ABattleCamera::OnPawnChangedClientOwner(APawn* pawn)
{
	if(pawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TRUE"));
		Target = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FALSEEE %s controller %s controllerCharacter %s"), *pawn->GetName(), *Controller->GetName(), *Controller->GetPawn()->GetName());
		
		Target = pawn;
		Controller->SetCameraToControllerServer(this);
		
	}
}
