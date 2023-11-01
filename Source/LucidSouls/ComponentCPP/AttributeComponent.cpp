// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributeComponent::InflictDamage(float Dmg)
{
	
	//CurHP = FMath::Clamp(CurHP - Dmg, 0.f, MaxHP);

	CurHP = FMath::Max(0.f, CurHP - Dmg);
	CurHP = FMath::Min(CurHP, MaxHP);

}

float UAttributeComponent::HPPercentage()
{
	return CurHP/MaxHP;
	//return 0.0f;
}

bool UAttributeComponent::IsAlive()
{
	return CurHP > 0.f;
}

