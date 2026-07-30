#include "GunSurvivorsGameMode1.h"
#include "Kismet/GameplayStatics.h"

AGunSurvivorsGameMode1::AGunSurvivorsGameMode1()
{
	PrimaryActorTick.bCanEverTick = true;
	SetScore(0);
}

void AGunSurvivorsGameMode1::BeginPlay() {
	Super::BeginPlay();
	SetScore(0);
}

void AGunSurvivorsGameMode1::SetScore(int NewScore)
{
	if (NewScore >= 0)
	{
		Score = NewScore;
		ScoreChangedDelegate.Broadcast(Score);
	}
}
void AGunSurvivorsGameMode1::AddScore(int AmountToAdd)
{
	int NewScore = Score + AmountToAdd;
	SetScore(NewScore);

	
}

void AGunSurvivorsGameMode1::RestartGame()
{
	GetWorldTimerManager().SetTimer(RestartGameTimer, this, &AGunSurvivorsGameMode1::OnRestartGameTimerTimeout, 1.0f, false, TimerBeforeRestart);
}
void AGunSurvivorsGameMode1::OnRestartGameTimerTimeout()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}