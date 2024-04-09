// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthWidgetComponent.h"
#include "Interfaces/Healthable.h"
#include "UI/HealthBarWidget.h"

void UHealthWidgetComponent::UpdateUI(ETeam Team)
{
	auto Healthable = Cast<IHealthable>(GetOwner());
	if (Healthable == nullptr) return;

	auto Percent = Healthable->GetPercentageHealth();

	auto SelfWidget = Cast<UHealthBarWidget>(GetUserWidgetObject());
	if (SelfWidget == nullptr) return;

	SelfWidget->UpdateProgressBar(Percent);
	
	FLinearColor Color = FLinearColor::Black;
	
	switch (Team) {
	case ETeam::Neutral:
		Color = FLinearColor::Green;
		break;
	case ETeam::Team1:
		Color = FLinearColor::Blue;
		break;
	case ETeam::Team2:
		Color = FLinearColor::Red;
		break;
	}

	SelfWidget->SetTeamColor(Color);

	
}
