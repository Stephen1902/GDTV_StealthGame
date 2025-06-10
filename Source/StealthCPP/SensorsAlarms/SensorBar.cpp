// Copyright 2025 DME Games


#include "SensorsAlarms/SensorBar.h"

#include "Components/BoxComponent.h"

ASensorBar::ASensorBar()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>("Collision Comp");
	CollisionComp->SetupAttachment(RootComp);
	CollisionComp->SetBoxExtent(FVector(5.0f, 150.f, 30.f));

	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("Mesh Comp");
	MeshComp->SetupAttachment(RootComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
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
