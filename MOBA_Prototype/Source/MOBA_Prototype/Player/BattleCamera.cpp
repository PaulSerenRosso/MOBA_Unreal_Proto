// Fill out your copyright notice in the Description page of Project Settings.
#include "../Player/BattleCamera.h"
#include "Kismet/GameplayStatics.h"

void ABattleCamera::Tick(float DeltaSeconds)
{UE_LOG(LogTemp, Warning, TEXT("Falsefdsqfqdsf"));
	Super::Tick(DeltaSeconds);
	if(Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("TRUEfdsqfqdsf"));
		SetActorLocation(Target->GetActorLocation()+directionFromTarget*distanceFromTarget);
	}

}

void ABattleCamera::BeginPlay()
{
	Super::BeginPlay();
	directionFromTarget.Normalize();
	Controller = Cast<APlayerBattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(Controller != nullptr)
	{
		Controller->SetViewTarget(this);
		Controller->OnPawnChanged.BindUFunction(this, "OnPawnChanged");
	}
	
	

}

void ABattleCamera::OnPawnChanged(APawn* pawn)
{
	if(pawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TRUE"));
		Target = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FALSE"));
		Target = pawn;
		pawn->
	}
}
