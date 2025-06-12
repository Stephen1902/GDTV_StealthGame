// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Framework/InteractInterface.h"
#include "SensorsAlarms/SensorBase.h"
#include "SensorBar.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API ASensorBar : public ASensorBase, public IInteractInterface
{
	GENERATED_BODY()

	ASensorBar();
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	TObjectPtr<class UBoxComponent> CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	TObjectPtr<UStaticMeshComponent> MeshComp;

private:
	UFUNCTION()
	void OnBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void Interact_Implementation() override;
};
