// Copyright 2025 DME Games

#include "SensorsAlarms/SensorBase.h"

#include "Components/PointLightComponent.h"

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

}

void ASensorBase::DetectPlayer(AActor* ActorDetected)
{
	
}

