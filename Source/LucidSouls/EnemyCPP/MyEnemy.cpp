// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "../ItemCPP/WeaponCPP/Weapon.h"
#include "../ComponentCPP/AttributeComponent.h"
#include "../HUDCPP/HealthBarWidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"



// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterAttributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("CharacterAttributes"));

	// Set Enemy capsule mesh to ignore camera channel so collision won't be messy as camera is bugging
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetGenerateOverlapEvents(true);

	// Set Enemy skeleton mesh to be world dynamic type as weapon collision is set to ignore pawn type
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	// Set Enemy skeleton mesh to ignore camera channel so collision won't be messy as camera is bugging
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// Set Enemy skeleton mesh to block visibility channel so hitbox can collide with Enemy mesh 
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	
	HPWidget = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HP"));
	HPWidget->SetupAttachment(GetRootComponent());
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	//PawnSensing->SightRadius = 4000.f;
	//PawnSensing->SetPeripheralVisionAngle(45.f);

}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{

	Super::BeginPlay();

	isDead = false;
	if (HPWidget) {
		HPWidget->SetVisibility(false);
	}

	Tags.Add(FName("Enemy"));

	// Ensure AIController is set to an AAIController
	AIController = Cast<AAIController>(GetController());

	// Move the enemy towards the target
	if (AIController && EnemyPTarget)
	{
		EnemyMove(EnemyPTarget);
	}

	// Configure PawnSensingComponent to trigger OnSeeingPawn
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AMyEnemy::OnSeeingPawn);
	}

	// Spawn and equip an enemy weapon
	if (WeaponBat)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			AWeapon* EnemyWeapon = World->SpawnActor<AWeapon>(WeaponBat);
			if (EnemyWeapon)
			{
				// Attach the weapon to the enemy's mesh
				EnemyWeapon->AttachToCharacter(GetMesh(), FName("RightHandSocket"), this, this);
				EquippedWeapon = EnemyWeapon;
			}
		}
	}

}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState > EEnemyState::State_UnoccupiedPatrol) {
		if (EnemyCTarget) {
			const double DistanceToTarget = (EnemyCTarget->GetActorLocation() - GetActorLocation()).Size();
			if (!IsInRange(EnemyCTarget, EnemyCRadius)) {
				EnemyState = EEnemyState::State_UnoccupiedPatrol;

				GetWorldTimerManager().ClearTimer(EnemyATime);
				EnemyCTarget = nullptr;
				if (HPWidget) {
					HPWidget->SetVisibility(false);
				}
				EnemyMove(EnemyPTarget);
				GetCharacterMovement()->MaxWalkSpeed = 125.f;

				UE_LOG(LogTemp, Warning, TEXT("Luchador lost interest. NOT CHASING"));
			}
			else if (!IsInRange(EnemyCTarget, EnemyARadius) && EnemyState != EEnemyState::State_FollowPlayer) {
				EnemyState = EEnemyState::State_FollowPlayer;
				GetWorldTimerManager().ClearTimer(EnemyATime);
				EnemyMove(EnemyCTarget);
				GetCharacterMovement()->MaxWalkSpeed = 300.f;
				UE_LOG(LogTemp, Warning, TEXT("Luchador chasing"));
			}
			else if (IsInRange(EnemyCTarget, EnemyARadius) && EnemyState != EEnemyState::State_Attacking ) {
				if (isDead == false) {
					EnemyState = EEnemyState::State_Attacking;
					GetWorldTimerManager().ClearTimer(EnemyATime);				
					GetWorldTimerManager().SetTimer(EnemyATime, this, &AMyEnemy::MeleeAttack, 0.5f);
					UE_LOG(LogTemp, Warning, TEXT("Luchador attacking"));

				}
				
			}
		}
	}
	else {
		// Need refactor
		if (EnemyPTarget) {
			if (IsInRange(EnemyPTarget, EnemyPRadius)) {

				TArray<AActor*> EnemyValidPTargets;
				for (auto Target : EnemyPTargets) {
					if (Target != EnemyPTarget) {
						EnemyValidPTargets.AddUnique(Target);
					}
				}

				const int32 EnemyPTargetsTotal = EnemyPTargets.Num();
				if (EnemyPTargetsTotal > 0) {
					const int32 PTargetSelectedIndex = FMath::RandRange(0, EnemyPTargetsTotal - 1);

					if (PTargetSelectedIndex >= 0 && PTargetSelectedIndex < EnemyValidPTargets.Num()) {
						AActor* Target = EnemyValidPTargets[PTargetSelectedIndex];
						EnemyPTarget = Target;

						GetWorldTimerManager().SetTimer(EnemyPTime, this, &AMyEnemy::EnemyPTimeEvent, 3.f);
					}

				}

			}
		}
	}
	

	
}

