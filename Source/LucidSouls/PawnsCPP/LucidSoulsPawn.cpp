// Fill out your copyright notice in the Description page of Project Settings.


#include "LucidSoulsPawn.h"

// Sets default values
ALucidSoulsPawn::ALucidSoulsPawn(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
    //Set default health and stamina
    Health = 100.0f;
    Stamina = 100.0f;

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

void ALucidSoulsPawn::DecreaseHealth(float Amount)
{
    Health -= Amount;

}

void ALucidSoulsPawn::DecreaseStamina(float Amount)
{
    Stamina -= Amount;
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