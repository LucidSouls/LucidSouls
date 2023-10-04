// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetGenerateOverlapEvents(true);

	// Set Enemy skeleton mesh to be world dynamic type as weapon collision is set to ignore pawn type
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	// Set Enemy skeleton mesh to block visibility channel so hitbox can collide with Enemy mesh 
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	// Set Enemy skeleton mesh to ignore camera channel so collision won't be messy as camera is bugging
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// Set Enemy capsule mesh to ignore camera channel so collision won't be messy as camera is bugging
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


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

}

// Called to bind functionality to input
void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyEnemy::GetHit(const FVector& ImpactPoint)
{

}

void AMyEnemy::OnHit()
{
	// Get the character's animation instance
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// Check if we have a valid animation instance and a valid on hit montage
	if (AnimInstance && OnHitMontage) {

		AnimInstance->Montage_Play(OnHitMontage);

		AnimInstance->Montage_JumpToSection(FName(""), OnHitMontage);
	}

	// Display a debug message if the GEngine is available
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, "Melee Attack");
	}
}

