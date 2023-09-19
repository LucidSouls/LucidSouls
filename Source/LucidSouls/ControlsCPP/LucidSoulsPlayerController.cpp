// Fill out your copyright notice in the Description page of Project Settings.

#include "LucidSoulsPlayerController.h"
#include "../PawnsCPP/LucidSoulsPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"

void ALucidSoulsPlayerController::BeginPlay()
{
	Super::BeginPlay();
	this->LucidSoulsPawn = Cast<ALucidSoulsPawn>(GetPawn());

	if (Player)
	{
		if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (!InputMapping.IsNull())
				{
					InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 1);
				}
			}
		}
	}
}

void ALucidSoulsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		if (UEnhancedInputComponent* InputManager = Cast<UEnhancedInputComponent>(InputComponent))
		{
			InputManager->BindAction(IA_W, ETriggerEvent::Triggered, this, &ALucidSoulsPlayerController::ApplyMoveForward);
			InputManager->BindAction(IA_D, ETriggerEvent::Triggered, this, &ALucidSoulsPlayerController::ApplyMoveRight);
			InputManager->BindAction(IA_A, ETriggerEvent::Triggered, this, &ALucidSoulsPlayerController::ApplyMoveLeft);
			InputManager->BindAction(IA_S, ETriggerEvent::Triggered, this, &ALucidSoulsPlayerController::ApplyMoveBack);

			InputManager->BindAction(IA_MOUSE_X, ETriggerEvent::Triggered, this, &ALucidSoulsPlayerController::ApplyMoveMouseX);
			InputManager->BindAction(IA_MOUSE_Y, ETriggerEvent::Triggered, this, &ALucidSoulsPlayerController::ApplyMoveMouseY);
		}
	}
}

void ALucidSoulsPlayerController::TranslateMovementDirection(const FVector& AbsoluteMovementDirection)
{
	if (LucidSoulsPawn)
	{
		LucidSoulsPawn->MoveTowards(AbsoluteMovementDirection);
	}
}

void ALucidSoulsPlayerController::TranslateCameraMovement(const FVector& CameraLookDirection)
{
	if (LucidSoulsPawn)
	{

	}
}

void ALucidSoulsPlayerController::ApplyMoveForward(const FInputActionInstance& Instance)
{
	if (LucidSoulsPawn)
	{
		this->TranslateMovementDirection(FVector(Instance.GetValue().GetMagnitude(), 0.f, 0.f));
	}
}

void ALucidSoulsPlayerController::ApplyMoveRight(const FInputActionInstance& Instance)
{
	if (LucidSoulsPawn)
	{
		this->TranslateMovementDirection(FVector(0.f, Instance.GetValue().GetMagnitude(), 0.f));
	}
}

void ALucidSoulsPlayerController::ApplyMoveLeft(const FInputActionInstance& Instance)
{
	if (LucidSoulsPawn)
	{
		this->TranslateMovementDirection(FVector(0.f, -Instance.GetValue().GetMagnitude(), 0.f));
	}
}

void ALucidSoulsPlayerController::ApplyMoveBack(const FInputActionInstance& Instance)
{
	if (LucidSoulsPawn)
	{
		this->TranslateMovementDirection(FVector(-Instance.GetValue().GetMagnitude(), 0.f, 0.f));
	}
}

void ALucidSoulsPlayerController::ApplyMoveMouseX(const FInputActionInstance& Instance)
{
	if (LucidSoulsPawn)
	{
		this->TranslateCameraMovement(FVector(0.0f, Instance.GetValue().GetMagnitude(), 0.f));
	}
}

void ALucidSoulsPlayerController::ApplyMoveMouseY(const FInputActionInstance& Instance)
{
	if (LucidSoulsPawn)
	{
		this->TranslateCameraMovement(FVector(Instance.GetValue().GetMagnitude(), 0.f, 0.f));
	}
}