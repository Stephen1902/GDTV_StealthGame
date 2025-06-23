// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	AEnemyAIController();
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard Controller")
	TObjectPtr<class UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard Controller")
	TObjectPtr<UBehaviorTree> BehaviorTreeToRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard Controller")
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guard Controller")
	class UAISenseConfig_Sight* SightConfig;

//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Base")
//	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Base")
//	TObjectPtr<UBlackboardData> BehaviorTreeData;

private:
	UFUNCTION()
	void PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
