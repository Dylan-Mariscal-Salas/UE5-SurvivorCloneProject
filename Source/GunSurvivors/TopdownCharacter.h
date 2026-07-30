#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/SceneComponent.h"
#include "PaperSpriteComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/TimerHandle.h"
#include "Bullet.h"
#include "Sound/SoundBase.h"


#include "TopdownCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDiedDelegate);

UCLASS()
class GUNSURVIVORS_API ATopdownCharacter : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* CharacterFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* GunParent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* GunSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* BulletSpawnPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ShootSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DieSound;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* RunFlipbook;
 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementSpeed = 100.0f;

	UPROPERTY(BlueprintReadWrite)
	FVector2D MovementDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanMove = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2D HorizontalLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D VerticalLimit;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABullet> BulletActorSpawn;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanShoot = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootCooldownDuration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Sirve = true;

	FTimerHandle ShootCooldownTimer;

	FPlayerDiedDelegate PlayerDiedDelegate;


	ATopdownCharacter();

	virtual void BeginPlay() override;	
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveTriggered(const FInputActionValue& Value);

	void MoveCompleted(const FInputActionValue& Value);

	void Shoot(const FInputActionValue& Value);

	bool IsInMapBoundsVertical(float XPos);

	bool IsInMapBoundsHorizontal(float ZPos);

	void OnShootCooldownTimerTimeout();

	UFUNCTION(BlueprintCallable, Category = "Test")
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
