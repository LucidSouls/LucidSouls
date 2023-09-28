// Fill out your copyright notice in the Description page of Project Settings.


#include "LucidSoulsPawnMovementComponent.h"

void ULucidSoulsPawnMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

void ULucidSoulsPawnMovementComponent::UpdateMovement(FVector InputVectorParam, float DeltaTime)
{
	if (!InputVectorParam.IsNearlyZero())
	{
		UpdatedComponent->ComponentVelocity += InputVectorParam * 200.0f;
	}
}

void ULucidSoulsPawnMovementComponent::UpdateRotation(FVector Rotation, float DeltaTime)
{
	InputVector = ConsumeInputVector();
	UpdateMovement(InputVector, DeltaTime);
}
