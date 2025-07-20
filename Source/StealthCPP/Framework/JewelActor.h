// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "JewelActor.generated.h"


UCLASS()
class STEALTHCPP_API AJewelActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJewelActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Jewel Actor")
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(EditAnywhere, Category = "Jewel Actor")
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(EditAnywhere, Category = "Jewel Actor")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Jewel Actor")
	UCurveFloat* FloatCurve;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	virtual void Interact_Implementation() override;

	UFUNCTION()
	void OnSphereOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	FVector StartScale;
	
	UPROPERTY()
	UTimelineComponent* OverlappedTimeline;
	
	FOnTimelineFloat InterpFunction{};
	
	UFUNCTION()
	void TimelineFloatReturn(float Val);

	UFUNCTION()
	void OnTimelineFinished();
};
