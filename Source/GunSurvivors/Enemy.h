#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "Components/SceneComponent.h"
#include "TopdownCharacter.h"
#include "Engine/TimerHandle.h"
#include "Sound/SoundBase.h"
class APowerUp;


#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDiedDelegate);

UCLASS()
class AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AEnemy();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* EnemyFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ATopdownCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* EnemyDeadFlipbookAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* EnemyDeadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Sirve = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanFollow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistance = 15.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APowerUp> PowerUpToSpawn;

	FTimerHandle DestroyTimer;

	FEnemyDiedDelegate EnemyDiedDelegate;

	virtual void BeginPlay() override;	
	
	virtual void Tick(float DeltaTime) override;

	void Die();
	void OnDestroyTimerTimeout();

	void SpawnPowerUp();

};
