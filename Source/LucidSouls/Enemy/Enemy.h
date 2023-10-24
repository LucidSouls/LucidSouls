// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LucidSouls/Enum/MyEnum.h"
#include "LucidSouls/Interface/OnHitInterface.h"
#include "Enemy.generated.h"


class UAttributeComponent;
class UAnimMontage;
class UHealthBarComponent;
UCLASS()
class LUCIDSOULS_API AEnemy : public ACharacter, public IOnHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montage)
	TObjectPtr<UAnimMontage> DeathMontage;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> ActorAttributes;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBar;

	UPROPERTY(BlueprintReadOnly)
	EDeath Death = EDeath::EDP_Alive;
	void PlayHitMontage(const FName& Section);
	void PlayDeathMontage();
};
