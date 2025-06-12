// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Framework/InteractInterface.h"
#include "GameFramework/Actor.h"
#include "LeverBase.generated.h"

UCLASS()
class STEALTHCPP_API ALeverBase : public AActor, public IInteractInterface
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

	// The sound to be played when the player interacts with this lever
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Levers")
	TObjectPtr<USoundBase> SoundOnInteract;

	// Which actor will be affected by this lever when used
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Levers")
	TObjectPtr<AActor> ActorToToggle;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SwitchLever();
private:
	UPROPERTY()
	TObjectPtr<UAnimSequence> AnimSequence;

	UPROPERTY()
	TObjectPtr<USoundAttenuation> SoundAttenuation;
	
	virtual void Interact_Implementation() override;

	bool bCanActivate;
	FTimerHandle ReactivateTimerHandle;
	UFUNCTION()
	void ReactiveTimer();
};
