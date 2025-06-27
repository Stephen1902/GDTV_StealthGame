// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BT_MoveThroughWaypoints.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API UBT_MoveThroughWaypoints : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector NextWaypointKey;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	int32 NextWaypoint = 0;

private:
	UPROPERTY()
	class UWaypointComponent* WaypointComponent;

	TArray<class AWaypointActor*> Waypoints;
	
	void StartMoveTo();
};
