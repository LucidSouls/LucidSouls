// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

<<<<<<< Updated upstream
=======
	if (EnemyState > EEnemyState::State_UnoccupiedPatrol) {
		if (EnemyCTarget) {
			const double DistanceToTarget = (EnemyCTarget->GetActorLocation() - GetActorLocation()).Size();
			
			// 1st case when combat target is outside of combat radius
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

			// 2nd case when combat target is inside of combat radius but outside of attack radius
			else if (!IsInRange(EnemyCTarget, EnemyARadius) && EnemyState != EEnemyState::State_FollowPlayer) {
				EnemyState = EEnemyState::State_FollowPlayer;
				GetWorldTimerManager().ClearTimer(EnemyATime);
				EnemyMove(EnemyCTarget);
				GetCharacterMovement()->MaxWalkSpeed = 300.f;
				UE_LOG(LogTemp, Warning, TEXT("Luchador chasing"));
			}

			// 3rd case when combat target is inside of attack radius
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

		// Check if there is a valid primary target
		if (EnemyPTarget && IsInRange(EnemyPTarget, EnemyPRadius)) {

			// Create an array to store valid secondary targets excluding the previously used target
			TArray<AActor*> EnemyValidPTargets;
			for (auto Target : EnemyPTargets) {
				if (Target != EnemyPTarget) {
					EnemyValidPTargets.AddUnique(Target);
				}
			}

			// If enemy valid target array of next targets has targets in it
			if (EnemyValidPTargets.Num() > 0) {

				// Select a random target
				const int32 EnemyPTargetIndex = FMath::RandRange(0, EnemyValidPTargets.Num() - 1);
				AActor* Target = EnemyValidPTargets[EnemyPTargetIndex];

				// Set the next target to the randomly selected secondary target
				EnemyPTarget = Target;

				// Set a timer to move the enemy to next target after 3 seconds
				GetWorldTimerManager().SetTimer(EnemyPTime, this, &AMyEnemy::EnemyPTimeEvent, 3.f);

			}

		}

	}
	

	
>>>>>>> Stashed changes
}

// Called to bind functionality to input
void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

<<<<<<< Updated upstream
=======
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

>>>>>>> Stashed changes
