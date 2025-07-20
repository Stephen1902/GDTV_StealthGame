// Copyright 2025 DME Games

#include "Framework/EscapeActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/EscapedWidget.h"

// Sets default values
AEscapeActor::AEscapeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Comp"));
	CollisionComp->SetupAttachment(RootComp);

	static ConstructorHelpers::FClassFinder<UEscapedWidget> FoundWidget(TEXT("/Game/UI/WBP_Escaped"));
	if (FoundWidget.Succeeded())
	{
		EscapedWidgetToShow = FoundWidget.Class;
	}

}

// Called when the game starts or when spawned
void AEscapeActor::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEscapeActor::OnCollisionBegin);
}

void AEscapeActor::OnCollisionBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if what has overlapped is the player
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		if (EscapedWidgetToShow)
		{
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				UEscapedWidget* EscapedWidget = CreateWidget<UEscapedWidget>(PC, EscapedWidgetToShow);
				EscapedWidget->AddToViewport();
				EscapedWidget->FadeImage();

				PC->SetShowMouseCursor(true);
				FInputModeGameAndUI InputModeGameAndUI;
				InputModeGameAndUI.SetWidgetToFocus(EscapedWidget->TakeWidget());
				PC->SetInputMode(InputModeGameAndUI);
			}
		}
	}
}
