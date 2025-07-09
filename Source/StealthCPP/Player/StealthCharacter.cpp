// Copyright 2025 DME Games

#include "Player/StealthCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "Blueprint/UserWidget.h"
#include "Framework/GuardInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "UI/DetectionWidget.h"

// Sets default values
AStealthCharacter::AStealthCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Default location and rotation for the mesh so it display correctly in the BP
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -89.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// Create the Spring Arm Component and attach it to the Capsule
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->TargetArmLength = 600.f;
	SpringArmComp->bUsePawnControlRotation = true;

	// Create the Camera Component and attach it to the Spring Arm
	CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComp->SetupAttachment(SpringArmComp);

	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping Comp"));

	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus Source Comp"));

	this->bUseControllerRotationYaw = false;

    if (UCharacterMovementComponent* const MovementComponent = GetCharacterMovement())
	{
		MovementComponent->bOrientRotationToMovement = true;
		MovementComponent->bUseControllerDesiredRotation = false;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> FoundCurve(TEXT("/Game/Player/TL_CameraMovement"));
	if (FoundCurve.Succeeded())
	{
		FloatCurve = FoundCurve.Object;

		CameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Comp"));
				
		//Bind the Callback function for the float return value
		CameraInterpFunction.BindUFunction(this, FName{ TEXT("TimelineFloatReturn") });
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FoundMontage(TEXT("/Game/Player/Animations/Takedown/Takedown_Attacker_Montage"));
	if (FoundMontage.Succeeded())
	{
		TakeDownMontageToPlay = FoundMontage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RollMontage(TEXT("/Game/Player/Animations/Roll/Roll_Montage"));
	if (RollMontage.Succeeded())
	{
		RollMontageToPlay = RollMontage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MantleMontage1m(TEXT("/Game/Player/Animations/Mantle/Mantling_1M_Montage"));
	if (MantleMontage1m.Succeeded())
	{
		MantleMontageToPlay1m = MantleMontage1m.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MantleMontage2m(TEXT("/Game/Player/Animations/Mantle/Mantling_2m_Montage"));
	if (MantleMontage2m.Succeeded())
	{
		MantleMontageToPlay2m = MantleMontage2m.Object;
	}

	static ConstructorHelpers::FClassFinder<UDetectionWidget> DetectionFound(TEXT("/Game/UI/WBP_Detection"));
	if (DetectionFound.Succeeded())
	{
		DetectionWidgetToDisplay = DetectionFound.Class;
	}
	
	Tags.Add(FName("Player"));

	bIsCrouching = false;
	bIsDodging = false;
	bIsMantling = false;
}

void AStealthCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AStealthCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Called when the game starts or when spawned
void AStealthCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		CameraManager->ViewPitchMax = 0.f;
		CameraManager->ViewPitchMin = -60.f;
	}

	StandardCameraPos = SpringArmComp->TargetArmLength;
	CrouchCameraPos = StandardCameraPos + 150.f;

	if (CameraTimeline)
	{
		CameraTimeline->AddInterpFloat(FloatCurve, CameraInterpFunction, FName{ TEXT("Camera Timeline")});
		CameraTimeline->SetLooping(false);
	}

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AStealthCharacter::MontageHasFinished);
	}
	
	StimuliSourceComponent->RegisterWithPerceptionSystem();
	StimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());

	if (DetectionWidgetToDisplay)
	{
		DetectionWidgetRef = CreateWidget<UDetectionWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), DetectionWidgetToDisplay);
		DetectionWidgetRef->AddToViewport();
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
	
}

// Called every frame
void AStealthCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStealthCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* InputPlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(InputPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AStealthCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AStealthCharacter::Look);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AStealthCharacter::TryToInteract);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AStealthCharacter::ToggleCrouch);

		// Dodge
		EnhancedInputComponent->BindAction(DodgeRollAction, ETriggerEvent::Started, this, &AStealthCharacter::DodgeRoll);

		// Mantle
		EnhancedInputComponent->BindAction(MantleAction, ETriggerEvent::Started, this, &AStealthCharacter::TryMantleClimb);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AStealthCharacter::SetWidgetPercent(float CurrentPercent)
{
	if (DetectionWidgetRef)
	{
		DetectionWidgetRef->UpdateProgressPercent(CurrentPercent);
	}
}

void AStealthCharacter::TryToInteract()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	if (OverlappingActors.Num() > 0)
	{
		
		for (AActor* It : OverlappingActors)
		{
			if (It->Implements<UInteractInterface>())
			{
				Execute_Interact(It);
			}

			if (It->Implements<UGuardInterface>())
			{
				FVector GuardLocation;
				FRotator GuardRotation;
				bool CanTakeDownGuard;
				AActor* ActorToIgnore;
				Execute_CanTakeDown(It, GuardLocation, GuardRotation, CanTakeDownGuard, ActorToIgnore);

				if (CanTakeDownGuard)
				{
					if (TakeDownMontageToPlay)
					{
						if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
						{
							AnimInstance->Montage_Play(TakeDownMontageToPlay, 1.0f);

							FMotionWarpingTarget MotionWarpingTarget;
							MotionWarpingTarget.Name = FName("TakeDown");
							MotionWarpingTarget.Location = GuardLocation;
							MotionWarpingTarget.Rotation = GuardRotation;
							MotionWarpingComp->AddOrUpdateWarpTarget(MotionWarpingTarget);

							GetCapsuleComponent()->IgnoreActorWhenMoving(ActorToIgnore, true);
							GetMesh()->IgnoreActorWhenMoving(ActorToIgnore, true);
						}
						
					}
					
				}
			}
		}
	}
}

