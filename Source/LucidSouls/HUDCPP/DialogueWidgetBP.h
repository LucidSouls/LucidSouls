// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidgetBP.generated.h"


/**
*
*/
UCLASS()
class LUCIDSOULS_API UDialogueWidgetBP : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UTextBlock> Text;
};