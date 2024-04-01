// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(CurrentRotation);
}

void APlayerCharacter::Move(FVector2D Direction)
{
	AddMovementInput(FVector::RightVector, Direction.X);
	AddMovementInput(FVector::ForwardVector, Direction.Y);
	GetCharacterMovement()->MovementMode = 
}


void APlayerCharacter::RotateServer_Implementation(FVector Direction)
{
	CurrentRotation = Direction.Rotation();
}


void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, CurrentRotation);
}

