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

	UFUNCTION()
	void StartAlarm();
	UFUNCTION()
	void StopAlarm();

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
	UPROPERTY()
	TObjectPtr<UAudioComponent> SoundBeingPlayed;
	
	bool bIsAlarmSounding;

	void TriggerAllGuards();
	
};
