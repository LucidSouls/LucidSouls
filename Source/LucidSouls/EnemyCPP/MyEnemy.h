// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/OnHitInterface.h"
#include "../PlayerCPP/MyCharacterStates.h"
#include "MyEnemy.generated.h"

class UAnimMontage;
class AAIController;
class UPawnSensingComponent;
class AWeapon;
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

	virtual void OnHitInflicted(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY()
	AAIController* AIController;

	// Combat
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponBat;



	virtual void Destroyed() override;
	void Die();
	void MeleeAttack();

	FTimerHandle EnemyATime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAttributeComponent* CharacterAttributes;

	UPROPERTY(EditAnywhere)
		USoundBase* AttackFleshSFX;

	UPROPERTY(EditAnywhere)
		UParticleSystem* BloodSFX;

	UPROPERTY(EditAnywhere)
		UAnimMontage* MeleeAttackMontage; // Melee attack animation montage to be play

	UPROPERTY(EditAnywhere)
		UAnimMontage* PosesMontageOnHit; // Enemy on hit animation montage to be play

	UPROPERTY(EditAnywhere)
		UAnimMontage* PosesMontageOnDeath;

	UPROPERTY(VisibleAnywhere)
	class UHealthBarWidgetComponent* HPWidget;

	UPROPERTY(BlueprintReadOnly)
	ECharacterDeadPose DeadPose = ECharacterDeadPose::Pose_Alive;

	UPROPERTY()
	AActor* EnemyCTarget;

	bool isDead = false;

	double EnemyPRadius = 200.f;
	double EnemyCRadius = 1000.f;
	double EnemyARadius = 150.f;

	UPROPERTY(EditInstanceOnly)
	AActor* EnemyPTarget;

	FTimerHandle EnemyPTime;
	void EnemyPTimeEvent();

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> EnemyPTargets;

	bool IsInRange(AActor* Target, double Radius);
	

	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void OnSeeingPawn(APawn* Pawn);

	void EnemyMove(AActor* Target);

	UPROPERTY(BlueprintReadWrite)
	EEnemyState EnemyState = EEnemyState::State_UnoccupiedPatrol;

	UFUNCTION(BlueprintCallable)
		void EnableCollisionForCharacterWeapon(ECollisionEnabled::Type CollisionEnabled);

	void OnHitInflictedMontage(const FName& SectionName);
};

//UENUM(BlueprintType)
//enum class ECharacterDeadPose : uint8 {
//	Pose_Alive UMETA(DisplayName = "Alive"),
//	Pose_Dead1 UMETA(DisplayName = "Dead1"),
//	Pose_Dead2 UMETA(DisplayName = "Dead2"),
//	Pose_Dead3 UMETA(DisplayName = "Dead3"),
//	Pose_Dead4 UMETA(DisplayName = "Dead4"),
//	Pose_Dead5 UMETA(DisplayName = "Dead5")
//};
//
//UENUM(BlueprintType)
//enum class EEnemyState : uint8 {
//	State_UnoccupiedPatrol UMETA(DisplayName = "UnoccupiedPatrol"),
//	State_FollowPlayer UMETA(DisplayName = "FollowPlayer"),
//	State_Attacking UMETA(DisplayName = "AttackingPlayer")
//};