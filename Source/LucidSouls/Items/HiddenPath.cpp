// Fill out your copyright notice in the Description page of Project Settings.


#include "HiddenPath.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "LucidSouls/Character/MyCharacter.h"

// Sets default values
AHiddenPath::AHiddenPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	SetRootComponent(Frame);
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());
}

void AHiddenPath::OnStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter && Destination)
	{
		MyCharacter->SetActorLocationAndRotation(Destination->GetActorLocation(), Destination->GetActorRotation());
	}
}

// Called when the game starts or when spawned
void AHiddenPath::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AHiddenPath::OnStartOverlap);
	
}

// Called every frame
void AHiddenPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

