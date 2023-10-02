// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement= true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 2000.f;
	SpringArm->bUsePawnControlRotation = true;
	ActualCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	ActualCamera->SetupAttachment(SpringArm);


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
	PlayerInputComponent->BindAxis(FName("MoveTowards"), this, &AMyCharacter::MoveTowards);
	PlayerInputComponent->BindAxis(FName("MoveSide"), this, &AMyCharacter::MoveSide);
	PlayerInputComponent->BindAxis(FName("LookUpAndDown"), this, &AMyCharacter::LookUpAndDown);
	PlayerInputComponent->BindAxis(FName("LookSide"), this, &AMyCharacter::LookSide);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
}

void AMyCharacter::MoveTowards(float Value)
{
	if ((Value != 0.f) && Controller != NULL) {

		FRotator Rotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
		FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyCharacter::MoveSide(float Value)
{
	if ((Value != 0.f) && Controller != NULL) {
		FRotator Rotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
		FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMyCharacter::LookUpAndDown(float Value)
{
	AddControllerPitchInput(Value);
}

void AMyCharacter::LookSide(float Value)
{
	AddControllerYawInput(Value);
}

