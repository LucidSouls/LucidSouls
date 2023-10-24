// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UBoxComponent;
class USceneComponent;
class APuzzleSquare;
UCLASS()
class LUCIDSOULS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(VisibleAnywhere)
	float TimePeriod;

	UPROPERTY(VisibleAnywhere)
	float Damage = 20.f;

	UPROPERTY(VisibleAnywhere)
	 TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> StartPoint;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EndPoint;


	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnStartBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void EquipItem(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	UBoxComponent* GetCollisionBox() const { return CollisionBox; };


};
