// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthWidgetComponent.h"

#include "Interfaces/Healthable.h"
#include "UI/HealthBarWidget.h"

void UHealthWidgetComponent::UpdateUI()
{
	auto Healthable = Cast<IHealthable>(GetOwner());
	if (Healthable == nullptr) return;

	auto Percent = Healthable->GetPercentageHealth();

	auto SelfWidget = Cast<UHealthBarWidget>(GetUserWidgetObject());
	if (SelfWidget == nullptr) return;

	SelfWidget->UpdateProgressBar(Percent);
}
