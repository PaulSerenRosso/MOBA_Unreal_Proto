// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers.h"

void UHelpers::LogScreenMessage(const FString& Text, float Duration )
{
	UE_LOG(LogTemp, Warning, TEXT("Launch Begin Player1"));
	if (GEngine)
	{UE_LOG(LogTemp, Warning, TEXT("Launch Begin Player2"));
		GEngine->AddOnScreenDebugMessage(
						  -1,        // don't over wrire previous message, add a new one
						  Duration,   // Duration of message - limits distance messages scroll onto screen
						  FColor::Cyan,  
						  FString::Printf(TEXT("%s"),*Text)  // Our usual text message format
					 );
	}
}