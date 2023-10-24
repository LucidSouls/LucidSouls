// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleLight.generated.h"

class UBoxComponent;
class UPointLightComponent;
UCLASS()
class LUCIDSOULS_API APuzzleLight : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UStaticMeshComponent> Cube;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UStaticMeshComponent> Wall;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UPointLightComponent> Light;

	UPROPERTY(EditAnywhere)
		int LightNum;

	UPROPERTY(EditAnywhere)
		int LightOrder;

	// Sets default values for this actor's properties
	APuzzleLight();

	UFUNCTION()
		void OnStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void ChangeLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
