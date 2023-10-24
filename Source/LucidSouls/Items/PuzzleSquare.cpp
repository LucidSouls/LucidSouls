// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleSquare.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "LucidSouls/Character/MyCharacter.h"





// Sets default values
APuzzleSquare::APuzzleSquare()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	UStaticMesh* CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	Cube->SetStaticMesh(CubeMesh);
	SetRootComponent(Cube);
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void APuzzleSquare::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APuzzleSquare::OnStartOverlap);
}

// Called every frame
void APuzzleSquare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsOn)
	{
		Cube->SetMaterial(0,MaterialOn);
	}

	else
	{
		Cube->SetMaterial(0,MaterialOff);
	}

}

void APuzzleSquare::OnStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (GEngine)
	{
		//In game debug the name of the actor that overlap with the item
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, FString("Square Overlap: ") + OtherActor->GetName());
	}

	//Assign a pointer to the overlapped actor which was casted into MyCharacter type 
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter && TargetSquares.Num()>0)
	{
		IsOn = !IsOn;
		for (TObjectPtr<APuzzleSquare>& square : TargetSquares)
		{
			square->IsOn = !(square->IsOn);
		}
	}
}


