// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "SensorBase.generated.h"

UCLASS()
class STEALTHCPP_API ASensorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASensorBase();

protected:
	// Called at Game Start
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	TObjectPtr<class UPointLightComponent> PointLightComp;

	// The sound to play when this sensor has been triggered
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	TObjectPtr<USoundBase> SoundOnTriggered; 
	
	// The attenuation settings for the sound being triggered
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	TObjectPtr<USoundAttenuation> SoundAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	UCurveFloat* FloatCurve;

	bool bIsActive = true;
public:	
	virtual void DetectPlayer(AActor* ActorDetected);

private:
	bool bHasBeenTriggered;

	UPROPERTY()
	UTimelineComponent* FlashTimeline;

	FOnTimelineFloat InterpFunction{};
	
	UFUNCTION()
	void TimelineFloatReturn(float Val);

	UPROPERTY()
	FTimerHandle FakeTimerHandle;
	
};
