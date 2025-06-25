// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CapturedWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API UCapturedWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Captured UI", meta=(BindWidget))
	class UImage* BG_Image;

	UPROPERTY(EditDefaultsOnly, Category = "Captured UI", meta=(BindWidget))
	class UTextBlock* TB_Captured;

	UFUNCTION(BlueprintImplementableEvent, Category = "Captured UI")
	void TriggerImageFade();
public:
	void FadeImage();
};
