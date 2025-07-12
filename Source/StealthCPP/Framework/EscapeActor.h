// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeActor.generated.h"

UCLASS()
class STEALTHCPP_API AEscapeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEscapeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, Category = "Escape Actor")
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(EditInstanceOnly, Category = "Escape Actor")
	TObjectPtr<class UBoxComponent> CollisionComp;

	UPROPERTY(EditInstanceOnly, Category = "Escape Actor")
	TSubclassOf<class UEscapedWidget> EscapedWidgetToShow;
private:
	UFUNCTION()
	void OnCollisionBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
