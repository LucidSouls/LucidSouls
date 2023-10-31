// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NPCInteract.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyNPC.generated.h"

class UDialogueWidgetBP;
UCLASS()
class LUCIDSOULS_API AMyNPC : public ACharacter, public INPCInteract
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyNPC();

	UPROPERTY(EditAnywhere)
		TArray<FString> Dialogue;
	UPROPERTY(EditAnywhere)
		TObjectPtr<UDialogueWidgetBP> DialogueWidget;
	UPROPERTY(EditAnywhere)
		int Index;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void Talk() override;

};