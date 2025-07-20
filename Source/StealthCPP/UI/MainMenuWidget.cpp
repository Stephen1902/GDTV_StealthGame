// Copyright 2025 DME Games


#include "UI/MainMenuWidget.h"

#include "LeaderboardWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerHighScores.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Set up the bindings for the buttons in the world
	BTN_Play->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayButtonClicked);
	BTN_Exit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
	
	// Check for a save slot and load previous times
	UPlayerHighScores* PlayerHighScores;
	if (UGameplayStatics::DoesSaveGameExist("PlayerSavedTimes", 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Saved slot exists"));
		PlayerHighScores = Cast<UPlayerHighScores>(UGameplayStatics::LoadGameFromSlot("PlayerSavedTimes", 0));
		UE_LOG(LogTemp, Warning, TEXT("GetSavedScores has %i entries"), PlayerHighScores->GetSavedScores().Num());
	}
	else
	// Create a save game slot and automatically save to it
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating saved slot"));
		PlayerHighScores = Cast<UPlayerHighScores>(UGameplayStatics::CreateSaveGameObject(UPlayerHighScores::StaticClass()));
		PlayerHighScores->SetNewScore(FText::FromString("00 : 30"));
		PlayerHighScores->SetNewScore(FText::FromString("00 : 35"));
		UGameplayStatics::SaveGameToSlot(PlayerHighScores, "PlayerSavedTimes", 0);
	}
	
	if (PlayerHighScores->GetSavedScores().Num() > 0 && LeaderboardWidgetToDisplay)
	{
		TArray<FText> SavedScores = PlayerHighScores->GetSavedScores();
		UE_LOG(LogTemp, Warning, TEXT("Saved Scores Array has %i entries"), SavedScores.Num());
		for (int32 i = 0; i < SavedScores.Num(); ++i)
		{
			ULeaderboardWidget* LeaderboardWidget = CreateWidget<ULeaderboardWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), LeaderboardWidgetToDisplay);
			UE_LOG(LogTemp, Warning, TEXT("Leaderboard Text is %s"), *SavedScores[i].ToString());
			LeaderboardWidget->SetLeaderboardText(SavedScores[i]);
			SB_Leaderboard->AddChild(LeaderboardWidget);
		}
	}
}

void UMainMenuWidget::OnPlayButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}

void UMainMenuWidget::OnExitButtonClicked()
{
	TEnumAsByte QuitPreference = EQuitPreference::Quit; 
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, QuitPreference, false);
}
