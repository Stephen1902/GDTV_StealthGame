// Copyright 2025 DME Games


#include "SensorsAlarms/SensorBar.h"

#include "Components/BoxComponent.h"

ASensorBar::ASensorBar()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>("Collision Comp");
	CollisionComp->SetupAttachment(RootComp);
	CollisionComp->SetBoxExtent(FVector(5.0f, 150.f, 130.f));

	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("Mesh Comp");
	MeshComp->SetupAttachment(RootComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshComp->SetRelativeLocation(FVector(0.f, 0.f, -25.f));
	
}

void ASensorBar::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASensorBar::ASensorBar::OnBoxOverlapped);
}

void ASensorBar::OnBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	DetectPlayer(OtherActor);
}

void ASensorBar::Interact_Implementation()
{
	IInteractInterface::Interact_Implementation();

	if (bIsActive)
	{
		bIsActive = false;
		MeshComp->SetVisibility(false);
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		bIsActive = true;
		MeshComp->SetVisibility(true);
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}
