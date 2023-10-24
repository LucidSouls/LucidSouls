// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include <Components/ProgressBar.h>
#include "HealthWidget.generated.h"
/**
 * 
 */
UCLASS()
class LUCIDSOULS_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	
};
