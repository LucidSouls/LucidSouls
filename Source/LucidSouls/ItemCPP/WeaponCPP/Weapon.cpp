// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "../../PlayerCPP/MyCharacter.h"


AWeapon::AWeapon()
{
	isAttached = false;
}

void AWeapon::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	/*AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter) {
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		ItemStaticMeshComponent->AttachToComponent(MyCharacter->GetMesh(), TransformRules, FName("RightHandSocket"));
		isAttached = true;
	}*/
}

void AWeapon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::Tick(float DeltaTime)
{
	if (isAttached) {

	}
	else {
		Super::Tick(DeltaTime);
	}
	
}

void AWeapon::AttachToCharacter(USceneComponent* CharacterMesh, FName SocketInCharacterMeshName) {
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemStaticMeshComponent->AttachToComponent(CharacterMesh, TransformRules, SocketInCharacterMeshName);
	isAttached = true;
}

