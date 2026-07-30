
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "Enemy.h"


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
	float MovementSpeedToAdd = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeedToIncrease = 0.25f;

	

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> EnemyActorToSpawn;
	
	
	APowerUp();


	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	void ChoosePowerUp();
	void ApplyPowerUp(ATopdownCharacter* Player);
	void KillAllEnemiesOnScreen();
	
};
