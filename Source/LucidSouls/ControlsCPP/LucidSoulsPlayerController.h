// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "LucidSoulsPlayerController.generated.h"

/**
 *
 */
UCLASS()
class LUCIDSOULS_API ALucidSoulsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "Input")
		TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
		class UInputAction* IA_W;

	UPROPERTY(EditAnywhere, Category = "Input")
		class UInputAction* IA_A;

	UPROPERTY(EditAnywhere, Category = "Input")
		class UInputAction* IA_S;

	UPROPERTY(EditAnywhere, Category = "Input")
		class UInputAction* IA_D;

	UPROPERTY(EDITAnywhere, Category = "Input")
		class UInputAction* IA_MOUSE_X;

	UPROPERTY(EDITAnywhere, Category = "Input")
		class UInputAction* IA_MOUSE_Y;

protected:
	class ALucidSoulsPawn* LucidSoulsPawn;

	void TranslateMovementDirection(const FVector& AbsoluteMovementDirection);
	void TranslateCameraMovement(const FVector& CameraLookDirection);

	void ApplyMoveForward(const FInputActionInstance&);
	void ApplyMoveRight(const FInputActionInstance&);
	void ApplyMoveLeft(const FInputActionInstance&);
	void ApplyMoveBack(const FInputActionInstance&);
	void ApplyMoveMouseX(const FInputActionInstance&);
	void ApplyMoveMouseY(const FInputActionInstance&);

private:
};
