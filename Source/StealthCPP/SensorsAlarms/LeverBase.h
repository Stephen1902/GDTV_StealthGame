// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeverBase.generated.h"

UCLASS()
class STEALTHCPP_API ALeverBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeverBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Levers")
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Levers")
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Levers")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Levers")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<UAnimSequence> AnimSequence;
};
