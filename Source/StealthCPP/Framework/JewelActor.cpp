// Copyright 2025 DME Games

#include "Framework/JewelActor.h"

#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/StealthCharacter.h"

// Sets default values
AJewelActor::AJewelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Comp"));
	CollisionComp->SetupAttachment(RootComp);
	CollisionComp->SetSphereRadius(72.f);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Overlap);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComponent->SetupAttachment(RootComp);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> FoundCurve(TEXT("/Game/Player/TL_CameraMovement"));
	if (FoundCurve.Succeeded())
	{
		FloatCurve = FoundCurve.Object;

		OverlappedTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Comp"));		
	}
}

// Called when the game starts or when spawned
void AJewelActor::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AJewelActor::OnSphereOverlapped);

	if (OverlappedTimeline)
	{
		// Bind the Callback function for the float return value
		InterpFunction.BindUFunction(this, FName{ TEXT("TimelineFloatReturn") });
		//Add the float curve to the timeline and connect it to the interp function
		OverlappedTimeline->AddInterpFloat(FloatCurve, InterpFunction, FName{ TEXT("Flash Timeline") });
		OverlappedTimeline->SetLooping(false);

		FOnTimelineEventStatic TimelineFinishedCallback;
		// Bind the callback function for when the timeline finishes
		TimelineFinishedCallback.BindUFunction(this, FName("OnTimelineFinished"));
		// Set the function to be called when the timeline finishes
		OverlappedTimeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
	}

	StartScale = MeshComponent->GetRelativeScale3D();
}

// Called every frame
void AJewelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJewelActor::Interact_Implementation()
{
	IInteractInterface::Interact_Implementation();
}

void AJewelActor::OnSphereOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedTimeline)
	{
		OverlappedTimeline->Play();
	}
}

void AJewelActor::TimelineFloatReturn(float Val)
{
	const float XThisTurn = UKismetMathLibrary::Lerp(StartScale.X, 0.01f, Val);
	const float YThisTurn = UKismetMathLibrary::Lerp(StartScale.Y, 0.01f, Val);
	const float ZThisTurn = UKismetMathLibrary::Lerp(StartScale.Z, 0.01f, Val);
	MeshComponent->SetRelativeScale3D(FVector(XThisTurn, YThisTurn, ZThisTurn));	
}

void AJewelActor::OnTimelineFinished()
{
	//  Attach the jewel to the player
	if (AStealthCharacter* PlayerChar = Cast<AStealthCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		PlayerChar->EquipJewel(MeshComponent->GetStaticMesh(), StartScale / 2);
	}
	
	// When the timeline has ended, destroy this actor
	Destroy();
}
