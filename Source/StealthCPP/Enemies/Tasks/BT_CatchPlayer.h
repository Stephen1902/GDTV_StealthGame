// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BT_CatchPlayer.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API UBT_CatchPlayer : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
