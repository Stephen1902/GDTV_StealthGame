// Copyright 2025 DME Games


#include "UI/CapturedWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UCapturedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_Retry->OnClicked.AddDynamic(this, &UCapturedWidget::OnRetryButtonClicked);
	BTN_MainMenu->OnClicked.AddDynamic(this, &UCapturedWidget::OnMainMenuButtonClicked);
	
}

void UCapturedWidget::FadeImage()
{
	TriggerImageFade();
}

void UCapturedWidget::OnRetryButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}

void UCapturedWidget::OnMainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}
