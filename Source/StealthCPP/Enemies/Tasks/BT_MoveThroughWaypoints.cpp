// Copyright 2025 DME Games

#include "Enemies/Tasks/BT_MoveThroughWaypoints.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyAIController.h"
#include "Enemies/WaypointComponent.h"
#include "Framework/WaypointActor.h"

EBTNodeResult::Type UBT_MoveThroughWaypoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	// Try to get the waypoint component from the owning character
	if (!WaypointComponent)
	{
		WaypointComponent = Cast<UWaypointComponent>(AIOwner->GetPawn()->GetComponentByClass(UWaypointComponent::StaticClass()));
		if (WaypointComponent)
		{
			Waypoints = WaypointComponent->GetWaypointActors();
		}
	}
	// Check the actor trying to call this has a waypoint component on it and it has waypoints set
	if (WaypointComponent && Waypoints.Num() > 0)
	{		
		NextWaypoint += 1;
		if (NextWaypoint >= Waypoints.Num())
		{
			NextWaypoint = 0;
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(NextWaypointKey.SelectedKeyName, Waypoints[NextWaypoint]);

		return EBTNodeResult::Succeeded;
	}
	// Either there is no waypoint component or no actors have been set	
	return EBTNodeResult::Failed;
}
