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

private:
	ULucidSoulsPawnMovementComponent* MovementComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveTowards(FVector AbsoluteMovementDirection);
	void Sprint();
};
