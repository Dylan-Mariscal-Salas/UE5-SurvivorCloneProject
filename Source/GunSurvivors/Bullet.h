

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "Engine/TimerHandle.h" 

#include "Bullet.generated.h"

UCLASS()
class GUNSURVIVORS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* BulletSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D BulletDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BulletMovementSpeed=100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsLaunched = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsDisabled = false;

	FTimerHandle DeleteTimer;

	ABullet();


	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	void LaunchBullet(FVector2D Direction, float MovementSpeed);

	void DeleteBullet();

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DisableBullet();
};
