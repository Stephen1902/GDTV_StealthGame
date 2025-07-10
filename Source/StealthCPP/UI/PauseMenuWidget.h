// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pause UI", meta=(BindWidget))
	class UImage* BG_Image;

	UPROPERTY(EditDefaultsOnly, Category = "Pause UI", meta=(BindWidget))
	class UTextBlock* TB_Pause;

	UPROPERTY(EditDefaultsOnly, Category = "Pause UI", meta=(BindWidget))
	class UButton* BTN_Resume;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pause UI", meta=(BindWidget))
	UButton* BTN_MainMenu;

private:
	UFUNCTION()
	void OnResumeButtonClicked();

	UFUNCTION()
	void OnMainMenuButtonClicked();
	
};
