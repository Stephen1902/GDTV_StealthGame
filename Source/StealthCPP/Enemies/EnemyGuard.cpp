// Copyright 2025 DME Games

#include "Enemies/EnemyGuard.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AEnemyGuard::AnimNotify);
	}
}

void AEnemyGuard::AnimMontageHasEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
}

void AEnemyGuard::AnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	const FVector StartLocation = GetActorLocation();
	const FVector EndLocation = (GetActorForwardVector() * 128.f) + StartLocation;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToTraceFor;
	ObjectsToTraceFor.Add(ObjectTypeQuery3);
	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(this);
	FHitResult HitResult;
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartLocation, EndLocation, 32.f, ObjectsToTraceFor, false, ActorToIgnore, EDrawDebugTrace::ForDuration, HitResult, true))
	{
		if (HitResult.GetActor()->ActorHasTag(FName("Player")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Got the player"));
		}
	}
	
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
