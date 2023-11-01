// Fill out your copyright notice in the Description page of Project Settings.





#include "PuzzleLight.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include <Kismet/GameplayStatics.h>





// Sets default values
APuzzleLight::APuzzleLight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	UStaticMesh* CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	Cube->SetStaticMesh(CubeMesh);
	SetRootComponent(Cube);
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());
	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
	Wall->SetStaticMesh(CubeMesh);
	Wall->SetupAttachment(GetRootComponent());
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(GetRootComponent());
}



void APuzzleLight::OnStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		//In game debug the name of the actor that overlap with the item
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, FString("Light Overlap: ") + OtherActor->GetName());
	}



	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);



	if (PlayerController)
	{
		PlayerController->InputComponent->BindAction(FName("Light"), IE_Pressed, this, &APuzzleLight::ChangeLight);
	}
}



void APuzzleLight::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
	{
		//In game debug the name of the actor that overlap with the item
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, FString("Light End Overlap: ") + OtherActor->GetName());
	}



	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);



	if (PlayerController)
	{




		for (int Index = 0; Index < (PlayerController->InputComponent->GetNumActionBindings()); Index++)
		{
			FInputActionBinding& ActionBinding = PlayerController->InputComponent->GetActionBinding(Index);



			if (ActionBinding.GetActionName() == FName("Light")) {
				PlayerController->InputComponent->RemoveActionBinding(Index);
				break;
			}
		}

	}
}



void APuzzleLight::ChangeLight()
{
	LightNum++;
	if (LightNum == 3)
	{
		LightNum = 0;
	}
	if (GEngine)
	{
		if (LightNum == LightOrder) {
			GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Green, "Correct");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, "Wrong");
		}
	}

}





// Called when the game starts or when spawned
void APuzzleLight::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APuzzleLight::OnStartOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APuzzleLight::OnEndOverlap);

}



// Called every frame
void APuzzleLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FLinearColor LightColor;
	switch (LightNum)
	{
	case 0:
		LightColor = FLinearColor(1.0f, 0.0f, 0.0f);  // Red color
		break;
	case 1:
		LightColor = FLinearColor(0.0f, 0.0f, 1.0f);  // Blue color
		break;
	case 2:
		LightColor = FLinearColor(0.0f, 1.0f, 0.0f);  // Green color
		break;
	default:
		break;
	}
	Light->SetLightColor(LightColor);



}