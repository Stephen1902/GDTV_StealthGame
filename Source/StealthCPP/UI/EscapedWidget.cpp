// Copyright 2025 DME Games

#include "UI/EscapedWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UEscapedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_Retry->OnClicked.AddDynamic(this, &UEscapedWidget::OnRetryButtonClicked);
	BTN_MainMenu->OnClicked.AddDynamic(this, &UEscapedWidget::OnMainMenuButtonClicked);
}

void UEscapedWidget::FadeImage()
{
	TriggerImageFade();
}

void UEscapedWidget::OnRetryButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}

void UEscapedWidget::OnMainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}
