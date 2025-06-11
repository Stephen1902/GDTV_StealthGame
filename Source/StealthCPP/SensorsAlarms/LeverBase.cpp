// Copyright 2025 DME Games

#include "SensorsAlarms/LeverBase.h"

#include "Components/SphereComponent.h"

// Sets default values
ALeverBase::ALeverBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("Root Comp");
	SetRootComponent(RootComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>("Collision Comp");
	CollisionComp->SetupAttachment(RootComp);
	CollisionComp->SetSphereRadius(60.f);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Comp");
	MeshComp->SetupAttachment(RootComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh Comp");
	SkeletalMeshComp->SetupAttachment(RootComp);

	static ConstructorHelpers::FObjectFinder<UAnimSequence> FoundAnim(TEXT("/Game/Meshes/LeverSimple/LeverSimple_Anim"));
	if (FoundAnim.Succeeded())
	{
		AnimSequence = FoundAnim.Object;
	}
}

// Called when the game starts or when spawned
void ALeverBase::BeginPlay()
{
	Super::BeginPlay();

	if (AnimSequence)
	{
		SkeletalMeshComp->PlayAnimation(AnimSequence, false);
	}
}

// Called every frame
void ALeverBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

