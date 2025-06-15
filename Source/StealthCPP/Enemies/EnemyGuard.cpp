// Copyright 2025 DME Games

#include "Enemies/EnemyGuard.h"

#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

AEnemyGuard::AEnemyGuard()
{
	// Default settings are placed here
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("Behaviour Tree Comp");
	
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
void AEnemyGuard::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTreeComponent && BehaviorTreeToRun)
	{
		BehaviorTreeComponent->StartTree(*BehaviorTreeToRun);
	}
}
