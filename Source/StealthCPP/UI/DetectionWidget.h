// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DetectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API UDetectionWidget : public UUserWidget
{
	GENERATED_BODY()

	// Called when game starts
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Detection Widget", meta=(BindWidget))
	class UCanvasPanel* DetectionPanel;

	UPROPERTY(EditDefaultsOnly, Category = "Detection Widget", meta=(BindWidget))
	class UProgressBar* DetectionProgress;

	UPROPERTY(EditDefaultsOnly, Category = "Detection Widget", meta=(BindWidget))
	class UTextBlock* DetectionText;

public:
	void UpdateProgressPercent(float CurrentPercent);
};
