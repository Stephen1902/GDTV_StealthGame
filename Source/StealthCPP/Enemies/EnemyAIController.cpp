// Copyright 2025 DME Games

#include "Enemies/EnemyAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"


AEnemyAIController::AEnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("Behaviour Tree Comp");
	//BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("Perception Comp");
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config Comp"));
	//Set default values
	SightConfig->SightRadius = 2000.f;
	SightConfig->LoseSightRadius = 300.f;
	SightConfig->PeripheralVisionAngleDegrees = 45.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
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

	if (PerceptionComp)
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::PerceptionUpdated);
	}
}

void AEnemyAIController::PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// Check if what has been sensed is has sight stimulus response
	if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Sight::StaticClass())
	{
		// Check if the actor sense was successful
		if (Stimulus.WasSuccessfullySensed())
		{
			BehaviorTreeComponent->GetBlackboardComponent()->SetValueAsBool(FName("PlayerSeen"), true);
		}
	}
}
