// Copyright 2025 DME Games


#include "Player/StealthCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
	
	Tags.Add(FName("Player"));

	bIsCrouching = false;
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

	StandardMeshLoc = FVector(0.f, 0.f, 0.f);
	CrouchedMeshLoc = FVector(0.f, 0.f, -40.f);

	if (CameraTimeline)
	{
		CameraTimeline->AddInterpFloat(FloatCurve, CameraInterpFunction, FName{ TEXT("Camera Timeline")});
		CameraTimeline->SetLooping(false);
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
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AStealthCharacter::TryToInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying To Interact"));
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	UE_LOG(LogTemp, Warning, TEXT("Found %i overlapping"), OverlappingActors.Num());
	if (OverlappingActors.Num() > 0)
	{
		
		for (AActor* It : OverlappingActors)
		{
			if (It->Implements<UInteractInterface>())
			{
				 IInteractInterface::Execute_Interact(It);
			}
		}
	}
}

void AStealthCharacter::ToggleCrouch()
{
	UE_LOG(LogTemp, Warning, TEXT("ToggleCrouch"));
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

void AStealthCharacter::TimelineFloatReturn(float Val)
{
	SpringArmComp->TargetArmLength = UKismetMathLibrary::Lerp(StandardCameraPos, CrouchCameraPos, Val);
	GetCapsuleComponent()->SetCapsuleHalfHeight(UKismetMathLibrary::Lerp(88.f, 48.f, Val));
	GetMesh()->SetRelativeLocation(UKismetMathLibrary::VLerp(FVector(0.f, 0.f, -89.f), FVector(0.f, 0.f, -49.f), Val));
}

