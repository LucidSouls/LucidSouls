// Fill out your copyright notice in the Description page of Project Settings.



#pragma once



#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"



class APuzzleLight;
class APuzzleSquare;



UCLASS()
class LUCIDSOULS_API ADoor : public AActor
{
	GENERATED_BODY()

public:



	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UStaticMeshComponent> Cube;



	UPROPERTY(EditAnywhere)
		bool Open;



	UPROPERTY(EditAnywhere)
		TArray<TObjectPtr<APuzzleLight>> Lights;
	UPROPERTY(EditAnywhere)
		TArray<TObjectPtr<APuzzleSquare>> Squares;
	UPROPERTY(EditAnywhere)
		TArray<TObjectPtr<APuzzleSquare>> OffSquares;


	UPROPERTY(EditAnywhere)
		TObjectPtr<AActor> Destination;







	// Sets default values for this actor's properties
	ADoor();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector Direction;
	float DistanceFromStartToDestination;
	float DistanceFromStartToCurrent;
	FVector StartLocation;
};