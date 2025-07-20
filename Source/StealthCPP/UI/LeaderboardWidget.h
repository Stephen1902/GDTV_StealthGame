// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeaderboardWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API ULeaderboardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetLeaderboardText(FText TextIn) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Leaderboard Widget", meta=(BindWidget))
	class UTextBlock* TB_PlayerTime;
};
