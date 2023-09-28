// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMeshComponent"));
	RootComponent = ItemStaticMeshComponent;
	//ItemStaticMeshComponent->SetWorldScale3D(FVector(6.0f, 6.0f, 6.0f));

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlap);
}


// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunTime += DeltaTime;
	float DeltaZ = Amplitude * FMath::Sin(RunTime * TimeConstant);
	AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));
}

void AItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherActorName = OtherActor->GetName();
	const FString TestLog = OtherActorName + " has taken the big donut";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, TestLog);
	}
}


