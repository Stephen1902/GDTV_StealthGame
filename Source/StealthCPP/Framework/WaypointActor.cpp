// Copyright 2025 DME Games


#include "Framework/WaypointActor.h"

#include "Components/ArrowComponent.h"

// Sets default values
AWaypointActor::AWaypointActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);
		
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Comp"));
	ArrowComp->SetupAttachment(RootComp);
	ArrowComp->SetRelativeScale3D(FVector(2.0f));
	ArrowComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
}
