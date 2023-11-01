// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "../ItemCPP/Item.h"
#include "../ItemCPP/WeaponCPP/Weapon.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../ComponentCPP/AttributeComponent.h"
#include "../HUDCPP/HealthBarWidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	/*PowerUp = false;*/

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	SpringArm->TargetArmLength = 500.f;
	SpringArm->bUsePawnControlRotation = true;
	ActualCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	ActualCamera->SetupAttachment(SpringArm);

	CharacterAttributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("CharacterAttributes"));

	accelerate = 1.0f;
	CharacterWeaponState = ECharacterWeaponState::State_WeaponUnequipped;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

	// Set Character skeleton mesh to be world dynamic type as weapon collision is set to ignore pawn type
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// Set Character skeleton mesh to block visibility channel so hitbox can collide with Character mesh 
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	HPWidget = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBar"));
	HPWidget->SetupAttachment(GetRootComponent());

	GameOverWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("GameOver"));
	GameOverWidget->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HPWidget) {
		HPWidget->SetVisibility(true);
	}
	if (GameOverWidget) {
		GameOverWidget->SetVisibility(false);
	}
	Tags.Add(FName("Player"));
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ItemGrabbing)
	{
		//Update item location when being dragged around
		FVector Start = ActualCamera->GetComponentLocation();
		FVector End = (ActualCamera->GetComponentRotation()).Vector() * 2000.0f + Start;
		PhysicsHandle->SetTargetLocation(End);
	}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Binding the movement functions to axis mapping inputs


	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &AMyCharacter::Jump);

	// Not working
	PlayerInputComponent->BindAction(FName("Sprint"), IE_Pressed, this, &AMyCharacter::Sprint);
	PlayerInputComponent->BindAction(FName("Jog"), IE_Released, this, &AMyCharacter::Jog);

	PlayerInputComponent->BindAction(FName("EquipWeapon"), IE_Pressed, this, &AMyCharacter::EquipWeapon);
	PlayerInputComponent->BindAction(FName("MeleeAttack"), IE_Pressed, this, &AMyCharacter::MeleeAttack);

	PlayerInputComponent->BindAction(FName("Move"), IE_Pressed, this, &AMyCharacter::GrabItem);
	PlayerInputComponent->BindAction(FName("Move"), IE_Released, this, &AMyCharacter::ReleaseItem);

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


void AMyCharacter::SetItemOverlap(AItem* Item)
{
	OverlappingItem = Item;
}

void AMyCharacter::GrabItem()
{
	if (PowerUp) {
		//result
		FHitResult HitResult;

		// start location is at player camera (will be converted to fps) and end location 
		// end location is 2000cm straight from start
		FVector Start = ActualCamera->GetComponentLocation();
		FVector End = (ActualCamera->GetComponentRotation()).Vector() * 2000.0f + Start;

		// Add player to ignoreed actors for collision query param
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		//Create a line trace and returns the first hit actor which belongs to the specified collision channel (ECC_Visibility)
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

		// if line tracing hit something, bBlockingHit from the hit result will be true
		if (HitResult.bBlockingHit)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Cyan, *HitResult.GetActor()->GetName());
			}
			HitResult.GetComponent()->SetSimulatePhysics(true);

			//Grab component
			PhysicsHandle->GrabComponentAtLocationWithRotation(HitResult.GetComponent(), NAME_None, HitResult.Location,
				HitResult.GetComponent()->GetComponentRotation());

			ItemGrabbing = true;
		}
		else {
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, TEXT("0 hit actor"));
			}
		}
	}
}

void AMyCharacter::ReleaseItem()
{
	if (PowerUp) {
		if (ItemGrabbing) {
			//Release component
			PhysicsHandle->ReleaseComponent();
			ItemGrabbing = false;
		}
	}
}

void AMyCharacter::EquipWeapon()
{
	// Attempt to cast the overlapping item to a weapon
	AWeapon* EquippingWeapon = Cast<AWeapon>(OverlappingItem);

	// Check if there is an overlapping item
	if (OverlappingItem) {
		//if (CharacterActionState == ECharacterActionState::State_Unocuppied) {
			if (CharacterWeaponState == ECharacterWeaponState::State_WeaponUnequipped) {
				// Check if the character's weapon is in the unequipped state.

				// Equip the weapon
				if (EquippingWeapon) {
					// If there is a weapon to equip, attach it to the character's mesh at a specific socket.
					EquippingWeapon->AttachToCharacter(GetMesh(), FName("SwordSocket"), this, this);

					/*
					EquippingWeapon->SetOwner(this);
					EquippingWeapon->SetInstigator(this);
					*/

					// Update the character's weapon state to "equipped."
					CharacterWeaponState = ECharacterWeaponState::State_WeaponEquipped;

					// Store a reference to the equipped weapon.
					EquippedWeapon = EquippingWeapon;

					// Output a message to the on-screen debug console if GEngine is available.
					if (GEngine) {
						GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, "Equipped");
					}
				}
			}
			else if (CharacterWeaponState == ECharacterWeaponState::State_WeaponEquipped) {
				// Check if the character's weapon is in the equipped state.

				// Update the character's weapon state to "unequipped."
				CharacterWeaponState = ECharacterWeaponState::State_WeaponUnequipped;

				//EquippedWeapon = nullptr;

				// Play an unequip montage, if available.
				EquipUnequipMontage(FName("Unequip"));

				// Output a message to the on-screen debug console if GEngine is available.
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, "Unequipped");
				}
			}
		//}

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(2, 30.0f, FColor::Cyan, FString::Printf(TEXT("CharacterWeaponState: %d"), CharacterWeaponState));
		}
	}
}


