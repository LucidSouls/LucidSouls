// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ItemCPP/Item.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UAnimMontage;
class AItem;
UCLASS()
class LUCIDSOULS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Locomotion
	float accelerate;

	void MoveForward(float value);
	void TurnXAxis(float value);
	void TurnYAxis(float value);
	void MoveRight(float value);
	void Sprint();
	void Jog();


	// Combat
	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* MeleeAttackMontage; // Melee attack animation montage to be play


	UPROPERTY(EditAnywhere)
	AItem* OverlappingItem;

	void SetOverlappingItem(AItem* Item);

	void EquipWeapon();
	void MeleeAttack();
	
	


};


