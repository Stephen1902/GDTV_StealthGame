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

void UDetectionWidget::UpdateProgressPercent(float CurrentPercent)
{
	DetectionProgress->SetPercent(CurrentPercent);
	if (CurrentPercent > 1.0f)
	{
		if (DetectionText->GetVisibility() == ESlateVisibility::Hidden)
		{
			DetectionText->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (DetectionText->GetVisibility() == ESlateVisibility::Visible)
		{
			DetectionText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
