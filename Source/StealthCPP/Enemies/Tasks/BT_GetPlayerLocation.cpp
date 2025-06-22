// Copyright 2025 DME Games

#include "Enemies/Tasks/BT_GetPlayerLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Framework/GuardInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBT_GetPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsObject(PlayerObject.SelectedKeyName, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		
		// Check for an owning controller
		if (AIOwner)
		{
			// Check if the guard implements an interface, get them to run if they do
			if (AIOwner->GetPawn()->Implements<UGuardInterface>())
			{
				IGuardInterface::Execute_MakeGuardRun(AIOwner->GetPawn());
			}
		}
		return EBTNodeResult::Succeeded;
	
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Has a player character.  Does not have a blackboard component.")); 
	
	return EBTNodeResult::Failed;
}
