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


void ALucidSoulsPawn::BeginPlay()
{
	Super::BeginPlay();

}


void ALucidSoulsPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void CharacterAnim::UpdateLocomotionState(ECharacterState NewState) {
    if (CurrentState != NewState) {
        CurrentState = NewState;
        switch (NewState) {
        case ECharacterState::Running:
            PlayRunningAnimation();
            break;
        case ECharacterState::Sprinting:
            PlaySprintingAnimation();
            break;
        case ECharacterState::Jumping:
            PlayJumpAnimation();
            break;
            // Other locomotion states and their respective animations
        }
    }
}

// This code snippet demonstrates a custom animation blending system for fluid transitions.

void CharacterAnim::BlendAnimations(UAnimMontage* BaseMontage, UAnimMontage* OverlayMontage) {
    if (BaseMontage && OverlayMontage) {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance) {
            float BlendDuration = 0.2f; // Set blending duration
            AnimInstance->Montage_Play(BaseMontage);
            AnimInstance->Montage_Play(OverlayMontage, BlendDuration);
            // Logic for handling blending between animations
        }
    }
}

void CharacterAnim::JumpToAnimationSection(FName SectionName, UAnimMontage* Montage) {
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && Montage) {
        AnimInstance->Montage_Play(Montage);
        AnimInstance->Montage_JumpToSection(SectionName, EAnimPlayMode::Stop);
        // Improve the logic for more precise section jumps
    }
}