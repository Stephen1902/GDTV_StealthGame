// Copyright 2025 DME Games

#include "Enemies/EnemyGuard.h"
#include "EnemyAIController.h"
#include "WaypointComponent.h"
#include "Framework/StealthGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AEnemyGuard::AEnemyGuard()
{
	// Default settings are placed here

	WaypointComponent = CreateDefaultSubobject<UWaypointComponent>(TEXT("Waypoint Comp"));
	
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
	bHasAttacked = false;
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
		if (HitResult.GetActor()->ActorHasTag(FName("Player")) && !bHasAttacked)
		{
			if (AStealthGameMode* StealthGameMode = Cast<AStealthGameMode>(GetWorld()->GetAuthGameMode()))
			{
				bHasAttacked = true;
				StealthGameMode->PlayerHasBeenCaught();
			}
		}
	}
}

void AEnemyGuard::MakeGuardRun_Implementation()
{
	Super::MakeGuardRun_Implementation();

	if (GetCharacterMovement()->MaxWalkSpeed != ChasingSpeed)
	{
		GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
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

void AEnemyGuard::CanTakeDown_Implementation(FVector& ActorLocation, FRotator& ActorRotation, bool& CanTakeDown, AActor*& IgnoredActor)
{
	Super::CanTakeDown_Implementation(ActorLocation, ActorRotation, CanTakeDown, IgnoredActor);

	if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController()))
	{
		EnemyAIController->SetIsDead();
	}
}

