// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidgetComponent.generated.h"

/**
 * 
 */

class UHealthBar;

UCLASS()
class LUCIDSOULS_API UHealthBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void SetHPPercentage(float Percent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthBar* HealthBarWidget;
};