// Called to bind functionality to input
void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyEnemy::OnHitInflicted(const FVector& ImpactPoint)
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

float AMyEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	EnemyState = EEnemyState::State_FollowPlayer;

	if (CharacterAttributes) {
		CharacterAttributes->InflictDamage(DamageAmount);
		
		if (HPWidget) {
			HPWidget->SetHPPercentage(CharacterAttributes->HPPercentage());
		}
	
	}

	EnemyCTarget = EventInstigator->GetPawn();
	
	EnemyMove(EnemyCTarget);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	return DamageAmount;

	//return 0.0f;
}

void AMyEnemy::Destroyed()
{
	if (EquippedWeapon) {
		EquippedWeapon->Destroy();
	}
}

void AMyEnemy::Die()
{

	isDead = true;
	// Get the character's animation instance
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();


	// Check if we have a valid animation instance and a valid on death montage
	if (AnimInstance && PosesMontageOnDeath) {

		AnimInstance->Montage_Play(PosesMontageOnDeath);

		FName SectionName = FName();

		int32 Section = FMath::RandRange(0, 4);

		if (Section == 0) {
			SectionName = FName("Dead1");
			DeadPose = ECharacterDeadPose::Pose_Dead1;
		}
		else if (Section == 1) {
			SectionName = FName("Dead2");
			DeadPose = ECharacterDeadPose::Pose_Dead2;
		}
		else if (Section == 2) {
			SectionName = FName("Dead3");
			DeadPose = ECharacterDeadPose::Pose_Dead3;
		}
		else if (Section == 3) {
			SectionName = FName("Dead4");
			DeadPose = ECharacterDeadPose::Pose_Dead4;
		}
		else if (Section == 4) {
			SectionName = FName("Dead5");
			DeadPose = ECharacterDeadPose::Pose_Dead5;
		}

		AnimInstance->Montage_JumpToSection(SectionName, PosesMontageOnDeath);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (HPWidget) {
		HPWidget->SetVisibility(false);
	}
	SetLifeSpan(3.f);

	// Display a debug message if the GEngine is available
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, "Enemy is dead");
	}
}


void AMyEnemy::MeleeAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// Check if we have a valid animation instance and a valid melee attack montage
	if (AnimInstance && MeleeAttackMontage) {

		AnimInstance->Montage_Play(MeleeAttackMontage);

		FName SectionName = FName();

		int32 Section = FMath::RandRange(0, 2);

		if (Section == 0) {
			SectionName = FName("Melee1");
		}
		else if (Section == 1) {
			SectionName = FName("Melee2");
		}
		else if (Section == 2) {
			SectionName = FName("Melee3");
		}

		AnimInstance->Montage_JumpToSection(SectionName, MeleeAttackMontage);
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, "Enemy striked");
		}
	}
}

void AMyEnemy::EnemyPTimeEvent()
{
	EnemyMove(EnemyPTarget);
}

bool AMyEnemy::IsInRange(AActor* Target, double Radius)
{

	if (Target == nullptr)
	{
		return false;
	}

	const FVector TargetLocation = Target->GetActorLocation();
	const FVector ActorLocation = GetActorLocation();
	const double TargetDistance = (TargetLocation - ActorLocation).Size();

	return TargetDistance <= Radius;
}

void AMyEnemy::EnemyMove(AActor* Target)
{

	if (AIController != nullptr && Target != nullptr)
	{
		FAIMoveRequest MoveEnemy;
		MoveEnemy.SetGoalActor(Target);
		MoveEnemy.SetAcceptanceRadius(50.f);
		AIController->MoveTo(MoveEnemy);
	}
}

void AMyEnemy::EnableCollisionForCharacterWeapon(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->WHitBox) {
		EquippedWeapon->WHitBox->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->ActorsToAvoid.Empty();
	}
}

void AMyEnemy::OnHitInflictedMontage(const FName& SectionName)
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

void AMyEnemy::OnSeeingPawn(APawn* SeenPawn)
{


	if (EnemyState != EEnemyState::State_FollowPlayer && SeenPawn->ActorHasTag(FName("Player")))
	{
		GetWorldTimerManager().ClearTimer(EnemyPTime);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		EnemyCTarget = SeenPawn;

		if (EnemyState != EEnemyState::State_Attacking)
		{
			EnemyState = EEnemyState::State_FollowPlayer;
			EnemyMove(EnemyCTarget);
		}
		UE_LOG(LogTemp, Warning, TEXT("Seeing pawn"));
	}
}

