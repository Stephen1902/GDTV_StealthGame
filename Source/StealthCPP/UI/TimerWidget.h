// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FText GetCurrentTime() const;
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* TB_Timer;

private:
	int32 Seconds;
	int32 Minutes;

	UPROPERTY()
	FTimerHandle TickTimerHandle;
	UFUNCTION()
	void TimerTick();

	static FString AppendInteger(int32 IntegerIn);
};
