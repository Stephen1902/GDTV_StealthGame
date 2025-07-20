// Copyright 2025 DME Games

#include "Framework/EscapeActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerHighScores.h"
#include "Player/StealthCharacter.h"
#include "UI/EscapedWidget.h"
#include "UI/TimerWidget.h"

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
	if (AStealthCharacter* PlayerCharacter = Cast<AStealthCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
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

		// The save slot should have been created already.  If it doesn't, there is a problem in the MainMenuWidget class
		if (UGameplayStatics::DoesSaveGameExist("PlayerSavedTimes", 0))
		{
			// Load the existing save
			UPlayerHighScores* PlayerHighScores = Cast<UPlayerHighScores>(UGameplayStatics::LoadGameFromSlot("PlayerSavedTimes", 0));
			// Add the player time to the saved game array
			PlayerHighScores->SetNewScore(PlayerCharacter->GetTimerWidget()->GetCurrentTime());
			// Save the game
			const bool bSaved = UGameplayStatics::SaveGameToSlot(PlayerHighScores, "PlayerSavedTimes", 0);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Tried to save game in Escape Actor, but the save slot doesn't exist."));
		}	
	}
}
