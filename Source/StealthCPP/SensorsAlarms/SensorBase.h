// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	TObjectPtr<class UPointLightComponent> PointLightComp;

	
public:	
	virtual void DetectPlayer(AActor* ActorDetected);
};
