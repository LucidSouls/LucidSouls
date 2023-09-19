// Fill out your copyright notice in the Description page of Project Settings.


#include "LucidSoulsMovementComponent.h"

void ULucidSoulsMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	InputVector = ConsumeInputVector();
	UpdateMovement(InputVector, DeltaTime);
}

void ULucidSoulsMovementComponent::UpdateMovement(FVector InputVectorParam, float DeltaTime)
{
	if (!InputVectorParam.IsNearlyZero())
	{
		UpdatedComponent->ComponentVelocity += InputVectorParam*200.0f;
	}
}

void ULucidSoulsMovementComponent::UpdateRotation(FVector Rotation, float DeltaTime)
{
}