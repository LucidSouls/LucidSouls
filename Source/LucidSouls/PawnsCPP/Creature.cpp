// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
// Sets default values
ACreature::ACreature()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	/*Capsule->SetCapsuleHalfHeight(70.f);
	Capsule->SetCapsuleRadius(75.f);*/
	SetRootComponent(Capsule);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Capsule);
	/*SpringArm->TargetArmLength = 600.f;*/
	CreatureMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CreatureMesh"));
	CreatureMesh->SetupAttachment(Capsule);
	ActualCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	ActualCamera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void ACreature::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACreature::MoveTowards(float Value) 
{
	if ((Value != 0.f) && Controller != NULL) {
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ACreature::LookUpAndDown(float Value)
{
	AddControllerPitchInput(Value);
}

void ACreature::LookSide(float Value)
{
	AddControllerYawInput(Value);
}


// Called every frame
void ACreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveTowards"), this, &ACreature::MoveTowards);
	PlayerInputComponent->BindAxis(FName("LookUpAndDown"), this, &ACreature::LookUpAndDown);
	PlayerInputComponent->BindAxis(FName("LookSide"), this, &ACreature::LookSide);
}