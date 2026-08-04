
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "Enemy.h"
#include "Engine/TimerHandle.h"


#include "PowerUp.generated.h"


UENUM(BlueprintType)
enum class EPowerUpType : uint8
{
	MovementSpeed UMETA(DisplayName = "Movement Speed"),
	AttackSpeed UMETA(DisplayName = "Attack Speed"),
	Bomb UMETA(DisplayName = "Bomb")
};

UCLASS()
class GUNSURVIVORS_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPowerUpType PowerUpType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* PowerUpFlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* MovementSpeedFlipbookAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* AttackSpeedFlipbookAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* BombFlipbookAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeedToAdd = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeedToIncrease = 0.15f;

	UPROPERTY()
	ATopdownCharacter* PlayerRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> EnemyActorToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeleteTime = 11.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeedPowerUpTime = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeedPowerUpTime = 10.0f;

	FTimerHandle DeleteTimer;
	FTimerHandle MovementSpeedPowerUpTimer;
	FTimerHandle AttackSpeedPowerUpTimer;

	APowerUp();


	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	
	void KillAllEnemiesOnScreen();
	void UpdateFlipbook(EPowerUpType Type);
	void OnDestroyTimerTimeout();
	void IncreaseMovementSpeed(ATopdownCharacter* Player);
	void IncreaseAttackSpeed(ATopdownCharacter* Player);
	void SetMovementSpeedBackToNormal();
	void SetAttackSpeedBackToNormal();

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
