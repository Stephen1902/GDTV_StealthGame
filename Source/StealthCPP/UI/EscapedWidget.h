// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EscapedWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API UEscapedWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Escaped UI", meta=(BindWidget))
	class UImage* BG_Image;

	UPROPERTY(EditDefaultsOnly, Category = "Escaped UI", meta=(BindWidget))
	class UTextBlock* TB_Escaped;

	UPROPERTY(EditDefaultsOnly, Category = "Escaped UI", meta=(BindWidget))
	class UButton* BTN_Retry;
	
	UPROPERTY(EditDefaultsOnly, Category = "Escaped UI", meta=(BindWidget))
	UButton* BTN_MainMenu;

	UFUNCTION(BlueprintImplementableEvent, Category = "Escaped UI")
	void TriggerImageFade();
public:
	void FadeImage();

	UFUNCTION()
	void OnRetryButtonClicked();

	UFUNCTION()
	void OnMainMenuButtonClicked();
};
