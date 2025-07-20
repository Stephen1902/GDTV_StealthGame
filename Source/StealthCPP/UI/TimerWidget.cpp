// Copyright 2025 DME Games

#include "UI/TimerWidget.h"
#include "Components/TextBlock.h"

FText UTimerWidget::GetCurrentTime() const
{
	FString StringToDisplay = AppendInteger(Minutes) + " : " + AppendInteger(Seconds); 
	return(FText::FromString(*StringToDisplay));
}

void UTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Seconds = 0;
	Minutes = 0;

	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UTimerWidget::TimerTick, 1.0f, true, 0.f);
}

void UTimerWidget::TimerTick()
{
	Seconds += 1;

	if (Seconds >= 60)
	{
		Minutes += 1;
		Seconds = 0;
	}

	FString StringToDisplay = AppendInteger(Minutes) + " : " + AppendInteger(Seconds); 
	TB_Timer->SetText(FText::FromString(*StringToDisplay));
}

FString UTimerWidget::AppendInteger(const int32 IntegerIn)
{
	if (IntegerIn < 10)
	{
		return "0" + FString::FromInt(IntegerIn);
	}

	return FString::FromInt(IntegerIn);
}
