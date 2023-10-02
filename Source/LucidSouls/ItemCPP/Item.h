// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UBoxComponent;

UCLASS()
class LUCIDSOULS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	float ElapsedTime;

	// Declare a mesh for the item (appearance to be assigned in game with asset)
	UPROPERTY(EditAnywhere, Category = "Item")
	UStaticMeshComponent* ItemStaticMeshComponent;

	// Declare a hit box surrounding the item for handling collision
	UPROPERTY(EditAnywhere)
	UBoxComponent* HitBox;

	// Declare the function to handle overlap events (used for OnActorBeginOverlap)
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// Declare the function to handle end overlap
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};