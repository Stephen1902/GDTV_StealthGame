// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GuardInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGuardInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STEALTHCPP_API IGuardInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	void CanTakeDown(FVector& ActorLocation, FRotator& ActorRotation, bool& CanTakeDown, AActor*& IgnoredActor);
};
