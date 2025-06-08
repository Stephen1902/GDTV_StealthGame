// Copyright 2025 DME Games

#include "SensorsAlarms/SensorBase.h"

#include "Alarm.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASensorBase::ASensorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("Root Comp");
	SetRootComponent(RootComp);

	PointLightComp = CreateDefaultSubobject<UPointLightComponent>("Point Light Comp");
	PointLightComp->SetupAttachment(RootComp);
	PointLightComp->Intensity = 100000.f;
	PointLightComp->SetLightColor(FColor::Red);

	bHasBeenTriggered = false;

	static ConstructorHelpers::FObjectFinder<USoundAttenuation> SoundAtt(TEXT("/Game/Audio/MetaSounds/SA_Sensor"));
	if (SoundAtt.Succeeded())
	{
		SoundAttenuation = SoundAtt.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> FoundCurve(TEXT("/Game/Alarms_Sensors/Sensors/BP_SensorCurve"));
	if (FoundCurve.Succeeded())
	{
		FloatCurve = FoundCurve.Object;

		FlashTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Comp"));
	
		//Bind the Callback function for the float return value
		InterpFunction.BindUFunction(this, FName{ TEXT("TimelineFloatReturn") });
	}
	
}

void ASensorBase::BeginPlay()
{
	Super::BeginPlay();

	if (FlashTimeline)
	{
		//Add the float curve to the timeline and connect it to the interp function
		FlashTimeline->AddInterpFloat(FloatCurve, InterpFunction, FName{ TEXT("Flash Timeline") });
	}
}

void ASensorBase::DetectPlayer(AActor* ActorDetected)
{
	// Check if the detected actor is the player and if the sensor has not been activated before
	if (ActorDetected->ActorHasTag(FName("Player")) && !bHasBeenTriggered)
	{
		bHasBeenTriggered = true;

		// Play a sound to alert the player this sensor has been triggered
		if (SoundOnTriggered && SoundAttenuation)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundOnTriggered, GetActorLocation(), 1.f, 1.f, 0.f, SoundAttenuation);
		}

		if (AAlarm* FoundActor = Cast<AAlarm>(UGameplayStatics::GetActorOfClass(GetWorld(), AAlarm::StaticClass())))
		{
			FoundActor->StartAlarm();
		}

		if (FlashTimeline)
		{
			FlashTimeline->Play();
		}
	}
}

void ASensorBase::TimelineFloatReturn(float Val)
{
	const FString ValAsString = FString::SanitizeFloat(Val);
	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, *ValAsString);
}

