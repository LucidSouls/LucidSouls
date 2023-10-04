// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "../ItemCPP/Item.h"
#include "../ItemCPP/WeaponCPP/Weapon.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	accelerate = 1.0f;


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Binding the movement functions to axis mapping inputs


	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &AMyCharacter::Jump);
	//PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AMyCharacter::Equip);

	// Not working
	PlayerInputComponent->BindAction(FName("Sprint"), IE_Pressed, this, &AMyCharacter::Sprint);
	PlayerInputComponent->BindAction(FName("Jog"), IE_Released, this, &AMyCharacter::Jog);

	PlayerInputComponent->BindAction(FName("EquipWeapon"), IE_Pressed, this, &AMyCharacter::EquipWeapon);
	PlayerInputComponent->BindAction(FName("MeleeAttack"), IE_Pressed, this, &AMyCharacter::MeleeAttack);

	PlayerInputComponent->BindAxis(FName("TurnXAxis"), this, &AMyCharacter::TurnXAxis);
	PlayerInputComponent->BindAxis(FName("TurnYAxis"), this, &AMyCharacter::TurnYAxis);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AMyCharacter::MoveRight);


}

void AMyCharacter::TurnYAxis(float value)
{
	AddControllerPitchInput(value);
}


void AMyCharacter::TurnXAxis(float value)
{
	AddControllerYawInput(value);
}


void AMyCharacter::MoveForward(float value)
{
	if (Controller && (value != 0.f))
	{
		// Get the controller's rotation
		const FRotator ControllerRotation = GetControlRotation();

		// Calculate the forward vector based on the controller's rotation (yaw)
		const FVector ForwardVector = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);

		float valueWithAcc = value * accelerate;

		// Add movement input in the forward direction
		AddMovementInput(ForwardVector, value);
		//if (GEngine)
		//{
		//	FString TestLog = FString::Printf(TEXT("ValueWithAcc: %f"), valueWithAcc);
		//	GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, TestLog);
		//}

	}
}


void AMyCharacter::MoveRight(float value)
{
	if (Controller && (value != 0.f))
	{
		// Get the controller's rotation
		const FRotator ControllerRotation = GetControlRotation();

		// Calculate the forward vector based on the controller's rotation (yaw)
		const FVector ForwardVector = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);

		// Calculate the right vector by taking the cross product of the forward vector and the up vector (WorldUpVector)
		const FVector RightVector = FVector::CrossProduct(ForwardVector, FVector::UpVector);

		float valueWithAcc = - value * accelerate;

		// Add movement input in the right direction
		AddMovementInput(RightVector, -value);
		
		//if (GEngine)
		//{
		//	FString TestLog = FString::Printf(TEXT("ValueWithAcc: %f"), valueWithAcc);
		//	GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan,TestLog);
		//}
	}
}

void AMyCharacter::Sprint()
{
	accelerate = 4.0f;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Green, "Sprinting");
	}
}

void AMyCharacter::Jog()
{
	accelerate = 1.0f;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, "Jogging");
	}
}

//void AMyCharacter::SetOverlappingItem(AItem* Item)
//{
//	OverlappingItem = Item;
//}

void AMyCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AMyCharacter::EquipWeapon()
{
	// Check if there is an overlapping item
	if (OverlappingItem) {
		// Attempt to cast the overlapping item to a weapon
		AWeapon* EquippingWeapon = Cast<AWeapon>(OverlappingItem);

		// If the cast is successful and we have a valid weapon, attach it to the character's mesh
		if (EquippingWeapon) {
			EquippingWeapon->AttachToCharacter(GetMesh(), FName("SwordSocket"));
		}
	}
}

void AMyCharacter::MeleeAttack()
{
	// Get the character's animation instance
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// Check if we have a valid animation instance and a valid melee attack montage
	if (AnimInstance && MeleeAttackMontage) {
		
		// Delay before playing the melee attack montage
		//const float DelayBeforeAttack = 0.5f;
		//FTimerHandle UnusedHandle;
		//GetWorld()->GetTimerManager().SetTimer(UnusedHandle, [this, AnimInstance]() {
		//	// Play the melee attack montage
		//	AnimInstance->Montage_Play(MeleeAttackMontage);

		//	// Jump to the specified section within the montage
		//	AnimInstance->Montage_JumpToSection(FName("Attack1"), MeleeAttackMontage);
		//	}, DelayBeforeAttack, false);

		AnimInstance->Montage_Play(MeleeAttackMontage);

		AnimInstance->Montage_JumpToSection(FName("Melee1"), MeleeAttackMontage);
	}

	// Display a debug message if the GEngine is available
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, "Melee Attack");
	}
}

