// Copyright 2025 DME Games

#include "SensorsAlarms/SensorRotative.h"

#include "Components/SpotLightComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASensorRotative::ASensorRotative()
{
	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>("Head Mesh Comp");
	HeadMesh->SetupAttachment(RootComp);

	SpotLightComp = CreateDefaultSubobject<USpotLightComponent>("Spotlight Comp");
	SpotLightComp->SetupAttachment(HeadMesh);
	SpotLightComp->Intensity = 100000.f;
	SpotLightComp->SetLightColor(FColor::Red);

	DetectionMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Detection Mesh Comp");
	DetectionMeshComp->SetupAttachment(SpotLightComp);
	DetectionMeshComp->SetCollisionResponseToAllChannels(ECR_Overlap);

	bTimelineForward = true;
	ForwardRotation = FRotator(0.f, 90.f, 0.f);
	BackwardRotation = FRotator(0.f, -90.f, 0.f);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> FoundCurve(TEXT("/Game/Alarms_Sensors/Sensors/CF_RotatingSensor"));
	if (FoundCurve.Succeeded())
	{
		FloatCurve = FoundCurve.Object;

		RotateTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Rotating Timeline Comp"));
				
		//Bind the Callback function for the float return value
		RotateFunction.BindUFunction(this, FName{ TEXT("TimelineRotateReturn") });
		EndOfRotateFunction.BindUFunction(this, FName{TEXT("EndOfRotation")});
	}
}

void ASensorRotative::BeginPlay()
{
	Super::BeginPlay();

	DetectionMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ASensorRotative::OnMeshOverlapped);

	if (RotateTimeline)
	{
		RotateTimeline->AddInterpFloat(FloatCurve, RotateFunction, FName{ TEXT("Rotate Timeline") });
		RotateTimeline->SetTimelineFinishedFunc(EndOfRotateFunction);
		RotateTimeline->Play();
	}
}

void ASensorRotative::OnMeshOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	DetectPlayer(OtherActor);
}

void ASensorRotative::TimelineRotateReturn(float Val)
{
	const FRotator RotThisVal = UKismetMathLibrary::RLerp(ForwardRotation, BackwardRotation, Val, false);
	HeadMesh->SetRelativeRotation(RotThisVal);
}

void ASensorRotative::EndOfRotation()
{
	UE_LOG(LogTemp, Warning, TEXT("Ended"));
	if (bTimelineForward)
	{
		bTimelineForward = false;
		RotateTimeline->ReverseFromEnd();
	}
	else
	{
		bTimelineForward = true;
		RotateTimeline->PlayFromStart();
	}
}
