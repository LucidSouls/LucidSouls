// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "UObject/ConstructorHelpers.h"
#include "LucidSouls/Items/PuzzleLight.h"
#include "LucidSouls/Items/PuzzleSquare.h"
// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	UStaticMesh* CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	Cube->SetStaticMesh(CubeMesh);
	SetRootComponent(Cube);

	

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	if (Destination != NULL)
	{
		StartLocation = GetActorLocation();
		
		Direction = Destination->GetActorLocation() - StartLocation;
		
		DistanceFromStartToDestination = Direction.Size();
		
		Direction = Direction.GetSafeNormal(); //convert it to a unit vector (size 1)
		
		DistanceFromStartToCurrent = 0.0f;
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Open = true;

	if (Lights.Num() > 0) {
		for (int Index = 0; Index < Lights.Num(); Index++)
		{
			if (Lights[Index]->LightNum != Lights[Index]->LightOrder)
			{
				Open = false;
			}
		}
	}
	else 
	{
		for (TObjectPtr<APuzzleSquare>& square : Squares)
		{
			if (square->IsOn == false)
			{
				Open = false;
			}
		}
	}

	if (Open) {
		if (Destination != NULL)
		{
			if (DistanceFromStartToCurrent < DistanceFromStartToDestination)
			{	
				FVector Location = GetActorLocation();
				Location += Direction * 200 * DeltaTime;
				SetActorLocation(Location);
				DistanceFromStartToCurrent = (Location - StartLocation).Size();
			}
		}
	}
}

