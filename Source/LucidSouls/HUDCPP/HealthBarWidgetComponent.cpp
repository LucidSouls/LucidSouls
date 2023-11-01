// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidgetComponent.h"
#include "../HUDCPP/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarWidgetComponent::SetHPPercentage(float Percent)
{
	HealthBarWidget = (HealthBarWidget == nullptr) ? Cast<UHealthBar>(GetUserWidgetObject()) : HealthBarWidget;

	if (HealthBarWidget && HealthBarWidget->HealthBar) {
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
