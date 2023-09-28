// Fill out your copyright notice in the Description page of Project Settings.


#include "LucidSoulsPawn.h"
#include "LucidSouls/MovementCPP/LucidSoulsPawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ALucidSoulsPawn::ALucidSoulsPawn(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    RootCapsule = CreateDefaultSubobject<UCapsuleComponent>("RootCapsule");
    check(RootCapsule != nullptr);

    Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>("Mesh");

    if (Mesh)
        Mesh->SetupAttachment(RootCapsule);
    
    MovementComponent = CreateDefaultSubobject<ULucidSoulsPawnMovementComponent>("MovementComponent");
    if (MovementComponent)
    {
        MovementComponent->UpdatedComponent = RootCapsule;
    }
    
    CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
    if (CameraArm)
    {
        CameraArm->SetupAttachment(RootCapsule);
    }

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
    if (Camera)
        Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ALucidSoulsPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALucidSoulsPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALucidSoulsPawn::MoveTowards(FVector AbsoluteMovementDirection)
{
    //MovementComponent->AddInputVector(AbsoluteMovementDirection);
    //AddMovementInput(AbsoluteMovementDirection);
    UE_LOG(LogTemp, Warning, TEXT("MoveTowards is called!"));
}

void ALucidSoulsPawn::Sprint() 
{
    //MovementComponent->SetSprintSpeed
    //MovementComponent->Run();
    UE_LOG(LogTemp, Warning, TEXT("Sprint is called!"));
}

//void ALucidSoulsPawn::MoveForward(float Value)
//{
//    // Calculate the movement direction based on the camera view.
//    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
//    AddMovementInput(Direction, Value);
//}
//
//void ALucidSoulsPawn::MoveRight(float Value)
//{
//    // Calculate the right movement direction based on the camera view.
//    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
//    AddMovementInput(Direction, Value);
//}
//
//void ALucidSoulsPawn::MoveBackward(float Value)
//{
//    // Reverse the forward movement direction.
//    MoveForward(-Value);
//}