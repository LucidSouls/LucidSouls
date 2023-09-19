// Fill out your copyright notice in the Description page of Project Settings.


#include "LucidSoulsPawn.h"

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

    MovementComponent = CreateDefaultSubobject<ULucidSoulsMovementComponent>("MovementComponent");
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