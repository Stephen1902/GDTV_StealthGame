// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

class UMainMenuWidget;
/**
 * 
 */
UCLASS()
class STEALTHCPP_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	AMainMenuGameMode();
	
	// Called on game start
	virtual void BeginPlay() override;
	
	
private:
	TSubclassOf<UMainMenuWidget> MainMenuWidget;

	UPROPERTY()
	TObjectPtr<USoundBase> MainMenuMusic;
};
