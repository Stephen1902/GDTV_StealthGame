// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerHighScores.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API UPlayerHighScores : public USaveGame
{
	GENERATED_BODY()

public:
	TArray<FText> GetSavedScores() { return SavedScores; }
	void SetNewScore(const FText NewScore);
private:
	// Anything to be saved must be a UPROPERTY or it won't save
	UPROPERTY()
	TArray<FText> SavedScores;
};