void AStealthCharacter::ToggleCrouch()
{
	if (!bIsCrouching)
	{
		bIsCrouching = true;
		if (UCharacterMovementComponent* CharMovement = GetCharacterMovement())
		{
			CharMovement->MaxWalkSpeed = 350.f;
		}

		if (CameraTimeline)
		{
			CameraTimeline->PlayFromStart();
		}
		else
		{
			SpringArmComp->TargetArmLength = CrouchCameraPos;
		}
	}
	else
	{
		if (!CheckIfCanUncrouch())
		{
			bIsCrouching = false;
			if (UCharacterMovementComponent* CharMovement = GetCharacterMovement())
			{
				CharMovement->MaxWalkSpeed = 600.f;
			}

			if (CameraTimeline)
			{
				CameraTimeline->ReverseFromEnd();
			}
			else
			{
				SpringArmComp->TargetArmLength = StandardCameraPos;
			}
		}
	}
}

void AStealthCharacter::DodgeRoll()
{
	if (RollMontageToPlay && !bIsDodging)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			bIsDodging = true;
			AnimInstance->Montage_Play(RollMontageToPlay, 1.0f);
		}
	}

}

void AStealthCharacter::MontageHasFinished(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == RollMontageToPlay)
	{
		bIsDodging = false;
	}

	if (Montage == MantleMontageToPlay1m || Montage == MantleMontageToPlay2m)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		bIsMantling = false;

		if (bIsCrouching && CameraTimeline)
		{
			CameraTimeline->PlayFromStart();
		}
	}
}

void AStealthCharacter::TryMantleClimb()
{
	// The player should only be able to mantle if on the ground and if they're not already mantling
	if (!GetCharacterMovement()->IsFalling() && !bIsMantling)
	{
		FHitResult HitResult;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);

		// If the player is crouching, check higher up the wall to be able to climb up it
		int32 NumberOfSteps = bIsCrouching ? 7 : 4;
		
		for (int32 i = 0; i <= NumberOfSteps; ++i)
		{
			FVector StartLoc = GetActorLocation();
			StartLoc.Z += i * 30.f;
			FVector EndLoc = (GetActorForwardVector() * 120.f) + StartLoc;
			// Do a line trace
			if (!UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLoc, EndLoc, 8.f, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true))
			{
				if (i > 0)
				{
					for (int32 j = 0; j <= 4; ++j)
					{
						StartLoc = GetActorLocation();
						StartLoc.X += ((j * 20.f) * GetActorForwardVector().X);
						StartLoc.Y += (j * 20.f) * GetActorForwardVector().Y;
						StartLoc.Z += 200.f;
						EndLoc = (GetActorUpVector() * -200.f) + StartLoc;
						if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLoc, EndLoc, 8.f, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true))
						{
							if (MotionWarpingComp)
							{
								FMotionWarpingTarget MotionWarpingTarget;
								MotionWarpingTarget.Name = FName("Mantle");
								MotionWarpingTarget.Location = HitResult.Location;
								MotionWarpingTarget.Location.Z -= 3.0f;
								MotionWarpingTarget.Rotation = GetActorRotation();
								MotionWarpingComp->AddOrUpdateWarpTarget(MotionWarpingTarget);

								// Check whether the height of the object to climb is more than 2 metres
								if (UKismetMathLibrary::Vector_Distance(HitResult.Location, GetActorLocation()) > 200.f)
								{
									if (MantleMontageToPlay2m)
									{
										PlayAnimMontage(MantleMontageToPlay2m);
									}
								}
								else
								{
									if (MantleMontageToPlay1m)
									{
										PlayAnimMontage(MantleMontageToPlay1m);
									}
								}

								GetCharacterMovement()->SetMovementMode(MOVE_Flying);
								bIsMantling = true;

								// If crouched, make the player stand
								if (bIsCrouching)
								{
									GetCapsuleComponent()->SetCapsuleHalfHeight(88.f, true);
									GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -89.f));
								}
							}
							
							break;
						}
					}
				}
				break;
			}
		}
	}
}

void AStealthCharacter::TimelineFloatReturn(float Val)
{
	SpringArmComp->TargetArmLength = UKismetMathLibrary::Lerp(StandardCameraPos, CrouchCameraPos, Val);
	GetCapsuleComponent()->SetCapsuleHalfHeight(UKismetMathLibrary::Lerp(88.f, 48.f, Val));
	GetMesh()->SetRelativeLocation(UKismetMathLibrary::VLerp(FVector(0.f, 0.f, -89.f), FVector(0.f, 0.f, -49.f), Val));
}

bool AStealthCharacter::CheckIfCanUncrouch()
{
	FHitResult HitResult;
	const FVector StartLoc = GetActorLocation();
	const FVector EndLoc = (GetActorUpVector() * 115.f) + StartLoc;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	return UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLoc, EndLoc, 32.f, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);
}

