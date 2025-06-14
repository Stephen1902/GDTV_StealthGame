// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Framework/GuardInterface.h"
#include "GameFramework/Character.h"
#include "EnemyCharacterBase.generated.h"

UCLASS()
class STEALTHCPP_API AEnemyCharacterBase : public ACharacter, public IGuardInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The radius, behind the enemy, at which the player can take down this enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Base")
	TObjectPtr<class USphereComponent> TakeDownRadius;

	// A false skeletal mesh to give the player a location for taking down this enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Base")
	TObjectPtr<USkeletalMeshComponent> RefMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Base")
	TObjectPtr<class UWidgetComponent> WidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Base")
	bool bCanBeTakenDown;
	
	// Animation Montage to play when taken down
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Base")
	TObjectPtr<UAnimMontage> TakeDownMontage;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void CanTakeDown_Implementation(FVector& ActorLocation, FRotator& ActorRotation, bool &CanTakeDown, AActor*& IgnoredActor) override;

	FTimerHandle RagdollDelayTimer;
	UFUNCTION()
	void RagdollEnemy();

private:
	UFUNCTION()
	void OnTakeDownOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnTakeDownOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
