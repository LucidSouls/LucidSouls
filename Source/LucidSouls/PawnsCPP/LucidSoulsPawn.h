// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../MovementCPP/LucidSoulsPawnMovementComponent.h"
#include "LucidSoulsPawn.generated.h"

UCLASS()
class LUCIDSOULS_API ALucidSoulsPawn : public APawn
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Sets default values for this pawn's properties
	ALucidSoulsPawn(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere)
		TObjectPtr<UCapsuleComponent> RootCapsule;

	UPROPERTY(EditAnywhere)
		TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
		TObjectPtr<USpringArmComponent> CameraArm;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UCameraComponent> Camera;


	// Health attribute
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pawn Attributes")
		float Health;

	// Stamina attribute
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pawn Attributes")
		float Stamina;

	// Function to decrease health
	UFUNCTION(BlueprintCallable, Category = "Pawn Attributes")
		void DecreaseHealth(float Amount);

	// Function to decrease stamina
	UFUNCTION(BlueprintCallable, Category = "Pawn Attributes")
		void DecreaseStamina(float Amount);

private:
	ULucidSoulsPawnMovementComponent* MovementComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveTowards(FVector AbsoluteMovementDirection);

};
