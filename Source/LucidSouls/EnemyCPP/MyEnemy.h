// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/OnHitInterface.h"
#include "MyEnemy.generated.h"

class UAnimMontage;


UCLASS()
class LUCIDSOULS_API AMyEnemy : public ACharacter, public IOnHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& ImpactPoint) override;

	// Combat
	UPROPERTY(EditAnywhere, Category = Montages)
		UAnimMontage* OnHitMontage; // Enemy on hit animation montage to be play

	void OnHit();
};
