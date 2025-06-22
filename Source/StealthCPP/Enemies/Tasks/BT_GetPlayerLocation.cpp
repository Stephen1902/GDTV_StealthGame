// Copyright 2025 DME Games

#include "Enemies/Tasks/BT_GetPlayerLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Framework/GuardInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBT_GetPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/*if (const auto BlackboardComp = OwnerComp.GetBlackboardComponent())
	/*{*/
		//BlackboardComp->SetValueAsVector(PlayerLocation.SelectedKeyName, OwnerComp.GetOwner()->GetActorLocation());
		// Check for an owning controller
		if (AIOwner)
		{
			// Get the player's location and move there.
			AIOwner->MoveToActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0), 80.f);

			// Check if the guard implements an interface, get them to run if they do
			if (AIOwner->GetPawn()->Implements<UGuardInterface>())
			{
				IGuardInterface::Execute_MakeGuardRun(AIOwner->GetPawn());
			}
			return EBTNodeResult::Succeeded;
		}
	/*}*/
	
	UE_LOG(LogTemp, Warning, TEXT("Has a player character.  Does not have a blackboard component.")); 
	
	return EBTNodeResult::Failed;
}
