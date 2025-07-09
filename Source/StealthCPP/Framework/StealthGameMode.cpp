// Copyright 2025 DME Games

#include "StealthGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Enemies/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "SensorsAlarms/Alarm.h"
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

void AStealthGameMode::PlayerHasBeenCaught()
{
	// Turn off the escape music
	if (EscapeMusicComp)
	{
		EscapeMusicComp->FadeOut(0.3f, 0.0f);		
	}

	// Turn off the rain
	if (RainSoundComp)
	{
		RainSoundComp->FadeOut(0.3f, 0.0f);
	}

	// Turn off the alarm
	if (AAlarm* AlarmActor = Cast<AAlarm>(UGameplayStatics::GetActorOfClass(GetWorld(), AAlarm::StaticClass())))
	{
		AlarmActor->StopAlarm();
	}
	
	// Play the caught sound effect
	if (CaughtSoundEffect)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), CaughtSoundEffect);
	}
	
	// Disable all the AI Guards
	TArray<AActor*> AllGuardControllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyAIController::StaticClass(), AllGuardControllers);
	if (AllGuardControllers.Num() > 0)
	{
		for (int32 i = 0; i < AllGuardControllers.Num() - 1; ++i)
		{
			if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(AllGuardControllers[i]))
			{
				EnemyAIController->SetIsDead();
			}
		}
	}
	
	// Add the captured widget to the screen, setting the focus to it
	if (CapturedWidgetToDisplay)
	{
		CapturedWidgetRef = CreateWidget<UCapturedWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), CapturedWidgetToDisplay);
		if (CapturedWidgetRef)
		{
			CapturedWidgetRef->AddToViewport();
			CapturedWidgetRef->FadeImage();

			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				PC->SetShowMouseCursor(true);
				FInputModeUIOnly InputModeUIOnly;
				InputModeUIOnly.SetWidgetToFocus(CapturedWidgetRef->TakeWidget());
				PC->SetInputMode(InputModeUIOnly);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AStealthGameMode is expecting a CapturedWidgetToDisplay"));
	}
}

void AStealthGameMode::SwitchToEscapeMusic()
{
	// We only want to trigger the escape music if it hasn't already been triggered
	if (!EscapeMusicComp)
	{
		if (StealthMusicComp && EscapeMusic)
		{
			StealthMusicComp->FadeOut(0.3f, 0.0f);
			EscapeMusicComp = UGameplayStatics::SpawnSound2D(GetWorld(), EscapeMusic, 0.5f);
			EscapeMusicComp->FadeIn(0.3f);
		}
	}
}
