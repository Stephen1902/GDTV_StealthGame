// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Main Menu UI", meta=(BindWidget))
	class UImage* BG_Image;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu UI", meta=(BindWidget))
	class UTextBlock* TB_MainMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu UI", meta=(BindWidget))
	class UButton* BTN_Play;
	
	UPROPERTY(EditDefaultsOnly, Category = "Main Menu UI", meta=(BindWidget))
	UButton* BTN_Exit;

private:
	UFUNCTION()
	void OnPlayButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();
};
