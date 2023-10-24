// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LucidSouls/Components/AttributeComponent.h"
#include "LucidSouls/HUD/HealthBarComponent.h"
#include "Animation/AnimInstance.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	ActorAttributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("ActorAttributes"));
	HealthBar = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (HealthBar)
	{
		HealthBar->UpdateHealth(1.f);

	}
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	if (ActorAttributes && ActorAttributes->IsAlive()) 
	{
		const FVector Forward = GetActorForwardVector();
		const FVector ImpactLow(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
		const FVector Hit = (ImpactLow - GetActorLocation()).GetSafeNormal();

		const double CosTheta = FVector::DotProduct(Forward, Hit);
		double Theta = FMath::Acos(CosTheta);
		Theta = FMath::RadiansToDegrees(Theta);

		const FVector CrossProduct = FVector::CrossProduct(Forward, Hit);
		if (CrossProduct.Z < 0)
		{
			Theta *= -1.f;
		}

		FName Section("BackHit");

		if (Theta >= -45.f && Theta < 45.f)
		{
			Section = FName("FrontHit");
		}
		else if (Theta >= -135.f && Theta < -45.f)
		{
			Section = FName("LeftHit");
		}
		else if (Theta >= 45.f && Theta < 135.f)
		{
			Section = FName("RightHit");
		}

		PlayHitMontage(FName(Section));
		/*UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);


		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString::Printf(TEXT("Theta: %f"), Theta));
		}
		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 100.f, 5.f, FColor::Red, 5.f);
		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Hit * 100.f, 5.f, FColor::Green, 5.f);*/
	}
	else
	{
		PlayDeathMontage();
	}
	
	
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//Check if ActorAttributes component is valid
	if (ActorAttributes)
	{
		//Apply damage to ActorAttributes (decrease health)
		ActorAttributes->ApplyDamage(DamageAmount);
		
		//Check if HealthBar component is valid
		if (HealthBar)
		{
			//Update the health bar according to health attribute current status
			float CurrentHealth = ActorAttributes->GetHealthStatus();
			HealthBar->UpdateHealth(CurrentHealth);

		}
	}
	return DamageAmount;
}

void AEnemy::PlayHitMontage(const FName& Section)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitMontage)
	{
		AnimInstance->Montage_Play(HitMontage);
		AnimInstance->Montage_JumpToSection(Section, HitMontage);
	}
}

void AEnemy::PlayDeathMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		int32 Selection = FMath::RandRange(0, 3);
		FName Section = FName();
		switch (Selection)
		{
		case 0:
			Section = FName("Death1");
			Death = EDeath::EDP_Death1;
			break;
		case 1:
			Section = FName("Death2");
			Death = EDeath::EDP_Death2;
			break;
		case 2:
			Section = FName("Death3");
			Death = EDeath::EDP_Death3;
			break;
		case 3:
			Section = FName("Death4");
			Death = EDeath::EDP_Death4;
			break;
		default:

			break;
		}
		AnimInstance->Montage_JumpToSection(Section, DeathMontage);
	}
}

