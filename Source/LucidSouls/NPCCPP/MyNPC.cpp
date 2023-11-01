// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNPC.h"
#include "../Interfaces/NPCInteract.h"
#include "LucidSouls/HUDCPP/DialogueWidgetBP.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/PlayerController.h"
#include "Sound/SoundBase.h"


// Sets default values
AMyNPC::AMyNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Index = -1;

}

// Called when the game starts or when spawned
void AMyNPC::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyNPC::Talk()
{
	Index++;
	if (Index > Dialogue.Num()) {
		DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
		Index = -1;
	}
	else {
		if (DialogueWidget) {
			DialogueWidget->SetVisibility(ESlateVisibility::Visible);
			DialogueWidget->Text->SetText(FText::FromString(Dialogue[Index]));
		}
		else {
			TSubclassOf<UDialogueWidgetBP> DialogueWidgetClass;
			DialogueWidget = CreateWidget<UDialogueWidgetBP>(UGameplayStatics::GetPlayerController(this, 0), DialogueWidgetClass);
			DialogueWidget->AddToViewport();
			DialogueWidget->Text->SetText(FText::FromString(Dialogue[Index]));
		}
	}

}

void AMyNPC::ChangeLevel()
{
	if (LevelName.Len() > 0) {
		UGameplayStatics::OpenLevel(this, FName(LevelName));
	}
}


void AMyNPC::PlaySound()
{
	if (Soundtrack) {
		UGameplayStatics::PlaySound2D(this, Soundtrack);
	}
}