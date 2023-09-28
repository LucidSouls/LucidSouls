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
	
	float RunTime;
	float Amplitude = 0.4f;
	float TimeConstant = 4.0f;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, Category = "Item")
	UStaticMeshComponent* ItemStaticMeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* HitBox;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
