// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "../../PlayerCPP/MyCharacter.h"
#include "../../Interfaces/OnHitInterface.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	isAttached = false;
	WHitBox = CreateDefaultSubobject<UBoxComponent>((TEXT("Hit Collision Box")));
	WHitBox->SetupAttachment(GetRootComponent());
	WHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WHitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WHitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	HitBoxStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Hit Start Point"));
	HitBoxStartPoint->SetupAttachment(GetRootComponent());
	HitBoxEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Hit Box End Point"));
	HitBoxEndPoint->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WHitBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::WeaponHitBoxOverlap);
}

void AWeapon::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AWeapon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::WeaponHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	/*if (OtherActor) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Green, "There is other actor");
		}
	}

	if (GetOwner()->ActorHasTag(TEXT("Enemy"))) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Green, "Owner is enemy actor");
		}
	}*/

	if (GetOwner()->ActorHasTag(TEXT("Player"))) {
		Damage = 10.f;
	}
	
	if (OtherActor->ActorHasTag(TEXT("Enemy"))) {
		ActorsToAvoid.AddUnique(OtherActor);
	}

	//const FVector Start = ;
	//const FVector End = ;

	FHitResult CollisionHitBoxOutHit;
	TArray<AActor*> IgnoreActors;
	
	for (AActor* Actor : ActorsToAvoid) {
		IgnoreActors.AddUnique(Actor);
	}
	IgnoreActors.Add(this);
	IgnoreActors.AddUnique(GetOwner());

	

	

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		HitBoxStartPoint->GetComponentLocation(),
		HitBoxEndPoint->GetComponentLocation(),
		FVector(5.f, 5.f, 5.f),
		HitBoxStartPoint->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		CollisionHitBoxOutHit,
		true
	);


	//if (CollisionHitBoxOutHit.GetActor())
	//{
	//	if (GetInstigator() != nullptr) {
	//		UGameplayStatics::ApplyDamage(
	//			CollisionHitBoxOutHit.GetActor(),
	//			Damage,
	//			GetInstigator()->GetController(),
	//			this,
	//			UDamageType::StaticClass()
	//		);
	//	}

	//	IOnHitInterface* HitInterface = Cast<IOnHitInterface>(CollisionHitBoxOutHit.GetActor());
	//	if (HitInterface) {
	//		HitInterface->OnHitInflicted(CollisionHitBoxOutHit.ImpactPoint);
	//	}
	//	ActorsToAvoid.AddUnique(CollisionHitBoxOutHit.GetActor());

	//	
	//	
	//}

	AActor* HitActor = CollisionHitBoxOutHit.GetActor();
	if (HitActor)
	{
		AController* InstigatorController = GetInstigator() ? GetInstigator()->GetController() : nullptr;
		UGameplayStatics::ApplyDamage(HitActor, Damage, InstigatorController, this, UDamageType::StaticClass());

		if (IOnHitInterface* HitInterface = Cast<IOnHitInterface>(HitActor))
		{
			HitInterface->OnHitInflicted(CollisionHitBoxOutHit.ImpactPoint);
		}

		ActorsToAvoid.AddUnique(HitActor);
	}

}

void AWeapon::Tick(float DeltaTime)
{
	if (isAttached) {

	}
	else {
		//Super::Tick(DeltaTime);
		
		ElapsedTime += DeltaTime;
		
		// Calculate a sinusoidal vertical motion using the cosine function and DeltaTime.
		float ItemZLocation = 1.0f * FMath::Cos(ElapsedTime * 2.0f);

		// Apply the vertical motion to the actor's Z position.
		AddActorWorldOffset(FVector(0.f, 0.f, ItemZLocation));
	}
	
}

void AWeapon::AttachToCharacter(USceneComponent* CharacterMesh, FName SocketInCharacterMeshName, AActor* NewOwner, APawn* NewInstigator) {
	AttachMeshToPlayerSocket(CharacterMesh, SocketInCharacterMeshName);
	
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	// Mark the weapon as attached
	isAttached = true;

	// Debug message to indicate attachment
	/*if (GEngine) {
		GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Green, "Weapon Attached to Character");
	}*/
}

void AWeapon::AttachMeshToPlayerSocket(USceneComponent* CharacterMesh, const FName& SocketInCharacterMeshName)
{
	FAttachmentTransformRules AttachTransformRules(EAttachmentRule::SnapToTarget, true);

	// Attach the ItemStaticMeshComponent to the CharacterMesh at the specified socket
	ItemStaticMeshComponent->AttachToComponent(CharacterMesh, AttachTransformRules, SocketInCharacterMeshName);
}

