// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaypointComponent.generated.h"

class AWaypointActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHCPP_API UWaypointComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWaypointComponent();

	TArray<AWaypointActor*> GetWaypointActors() const { return WaypointActors; }
	
protected:
	UPROPERTY(EditAnywhere, Category = "Waypoint Component")
	TArray<AWaypointActor*> WaypointActors;

		
};
