// Copyright 2025 DME Games

#include "Enemies/EnemyGuard.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyGuard::AEnemyGuard()
{
	// Default settings are placed here
	
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Needs to be true to ensure animation works correctly.
	GetCharacterMovement()->bRequestedMoveUseAcceleration = true;

	// These settings ensure smooth animation
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	// Default walk and chasing speed for this enemy
	WalkingSpeed = 150.f;
	ChasingSpeed = 500.f;

	bIsAttacking = false;
}
void AEnemyGuard::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;

	AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AEnemyGuard::AnimMontageHasEnded);
	}
}

void AEnemyGuard::AnimMontageHasEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
}

void AEnemyGuard::MakeGuardRun_Implementation()
{
	Super::MakeGuardRun_Implementation();

	if (GetCharacterMovement()->MaxWalkSpeed != ChasingSpeed)
	{
		GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Guard Runs"));
	}
}

void AEnemyGuard::MakeGuardCatch_Implementation()
{
	Super::MakeGuardCatch_Implementation();

	if (CatchMontageToPlay && AnimInstance && !bIsAttacking)
	{
		bIsAttacking = true;
		AnimInstance->Montage_Play(CatchMontageToPlay, 1.0f);
	}
}
