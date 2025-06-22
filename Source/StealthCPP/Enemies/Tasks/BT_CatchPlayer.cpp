// Copyright 2025 DME Games


#include "Enemies/Tasks/BT_CatchPlayer.h"

#include "AIController.h"
#include "Framework/GuardInterface.h"
#include "Kismet/GameplayStatics.h"

class UGuardInterface;

EBTNodeResult::Type UBT_CatchPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (AIOwner)
	{
		// Check if the guard implements an interface, get them to catch the player if they do
		if (AIOwner->GetPawn()->Implements<UGuardInterface>())
		{
			IGuardInterface::Execute_MakeGuardCatch(AIOwner->GetPawn());
		}
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
