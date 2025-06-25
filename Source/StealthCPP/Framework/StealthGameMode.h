// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StealthGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API AStealthGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Game Mode|Widgets")
	TSubclassOf<class UCapturedWidget> CapturedWidgetToDisplay;
	
public:
	void CreateCapturedWidget();

private:
	UPROPERTY()
	UCapturedWidget* CapturedWidgetRef;
};
