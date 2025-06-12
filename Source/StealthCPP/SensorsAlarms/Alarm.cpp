// Copyright 2025 DME Games


#include "SensorsAlarms/Alarm.h"

#include "Components/AudioComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAlarm::AAlarm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Comp");
	MeshComp->SetupAttachment(RootComp);

	SpotLightOne = CreateDefaultSubobject<USpotLightComponent>("Spotlight One");
	SpotLightOne->SetupAttachment(MeshComp);
	SpotLightOne->Intensity = 100000.f;
	SpotLightOne->LightColor = FColor::Red;

	SpotLightTwo = CreateDefaultSubobject<USpotLightComponent>("Spotlight Two");
	SpotLightTwo->SetupAttachment(MeshComp);
	SpotLightTwo->Intensity = 100000.f;
	SpotLightTwo->LightColor = FColor::Red;

	RotatingMovementComp = CreateDefaultSubobject<URotatingMovementComponent>("Rotating Movement");
	RotatingMovementComp->RotationRate = FRotator::ZeroRotator;

	bIsAlarmSounding = false;
}

void AAlarm::BeginPlay()
{
	Super::BeginPlay();
}

void AAlarm::StartAlarm()
{
	if (!bIsAlarmSounding && AlarmSoundToPlay && RotatingMovementComp)
	{
		SoundBeingPlayed = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), AlarmSoundToPlay, GetActorLocation());
		bIsAlarmSounding = true;
		RotatingMovementComp->RotationRate = FRotator(0.f, 180.f, 0.f);
		SpotLightOne->SetVisibility(true);
		SpotLightTwo->SetVisibility(true);
	}
}

void AAlarm::StopAlarm()
{
	if (bIsAlarmSounding && RotatingMovementComp)
	{
		SoundBeingPlayed->Stop();
		bIsAlarmSounding = false;
		RotatingMovementComp->RotationRate = FRotator(0.f, 0.f, 0.f);
		SpotLightOne->SetVisibility(false);
		SpotLightTwo->SetVisibility(false);
	}
}
