// Copyright 2025 DME Games

#include "UI/DetectionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UDetectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DetectionProgress->SetPercent(0.f);
	DetectionText->SetVisibility(ESlateVisibility::Hidden);
}
