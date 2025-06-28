// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "SensorsAlarms/SensorBase.h"
#include "SensorRotative.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHCPP_API ASensorRotative : public ASensorBase
{
	GENERATED_BODY()

	ASensorRotative();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	TObjectPtr<UStaticMeshComponent> WallMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	TObjectPtr<UStaticMeshComponent> HeadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	TObjectPtr<class USpotLightComponent> SpotLightComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	TObjectPtr<UStaticMeshComponent> DetectionMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	FRotator ForwardRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	FRotator BackwardRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensors")
	float TimeToFillDetectionGauge;
private:
	UFUNCTION()
	void OnMeshOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	UTimelineComponent* RotateTimeline;

	FOnTimelineFloat RotateFunction{};
	FOnTimelineEventStatic EndOfRotateFunction{};
	
	UFUNCTION()
	void TimelineRotateReturn(float Val);

	UFUNCTION()
	void EndOfRotation();

	bool bTimelineForward;

	UPROPERTY()
	class AStealthCharacter* PlayerRef;
	bool bPlayerInZone;
	FTimerHandle PlayerInZoneTimer;
	UFUNCTION()
	void PlayerInZone(AActor* OtherActor);
	UFUNCTION()
	void PlayerOutOfZone();
	void UpdateProgressBar();
	float TimeInZone;
};
