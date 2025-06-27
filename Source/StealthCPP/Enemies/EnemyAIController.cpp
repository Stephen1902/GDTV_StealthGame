// Copyright 2025 DME Games

#include "Enemies/EnemyAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "SensorsAlarms/Alarm.h"


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
			if (ShoutSoundToPlay && !BehaviorTreeComponent->GetBlackboardComponent()->GetValueAsBool(FName("PlayerSeen")))
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShoutSoundToPlay, GetPerceptionComponent()->GetOwner()->GetActorLocation());
			}

			BehaviorTreeComponent->GetBlackboardComponent()->SetValueAsBool(FName("PlayerSeen"), true);

			GetWorld()->GetTimerManager().SetTimer(ShoutTimerHandle, this, &AEnemyAIController::ShoutTimerFinished, 0.2f, false, 0.2f);
		}
	}
}

void AEnemyAIController::AlarmHasBeenTriggered()
{
	BehaviorTreeComponent->GetBlackboardComponent()->SetValueAsBool(FName("PlayerSeen"), true);
}

void AEnemyAIController::ShoutTimerFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(ShoutTimerHandle);
	
	if (AAlarm* AlarmActor = Cast<AAlarm>(UGameplayStatics::GetActorOfClass(GetWorld(), AAlarm::StaticClass())))
	{
		AlarmActor->StartAlarm();
	}
}
