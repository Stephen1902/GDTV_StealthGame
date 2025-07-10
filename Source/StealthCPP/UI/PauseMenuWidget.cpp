// Copyright 2025 DME Games

#include "UI/PauseMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Player/StealthCharacter.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_Resume->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeButtonClicked);
	BTN_MainMenu->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMainMenuButtonClicked);
}

void UPauseMenuWidget::OnResumeButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked"));
	if (AStealthCharacter* StealthCharacter = Cast<AStealthCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		StealthCharacter->TogglePauseMenu();
	}
}

void UPauseMenuWidget::OnMainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}
