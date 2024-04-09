// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers.h"

void UHelpers::LogScreenMessage(const FString& Text, float Duration )
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
						  -1,        // don't over wrire previous message, add a new one
						  Duration,   // Duration of message - limits distance messages scroll onto screen
						  FColor::Cyan,  
						  FString::Printf(TEXT("%s"),*Text)  // Our usual text message format
					 );
	}
}

ETeam UHelpers::GetEnemyTeam(ETeam Team)
{
	switch (Team)
	{
	case ETeam::Team1:
		{
			return ETeam::Team2;
		}
	case ETeam::Team2:
		{
			return ETeam::Team1;
		}
	case ETeam::Neutral:
		{
			return ETeam::Neutral;
		}
	}
	return ETeam::Neutral;
}
