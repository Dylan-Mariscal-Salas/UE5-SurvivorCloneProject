// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/TimerHandle.h"

#include "GunSurvivorsGameMode1.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreChangedDelegate, int, NewScore);
/**
 * 
 */
UCLASS()
class GUNSURVIVORS_API AGunSurvivorsGameMode1 : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int Score = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimerBeforeRestart = 1.0f;

	FTimerHandle RestartGameTimer;
	

	UPROPERTY(BlueprintAssignable)
	FScoreChangedDelegate ScoreChangedDelegate;

	AGunSurvivorsGameMode1();
	virtual void BeginPlay() override;

	void SetScore(int NewScore);
	void AddScore(int AmountToAdd);

	void RestartGame();
	void OnRestartGameTimerTimeout();
	
};
