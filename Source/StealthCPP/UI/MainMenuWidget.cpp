// Copyright 2025 DME Games


#include "UI/MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_Play->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayButtonClicked);
	BTN_Exit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
}

void UMainMenuWidget::OnPlayButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}

void UMainMenuWidget::OnExitButtonClicked()
{
	TEnumAsByte QuitPreference = EQuitPreference::Quit; 
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, QuitPreference, false);
}
