// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleSquare.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
UCLASS()
class LUCIDSOULS_API APuzzleSquare : public AActor
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
        TObjectPtr<UMaterialInterface> MaterialOn;

    UPROPERTY(EditAnywhere)
        TObjectPtr<UMaterialInterface> MaterialOff;

    UPROPERTY(VisibleAnywhere)
        TObjectPtr<UStaticMeshComponent> Cube;

    UPROPERTY(VisibleAnywhere)
        TObjectPtr<UBoxComponent> CollisionBox;

    UPROPERTY(EditAnywhere)
        TArray<TObjectPtr<APuzzleSquare>> TargetSquares;

    


    UPROPERTY(EditAnywhere)
        bool IsOn;

    // Sets default values for this actor's properties
    APuzzleSquare();

    UFUNCTION()
        void OnStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

};