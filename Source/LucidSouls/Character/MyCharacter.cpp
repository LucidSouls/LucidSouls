// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNPC.h"

#include "MyCharacter.h"

#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "LucidSouls/Items/Item.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "LucidSouls/LucidSoulsGameModeBase.h"
#include "LucidSouls/PawnsCPP/Creature.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	PowerUp = false;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement= true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	SpringArm->TargetArmLength = 500.f;
	SpringArm->bUsePawnControlRotation = true;
	ActualCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	ActualCamera->SetupAttachment(SpringArm);


}

void AMyCharacter::ItemCollisionEnable(ECollisionEnabled::Type CollisionEnabled)
{
	if (ItemOverlap != NULL && ItemOverlap->GetCollisionBox())
	{
		ItemOverlap->GetCollisionBox()->SetCollisionEnabled(CollisionEnabled);
	}
}

void AMyCharacter::ItemCollisionDisable()
{
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

void AMyCharacter::Talk()
{
	TArray<TObjectPtr<AActor>> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AMyNPC::StaticClass()); 

	for (AActor* Actor : OverlappingActors)
	{
		
		if (Actor->GetClass()->ImplementsInterface(UNPCInteract::StaticClass()))
		{
			INPCInteract* NPCInterface = Cast<INPCInteract>(Actor);
			if (NPCInterface)
			{
				NPCInterface->Talk();
			}
		}
	}

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

}



void AMyCharacter::Attack()
{
	StartAttacks();

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
	PlayerInputComponent->BindAxis(FName("MoveTowards"), this, &AMyCharacter::MoveTowards);
	PlayerInputComponent->BindAxis(FName("MoveSide"), this, &AMyCharacter::MoveSide);
	PlayerInputComponent->BindAxis(FName("LookUpAndDown"), this, &AMyCharacter::LookUpAndDown);
	PlayerInputComponent->BindAxis(FName("LookSide"), this, &AMyCharacter::LookSide);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AMyCharacter::Attack);
	PlayerInputComponent->BindAction(FName("AttachDeattach"), IE_Pressed, this, &AMyCharacter::AttachItem);
	PlayerInputComponent->BindAction(FName("Transform"), IE_Pressed, this, &AMyCharacter::Transform);
	PlayerInputComponent->BindAction(FName("Move"), IE_Pressed, this, &AMyCharacter::GrabItem);
	PlayerInputComponent->BindAction(FName("Move"), IE_Released, this, &AMyCharacter::ReleaseItem);
	//PlayerInputComponent->BindKey(EKeys::T, IE_Pressed, this, &AMyCharacter::Talk);


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

void AMyCharacter::AttachItem() 
{
	if (ItemOverlap)
	{
		ItemOverlap->EquipItem(GetMesh(), FName("HandSocket"),this,this);

	}
}



void AMyCharacter::StartAttacks()
{
	//Create a pointer to AnimInstance component of the Mesh
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && Attacks)
	{
		//Check animation duration
		FString DebugMessage = FString::Printf(TEXT("Attack montage duration: %f"), (AnimInstance->Montage_Play(Attacks)));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, DebugMessage);
		}
		
		//Randomize selection for attacks
		int32 Select = FMath::RandRange(1, 2);
		
		FName Section = FName(FString::FromInt(Select));
		
		//Play the montage
		AnimInstance->Montage_Play(Attacks);
		AnimInstance->Montage_JumpToSection(Section, Attacks);
	}
}

void AMyCharacter::Transform()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, "called base game");
	}
	GetWorld()->GetAuthGameMode()->DefaultPawnClass = ACreature::StaticClass();
	//ALucidSoulsGameModeBase GameMode = Cast<ALucidSoulsGameModeBase>(UGameplayStatics::GetGameMode);
}

