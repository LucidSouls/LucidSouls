// Fill out your copyright notice in the Description page of Project Settings.
// This needs to inherit from PawnMovementComponent NOT CHARACTERMOVEMENTCOMPONENT
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LucidSoulsMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class LUCIDSOULS_API ULucidSoulsMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

protected:
	float JumpHeight;
	float Stamina;
	float WalkSpeed;
	float RunSpeed;

	FVector InputVector;
	FVector InputRotation;

	void UpdateMovement(FVector InputVectorParam, float DeltaTime);
	void UpdateRotation(FVector Rotation, float DeltaTime);

private:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
