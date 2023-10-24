// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "LucidSouls/Character/MyCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "LucidSouls/Interface/OnHitInterface.h"
#include "LucidSouls/Items/PuzzleSquare.h"
#include "GameFramework/DamageType.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	StartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("StartPoint"));
	StartPoint->SetupAttachment(GetRootComponent());
	EndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndPoint"));
	EndPoint->SetupAttachment(GetRootComponent());

}

void AItem::OnStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{	
		//In game debug the name of the actor that overlap with the item
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, FString("Start Overlap: ") + OtherActor->GetName());
	}
	
	//Assign a pointer to the overlapped actor which was casted into MyCharacter type 
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter) 
	{
		//Set the overlap item of my character to this item
		MyCharacter->SetItemOverlap(this);

	}

}


void AItem::OnStartBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TArray<AActor*> ActorsIgnore;
	FHitResult HitBox;
	ActorsIgnore.Add(this);
	const FVector Start = StartPoint->GetComponentLocation();
	const FVector End = EndPoint->GetComponentLocation();
	UKismetSystemLibrary::BoxTraceSingle(this, Start, End, FVector(4.f,4.f,4.f),
		StartPoint->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsIgnore,
		EDrawDebugTrace::ForDuration,
		HitBox,
		true
		);
	if (HitBox.GetActor())
	{
		UGameplayStatics::ApplyDamage(
			HitBox.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);

		IOnHitInterface* OnHitInterface = Cast<IOnHitInterface>(HitBox.GetActor());
		if (OnHitInterface)
		{
			OnHitInterface->GetHit(HitBox.ImpactPoint);
		}

		
	}
}


void AItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
	{
		//In game debug the name of the overlap actor after finishing overlaping with item
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Cyan, FString("End Overlap: ") + OtherActor->GetName());
	}

	//Assign a pointer to the overlapped actor which was casted into MyCharacter type
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter)
	{
		//Set the overlap item of my character to nullpointer
		MyCharacter->SetItemOverlap(nullptr);
		
	}
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnStartBoxOverlap);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnStartOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnEndOverlap);

	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AItem::EquipItem(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	StaticMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

