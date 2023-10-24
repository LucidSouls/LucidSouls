// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarComponent.h"
#include "LucidSouls/HUD/HealthWidget.h"
#include "Components/ProgressBar.h"



void UHealthBarComponent::UpdateHealth(float Percentage)
{
    //Check if Healthwidget is valid, if not, assign its value by the casting the user widget  
    HealthWidget = (HealthWidget == nullptr) ? Cast<UHealthWidget>(GetUserWidgetObject()) : HealthWidget;
    
    //Check if healthwidget is valid and HealthBar component of healthwidget is valid
    if (HealthWidget && HealthWidget->HealthBar)
    {
        //Update the health bar
        HealthWidget->HealthBar->SetPercent(Percentage);
    }
}



//void UHealthBarComponent::SetHealthPercent(float Percent)
//{
//	if (HealthWidget == nullptr) 
//	{
//		HealthWidget = Cast<UHealthWidget>(GetUserWidgetObject());
//	}
//	if (HealthWidget && HealthWidget->HealthBar)
//	{
//		HealthWidget->HealthBar->SetPercent(Percent);
//	}
//}
