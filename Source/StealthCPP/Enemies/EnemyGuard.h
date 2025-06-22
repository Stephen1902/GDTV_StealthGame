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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Base")
	float WalkingSpeed;

	// Speed when chasing a player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Base")
	float ChasingSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Base")
	TObjectPtr<UAnimMontage> CatchMontageToPlay;

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;

	UFUNCTION()
	void AnimMontageHasEnded(UAnimMontage* Montage, bool bInterrupted);

	bool bIsAttacking;
	
	UFUNCTION()
	virtual void MakeGuardRun_Implementation() override;

	UFUNCTION()
	virtual void MakeGuardCatch_Implementation() override;
};
