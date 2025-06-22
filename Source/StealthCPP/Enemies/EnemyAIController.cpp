// Copyright 2025 DME Games

#include "Enemies/EnemyAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("Behaviour Tree Comp");
	//BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
/*
	if (BlackboardComponent && BehaviorTreeData)
	{
		UseBlackboard(BehaviorTreeData, static_cast<UBlackboardComponent*&>(BlackboardComponent));
	}
*/

	if (BehaviorTreeComponent && BehaviorTreeToRun)
	{
		RunBehaviorTree(BehaviorTreeToRun);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s failed to get a behaviour tree to run."), *GetName());
	}
}
