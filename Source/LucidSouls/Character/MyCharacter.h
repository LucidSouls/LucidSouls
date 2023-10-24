// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class AItem;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class ALucidSoulsGameModeBase;
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

	UPROPERTY(EditDefaultsOnly, Category = Montage)
		TObjectPtr<UAnimMontage> Attacks;

	UPROPERTY(VisibleInstanceOnly)
		TObjectPtr<AItem> ItemOverlap;

	UFUNCTION(BlueprintCallable)
	void ItemCollisionEnable(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
	void ItemCollisionDisable();

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
	void Attack();
	void AttachItem();
	void StartAttacks();
	void SetItemOverlap(AItem* Item) { ItemOverlap = Item; };
	void Transform();
};
