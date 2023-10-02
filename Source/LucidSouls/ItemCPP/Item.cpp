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
	RootComponent = ItemStaticMeshComponent;
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
	
	// Using Cosine function (amplitude 3 and time constant 2 with elasped time) to determine the z offset of item
	ElapsedTime += DeltaTime;
	float NewDeltaZ = 1.0f * FMath::Cos(ElapsedTime * 2.0f); 
	AddActorWorldOffset(FVector(0.f, 0.f, NewDeltaZ));
}

void AItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);

	const FString OtherActorName = OtherActor->GetName();
	const FString TestLog = OtherActorName + " has taken the big donut";
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, TestLog);
	}
	if (MyCharacter) {
		MyCharacter->SetOverlappingItem(this);
	}

}

void AItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	const FString OtherActorName = OtherActor->GetName();
	const FString TestLog = OtherActorName + " has stopped overlapping with the big donut";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Yellow, TestLog);
	}
	if (MyCharacter) {
		MyCharacter->SetOverlappingItem(nullptr);
	}
}


