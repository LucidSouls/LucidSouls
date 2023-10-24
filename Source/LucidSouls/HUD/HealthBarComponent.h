// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
class UHealthWidget;
UCLASS()
class LUCIDSOULS_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void UpdateHealth(float Percentage);
private:
	UPROPERTY()
	TObjectPtr<UHealthWidget> HealthWidget;
};
