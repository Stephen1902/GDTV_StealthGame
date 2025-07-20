// Copyright 2025 DME Games

#include "Player/PlayerHighScores.h"

void UPlayerHighScores::SetNewScore(const FText NewScore)
{
	SavedScores.Add(NewScore);

}
