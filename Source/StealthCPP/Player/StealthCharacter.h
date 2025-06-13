// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Framework/InteractInterface.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "StealthCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class STEALTHCPP_API AStealthCharacter : public ACharacter, public IInteractInterface
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	UCurveFloat* FloatCurve;

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
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void TryToInteract();

	bool bIsCrouching;
	UFUNCTION()
	void ToggleCrouch();

	float StandardCameraPos;
	float CrouchCameraPos;

	FVector StandardMeshLoc;
	FVector CrouchedMeshLoc;
	
	UPROPERTY()
	UTimelineComponent* CameraTimeline;

	FOnTimelineFloat CameraInterpFunction{};
	
	UFUNCTION()
	void TimelineFloatReturn(float Val);

};
