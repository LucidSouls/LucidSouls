// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class LUCIDSOULS_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeapon();

protected:

	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	bool isAttached;

public:
	// Called every frame
	virtual void Tick(float DeltaTime);

	void AttachToCharacter(USceneComponent* CharacterMesh, FName SocketInCharacterMeshName);

};

