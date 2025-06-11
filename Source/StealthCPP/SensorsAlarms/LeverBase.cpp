// Copyright 2025 DME Games

#include "SensorsAlarms/LeverBase.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

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
	
	static ConstructorHelpers::FObjectFinder<USoundAttenuation> SoundAtt(TEXT("/Game/Audio/MetaSounds/SA_Sensor"));
	if (SoundAtt.Succeeded())
	{
		SoundAttenuation = SoundAtt.Object;
	}
}

// Called when the game starts or when spawned
void ALeverBase::BeginPlay()
{
	Super::BeginPlay();

	
	

}

// Called every frame
void ALeverBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void ALeverBase::SwitchLever()
{
	// Play a sound to alert the player this lever has been triggered
	if (SoundOnInteract && SoundAttenuation)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundOnInteract, GetActorLocation(), 1.f, 1.f, 0.f, SoundAttenuation);
	}

	if (AnimSequence)
    {
    	SkeletalMeshComp->PlayAnimation(AnimSequence, false);
    }
}

void ALeverBase::Interact_Implementation()
{
	IInteractInterface::Interact_Implementation();

	SwitchLever();
}
