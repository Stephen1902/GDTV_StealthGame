// Copyright 2025 DME Games

#include "SensorsAlarms/SensorRotative.h"

#include "Components/ProgressBar.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/StealthCharacter.h"

ASensorRotative::ASensorRotative()
{
	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>("Wall Mesh Comp");
	WallMesh->SetupAttachment(RootComp);
		
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
	ForwardRotation = FRotator(0.f, 180.f, 0.f);
	BackwardRotation = FRotator(0.f, 0.f, 0.f);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> FoundCurve(TEXT("/Game/Alarms_Sensors/Sensors/CF_RotatingSensor"));
	if (FoundCurve.Succeeded())
	{
		FloatCurve = FoundCurve.Object;

		RotateTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Rotating Timeline Comp"));
				
		//Bind the Callback function for the float return value
		RotateFunction.BindUFunction(this, FName{ TEXT("TimelineRotateReturn") });
		EndOfRotateFunction.BindUFunction(this, FName{TEXT("EndOfRotation")});
	}

	bPlayerInZone = false;
	TimeInZone = 0.f;
	TimeToFillDetectionGauge = 3.0f;
}

void ASensorRotative::BeginPlay()
{
	Super::BeginPlay();

	DetectionMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ASensorRotative::OnMeshOverlapped);
	DetectionMeshComp->OnComponentEndOverlap.AddDynamic(this, &ASensorRotative::OnMeshOverlapEnd);

	if (RotateTimeline)
	{
		RotateTimeline->AddInterpFloat(FloatCurve, RotateFunction, FName{ TEXT("Rotate Timeline") });
		RotateTimeline->SetTimelineFinishedFunc(EndOfRotateFunction);
		RotateTimeline->Play();
	}
}

void ASensorRotative::OnMeshOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if it's the player 
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		bPlayerInZone = true;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "PlayerInZone", OtherActor);
		GetWorld()->GetTimerManager().SetTimer(PlayerInZoneTimer, TimerDelegate, 0.002f, true, 0.f);
	}	
}

void ASensorRotative::OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Out"));
		bPlayerInZone = false;
		// If the player has not triggered the sensor, reset everything
		if (!bHasBeenTriggered)
		{
			GetWorld()->GetTimerManager().ClearTimer(PlayerInZoneTimer);
			GetWorld()->GetTimerManager().SetTimer(PlayerInZoneTimer, this, &ASensorRotative::PlayerOutOfZone, 0.002f, true, 0.f);
		}
	}
}

void ASensorRotative::TimelineRotateReturn(float Val)
{
	const FRotator RotThisVal = UKismetMathLibrary::RLerp(ForwardRotation, BackwardRotation, Val, false);
	HeadMesh->SetRelativeRotation(RotThisVal);
}

void ASensorRotative::EndOfRotation()
{
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

void ASensorRotative::PlayerInZone(AActor* OtherActor)
{
	TimeInZone += 0.005f;
	
	UpdateProgressBar();

	// Check if the player has stood in the zone for longer than the detection time
	if (TimeInZone > TimeToFillDetectionGauge)
	{
		// Call the detect player in the sensor base class
		DetectPlayer(OtherActor);
		GetWorld()->GetTimerManager().ClearTimer(PlayerInZoneTimer);
	}
}

void ASensorRotative::PlayerOutOfZone()
{
	TimeInZone -= 0.005f;

	UpdateProgressBar();

	if (TimeInZone <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(PlayerInZoneTimer);
	}
	
}

void ASensorRotative::UpdateProgressBar()
{
	if (!PlayerRef)
	{
		PlayerRef = Cast<AStealthCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	if (PlayerRef)
	{
		PlayerRef->SetWidgetPercent(TimeInZone / TimeToFillDetectionGauge);
	}
}
