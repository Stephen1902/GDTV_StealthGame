// Copyright 2025 DME Games

#include "StealthGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CapturedWidget.h"

void AStealthGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (StealthMusic)
	{
		StealthMusicComp = UGameplayStatics::SpawnSound2D(GetWorld(), StealthMusic, 0.5f);
		StealthMusicComp->FadeIn(0.3f);
	}

	if (RainSoundEffect)
	{
		RainSoundComp = UGameplayStatics::SpawnSound2D(GetWorld(), RainSoundEffect, 1.0f);
		RainSoundComp->FadeIn(0.3f);		 
	}
}

void AStealthGameMode::CreateCapturedWidget()
{
	if (CapturedWidgetToDisplay)
	{
		CapturedWidgetRef = CreateWidget<UCapturedWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), CapturedWidgetToDisplay);
		if (CapturedWidgetRef)
		{
			CapturedWidgetRef->AddToViewport();
			CapturedWidgetRef->FadeImage();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AStealthGameMode is expecting a CapturedWidgetToDisplay"));
	}
}

void AStealthGameMode::SwitchToEscapeMusic()
{
	if (StealthMusicComp && EscapeMusic)
	{
		StealthMusicComp->FadeOut(0.3f, 1.0f);
		EscapeMusicComp = UGameplayStatics::SpawnSound2D(GetWorld(), EscapeMusic, 0.5f);
		EscapeMusicComp->FadeIn(0.3f);
	}
}