void AMyCharacter::MeleeAttack()
{
	// BUG HERE
	if (CharacterWeaponState != ECharacterWeaponState::State_WeaponUnequipped)
	{
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

			FName SectionName = FName();

			int32 Section = FMath::RandRange(0, 1);

			if (Section == 0) {
				SectionName = FName("Melee1");
			}
			else if (Section == 1) {
				SectionName = FName("Melee2");
			}

			AnimInstance->Montage_JumpToSection(SectionName, MeleeAttackMontage);
		}

		//CharacterActionState = ECharacterActionState::State_Atttacking;

		// Display a debug message if the GEngine is available
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, "Melee Attack");
		}
	}

	// Get the character's animation instance
	
}

void AMyCharacter::EndOfAttackEvent()
{
	//CharacterActionState = ECharacterActionState::State_Unocuppied;
}



float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CharacterAttributes) {
		CharacterAttributes->InflictDamage(DamageAmount);

		if (HPWidget) {
			HPWidget->SetHPPercentage(CharacterAttributes->HPPercentage());
		}

	}
	return DamageAmount;
}

void AMyCharacter::EnableCollisionForCharacterWeapon(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->WHitBox) {
		EquippedWeapon->WHitBox->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->ActorsToAvoid.Empty();
	}
}

void AMyCharacter::OnHitInflicted(const FVector& ImpactPoint)
{

	if (HPWidget) {
		HPWidget->SetVisibility(true);
	}

	if (CharacterAttributes && CharacterAttributes->IsAlive()) {

		// Get the enemy's forward vector (already normalised).
		const FVector ActorForward = GetActorForwardVector();

		// Create a vector at the same Z-level as the enemy, pointing from the enemy's location to the impact point.
		const FVector ImpactAtActorZLocation(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);

		// Calculate a normalized vector pointing from the enemy's location to the impact point.
		const FVector HitVector = (ImpactAtActorZLocation - GetActorLocation()).GetSafeNormal();

		// Calculate the cosine of the angle between the enemy's forward vector and the hit vector.
		const double CosHitAngle = FVector::DotProduct(ActorForward, HitVector);

		// Convert the angle to degrees.
		double HitAngle = FMath::Acos(CosHitAngle);
		HitAngle = FMath::RadiansToDegrees(HitAngle);

		// Calculate the cross product of the forward vector and the hit vector.
		const FVector CrossProduct = FVector::CrossProduct(ActorForward, HitVector);

		// Determine the section based on the calculated angle and value of cross product vector
		FName Section = (HitAngle >= 0.f && HitAngle < 45.f) ? FName("Front") :
			(HitAngle >= 45.f && HitAngle < 135.f && CrossProduct.Z > 0) ? FName("Right") :
			(HitAngle >= 45.f && HitAngle < 135.f && CrossProduct.Z < 0) ? FName("Left") :
			FName("Back");

		// Play on hit montage accordingly
		OnHitInflictedMontage(Section);

		// Convert the section name to a string.
		FString direction = Section.ToString();

		// Create a debug message indicating the hit angle and direction.
		FString TestLog = FString::Printf(TEXT("Hit inflicted at %f degrees from "), HitAngle);
		TestLog = TestLog + direction;

		// Display the debug message on the screen.
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, TestLog);
		}
	}

	else {
		Die();
	}

	if (AttackFleshSFX && BloodSFX && GetWorld()) {
		UGameplayStatics::PlaySoundAtLocation(this, AttackFleshSFX, ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodSFX, ImpactPoint);
	}

}

void AMyCharacter::Die()
{
	isDead = true;
	// Get the character's animation instance
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();


	// Check if we have a valid animation instance and a valid on death montage
	if (AnimInstance && PosesMontageOnDeath) {

		AnimInstance->Montage_Play(PosesMontageOnDeath);

		FName SectionName = FName("Dead1");
		DeadPose = ECharacterDeadPose::Pose_Dead1;

		AnimInstance->Montage_JumpToSection(SectionName, PosesMontageOnDeath);

		if (HPWidget) {
			HPWidget->SetVisibility(false);
		}

		if (GameOverWidget) {
			FTimerHandle TimerHandleGameOver;
			GetWorldTimerManager().SetTimer(TimerHandleGameOver, [this]() {
				GameOverWidget->SetVisibility(true);
				}, 3.0f, false);
		}
	}

}

void AMyCharacter::OnHitInflictedMontage(const FName& SectionName)
{
	// Get the character's animation instance
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// Check if we have a valid animation instance and a valid on hit montage
	if (AnimInstance && PosesMontageOnHit) {

		AnimInstance->Montage_Play(PosesMontageOnHit);
		UE_LOG(LogTemp, Warning, TEXT("Montage played!"));

		AnimInstance->Montage_JumpToSection(SectionName, PosesMontageOnHit);
	}

	// Display a debug message if the GEngine is available
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, "Enemy is hit");
	}
}

void AMyCharacter::EquipUnequipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && PosesMontageOnEquip) {
		AnimInstance->Montage_Play(PosesMontageOnEquip);
		AnimInstance->Montage_JumpToSection(SectionName, PosesMontageOnEquip);
	}
}

void AMyCharacter::Disarm()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToPlayerSocket(GetMesh(), FName("HideSwordSocket"));
	}
}

void AMyCharacter::Arm()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToPlayerSocket(GetMesh(), FName("SwordSocket"));
	}
}

