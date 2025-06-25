// Copyright 2025 DME Games

#include "StealthGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CapturedWidget.h"

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
