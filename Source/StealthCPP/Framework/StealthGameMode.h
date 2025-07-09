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
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Mode|Widgets")
	TSubclassOf<class UCapturedWidget> CapturedWidgetToDisplay;

	// Music to play when the player is roaming
	UPROPERTY(EditDefaultsOnly, Category = "Game Music")
	TObjectPtr<USoundBase> StealthMusic;

	// Music to play when a guard has seen the player and the player is trying to escape
	UPROPERTY(EditDefaultsOnly, Category = "Game Music")
	TObjectPtr<USoundBase> EscapeMusic;

	UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
	TObjectPtr<USoundBase> RainSoundEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
	TObjectPtr<USoundBase> CaughtSoundEffect;
	
	// Called at Game Start
	virtual void BeginPlay() override;
public:
	void PlayerHasBeenCaught();

	void SwitchToEscapeMusic();

private:
	UPROPERTY()
	UCapturedWidget* CapturedWidgetRef;

	UPROPERTY()
	UAudioComponent* StealthMusicComp;

	UPROPERTY()
	UAudioComponent* EscapeMusicComp;

	UPROPERTY()
	UAudioComponent* RainSoundComp;
};
