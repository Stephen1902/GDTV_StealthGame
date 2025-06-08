// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Alarm.generated.h"

class USpotLightComponent;
class URotatingMovementComponent;

UCLASS()
class STEALTHCPP_API AAlarm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlarm();

protected:
	virtual void BeginPlay() override;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm")
	TObjectPtr<USpotLightComponent> SpotLightOne;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm")
	TObjectPtr<USpotLightComponent> SpotLightTwo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm")
	TObjectPtr<URotatingMovementComponent> RotatingMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm")
	TObjectPtr<USoundBase> AlarmSoundToPlay;
private:
	UFUNCTION()
	void StartAlarm();
	UFUNCTION()
	void StopAlarm();

	UPROPERTY()
	TObjectPtr<UAudioComponent> SoundBeingPlayed;
	
	bool bIsAlarmSounding;

	/** To be removed */
	FTimerHandle AlarmTimerHandle;
};
