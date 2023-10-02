// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
UCLASS()
class LUCIDSOULS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UCameraComponent> ActualCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveTowards(float Value);
	void MoveSide(float Value);
	void LookUpAndDown(float Value);
	void LookSide(float Value);

};
