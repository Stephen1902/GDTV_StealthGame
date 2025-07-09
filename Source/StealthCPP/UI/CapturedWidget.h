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
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Captured UI", meta=(BindWidget))
	class UImage* BG_Image;

	UPROPERTY(EditDefaultsOnly, Category = "Captured UI", meta=(BindWidget))
	class UTextBlock* TB_Captured;

	UPROPERTY(EditDefaultsOnly, Category = "Captured UI", meta=(BindWidget))
	class UButton* BTN_Retry;
	
	UPROPERTY(EditDefaultsOnly, Category = "Captured UI", meta=(BindWidget))
	UButton* BTN_MainMenu;

	UFUNCTION(BlueprintImplementableEvent, Category = "Captured UI")
	void TriggerImageFade();
public:
	void FadeImage();

	UFUNCTION()
	void OnRetryButtonClicked();

	UFUNCTION()
	void OnMainMenuButtonClicked();
};
