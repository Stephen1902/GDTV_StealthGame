// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyCharacterBase.h"
#include "EnemyGuard.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API AEnemyGuard : public AEnemyCharacterBase
{
	GENERATED_BODY()

	AEnemyGuard();

protected:
	// Called at game start
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Base")
	TObjectPtr<class UBehaviorTreeComponent> BehaviorTreeComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Base")
	TObjectPtr<class UBehaviorTree> BehaviorTreeToRun;
};
