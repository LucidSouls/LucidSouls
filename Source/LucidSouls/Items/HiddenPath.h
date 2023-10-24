// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HiddenPath.generated.h"

class UBoxComponent;
UCLASS()
class LUCIDSOULS_API AHiddenPath : public AActor
{
	GENERATED_BODY()
	
public:	
	
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UStaticMeshComponent> Frame;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UStaticMeshComponent> Door;
	
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(EditAnywhere)
		TObjectPtr<AActor> Destination;

	// Sets default values for this actor's properties
	AHiddenPath();

	UFUNCTION()
		void OnStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
