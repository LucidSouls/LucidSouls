// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"
#include "../PlayerCPP/MyCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set the static mesh component to the root component
	ItemStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMeshComponent"));
	ItemStaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(ItemStaticMeshComponent);
	//ItemStaticMeshComponent->SetWorldScale3D(FVector(6.0f, 6.0f, 6.0f));
	// Create the hit box component and attach it to the root component
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(GetRootComponent());

	// Bind both OnOverlap and OnEndOverlap functions to the HitBox overlapp/end overlap events
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlap);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &AItem::OnEndOverlap);



}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ElapsedTime += DeltaTime;

	// Calculate a sinusoidal vertical motion using the cosine function and DeltaTime.
	float ItemZLocation = 1.0f * FMath::Cos(ElapsedTime * 2.0f);

	// Apply the vertical motion to the actor's Z position.
	AddActorWorldOffset(FVector(0.f, 0.f, ItemZLocation));

	// Adjust the actor's scale to create a pulsating effect over time.
	float ScaleFactor = 1.0f + 0.2f * FMath::Sin(ElapsedTime * 3.0f); 
	float ScaleFactorY = 1.0f * FMath::Cos(ElapsedTime * 0.5f + 1.5f);
	//float ScaleFactorZ = -5.0f * FMath::Sin(ElapsedTime);
	SetActorScale3D(FVector(1.f, ScaleFactorY, ScaleFactor));// Pulsate with time

	// Calculate a rotation angle based on DeltaTime for a spinning effect.
	float RotationAngle = DeltaTime * 90.0f; // Rotate 90 degrees per second.

	// Apply a rotation to the actor around the Y-axis.
	AddActorWorldRotation(FRotator(RotationAngle, 0.f, 0.f));
}

void AItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor))
	{
		const FString OtherActorName = OtherActor->GetName();
		MyCharacter->SetItemOverlap(this);
	}

}

void AItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	

	if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor))
	{
		const FString OtherActorName = OtherActor->GetName();
		MyCharacter->SetItemOverlap(nullptr);
	}

}


