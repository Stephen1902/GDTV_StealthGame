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

	// Default speed for this enemy when patrolling / walking
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Guard")
	float WalkingSpeed;

	// Speed when chasing a player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Guard")
	float ChasingSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Guard")
	TObjectPtr<UAnimMontage> CatchMontageToPlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Guard")
	class UWaypointComponent* WaypointComponent;

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;

	UFUNCTION()
	void AnimMontageHasEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void AnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	bool bIsAttacking;
	bool bHasAttacked;
	
	UFUNCTION()
	virtual void MakeGuardRun_Implementation() override;

	UFUNCTION()
	virtual void MakeGuardCatch_Implementation() override;


};
