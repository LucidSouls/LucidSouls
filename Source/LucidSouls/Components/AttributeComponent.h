// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUCIDSOULS_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ApplyDamage(float Damage);
	float GetHealthStatus();
	bool IsAlive();

private:
	UPROPERTY(EditAnywhere, Category = "Actor Atrributes")
		float Health;
	UPROPERTY(EditAnywhere, Category = "Actor Atrributes")
		float MaxHealth;


};
