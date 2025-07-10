// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Framework/InteractInterface.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Framework/GuardInterface.h"
#include "StealthCharacter.generated.h"

class UPauseMenuWidget;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class STEALTHCPP_API AStealthCharacter : public ACharacter, public IInteractInterface, public IGuardInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStealthCharacter();

	UFUNCTION(BlueprintCallable, Category = "Player")
	bool GetPlayerCrouching() const	{ return bIsCrouching; }

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TObjectPtr<UAnimMontage> TakeDownMontageToPlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TObjectPtr<UAnimMontage> RollMontageToPlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TObjectPtr<UAnimMontage> MantleMontageToPlay1m;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TObjectPtr<UAnimMontage> MantleMontageToPlay2m;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TObjectPtr<class UAIPerceptionStimuliSourceComponent> StimuliSourceComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	UCurveFloat* FloatCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TSubclassOf<class UDetectionWidget> DetectionWidgetToDisplay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetToDisplay;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeRollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MantleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseMenuAction;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetWidgetPercent(float CurrentPercent);

	// Public so can be called from the Pause Menu Widget class;
	void TogglePauseMenu();

private:
	UFUNCTION()
	void TryToInteract();

	bool bIsCrouching;
	UFUNCTION()
	void ToggleCrouch();

	bool bIsDodging;
	UFUNCTION()
	void DodgeRoll();
	UFUNCTION()
	void MontageHasFinished(UAnimMontage* Montage, bool bInterrupted);

	bool bIsMantling;
	UFUNCTION()
	void TryMantleClimb();

	bool bGameIsPaused;
	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenuRef;
	
	float StandardCameraPos;
	float CrouchCameraPos;
	
	UPROPERTY()
	UTimelineComponent* CameraTimeline;

	FOnTimelineFloat CameraInterpFunction{};
	
	UFUNCTION()
	void TimelineFloatReturn(float Val);

	bool CheckIfCanUncrouch();

	UPROPERTY()
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComp;

	TObjectPtr<UDetectionWidget> DetectionWidgetRef;
};
