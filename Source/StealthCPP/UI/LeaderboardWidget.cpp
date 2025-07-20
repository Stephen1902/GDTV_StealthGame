// Copyright 2025 DME Games


#include "UI/LeaderboardWidget.h"

#include "Components/TextBlock.h"

void ULeaderboardWidget::SetLeaderboardText(FText TextIn) const
{
	TB_PlayerTime->SetText(TextIn);
}
