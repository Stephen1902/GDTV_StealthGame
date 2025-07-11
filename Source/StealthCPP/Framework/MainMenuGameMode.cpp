// Copyright 2025 DME Games

#include "Framework/MainMenuGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MainMenuWidget.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	static ConstructorHelpers::FClassFinder<UMainMenuWidget> FoundWidget(TEXT("/Game/UI/WBP_Menu"));
	if (FoundWidget.Succeeded())
	{
		MainMenuWidget = FoundWidget.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> FoundMusic(TEXT("/Game/Audio/Music/StealthMusic"));
	if (FoundMusic.Succeeded())
	{
		MainMenuMusic = FoundMusic.Object;
	}
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidget)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (UMainMenuWidget* MainMenuWidgetRef = CreateWidget<UMainMenuWidget>(PC, MainMenuWidget))
		{
			MainMenuWidgetRef->AddToViewport();
			PC->SetShowMouseCursor(true);
			FInputModeUIOnly InputModeUIOnly;
			InputModeUIOnly.SetWidgetToFocus(MainMenuWidgetRef->TakeWidget());
			PC->SetInputMode(InputModeUIOnly);
		}
		
		if (MainMenuMusic)
		{
			UGameplayStatics::SpawnSound2D(GetWorld(), MainMenuMusic, 0.3f);
		}
				
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Main Menu Widget was found."));
	}
}
