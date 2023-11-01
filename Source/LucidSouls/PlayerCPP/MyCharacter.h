// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ItemCPP/Item.h"
#include "../ItemCPP/WeaponCPP/Weapon.h"
#include "MyCharacterStates.h"
#include "GameFramework/Character.h"
#include "MyCoreCharacter.h"
#include "MyCharacter.generated.h"

class UAnimMontage;
class AItem;
class AWeapon;
class UPhysicsHandleComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class LUCIDSOULS_API AMyCharacter : public ACharacter, public IOnHitInterface
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
	
	ECharacterWeaponState CharacterWeaponState;

	//UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//ECharacterActionState CharacterActionState = ECharacterActionState::State_Unocuppied;

	// Locomotion
	float accelerate;

	void MoveForward(float value);
	void TurnXAxis(float value);
	void TurnYAxis(float value);
	void MoveRight(float value);
	void Sprint();
	void Jog();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TObjectPtr<USpringArmComponent> SpringArm;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TObjectPtr<UCameraComponent> ActualCamera;

	// Combat

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UHealthBarWidgetComponent* HPWidget;
	
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* GameOverWidget;

	UFUNCTION(BlueprintCallable)
		void EnableCollisionForCharacterWeapon(ECollisionEnabled::Type CollisionEnabled);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AWeapon* EquippedWeapon;


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

	void OnHitInflictedMontage(const FName& SectionName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAttributeComponent* CharacterAttributes;

	UFUNCTION(BlueprintCallable)
	void EndOfAttackEvent();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	void EquipUnequipMontage(FName SectionName);

	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* PosesMontageOnEquip; // Melee attack animation montage to be play

	UPROPERTY(EditAnywhere)
	AItem* OverlappingItem;




	void SetItemOverlap(AItem* Item);

	void EquipWeapon();
	void MeleeAttack();

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UPROPERTY(VisibleAnywhere)
		bool ItemGrabbing;

	UPROPERTY(BlueprintReadWrite)
		bool PowerUp;
	
	virtual void OnHitInflicted(const FVector& ImpactPoint) override;
	
	UFUNCTION(BLueprintCallable)
	void Die();

	UPROPERTY(BlueprintReadOnly)
		ECharacterDeadPose DeadPose = ECharacterDeadPose::Pose_Alive;

	bool isDead = false;


	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UPhysicsHandleComponent> PhysicsHandle;
	UFUNCTION(BlueprintCallable)
		void GrabItem();

	UFUNCTION(BlueprintCallable)
		void ReleaseItem();

};

//UENUM(BlueprintType)
//enum class ECharacterPDeadPose : uint8 {
//	Pose_Alive UMETA(DisplayName = "Alive"),
//	Pose_Dead1 UMETA(DisplayName = "Dead1")
//	Pose_Dead2 UMETA(DisplayName = "Dead2"),
//	Pose_Dead3 UMETA(DisplayName = "Dead3"),
//	Pose_Dead4 UMETA(DisplayName = "Dead4"),
//	Pose_Dead5 UMETA(DisplayName = "Dead5")
//};
//
//UENUM(BlueprintType)
//enum class ECharacterActionState : uint8 {
//	State_Unocuppied UMETA(DisplayName = "Unoccupied"),
//	State_Atttacking UMETA(DisplayName = "Attacking")
//};
//
//UENUM(BlueprintType)
//enum class ECharacterWeaponState : uint8 {
//	State_WeaponUnequipped UMETA(DisplayName = "Unequipped"),
//	State_WeaponEquipped UMETA(DisplayName = "Equipped"),
//};