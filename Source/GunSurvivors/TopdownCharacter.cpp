


#include "TopdownCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

ATopdownCharacter::ATopdownCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	CharacterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterFlipbook"));
	CharacterFlipbook->SetupAttachment(RootComponent);

	GunParent = CreateDefaultSubobject<USceneComponent>(TEXT("GunParent"));
	GunParent->SetupAttachment(RootComponent);

	GunSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GunSprite"));
	GunSprite->SetupAttachment(GunParent);

	BulletSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPosition"));
	BulletSpawnPosition->SetupAttachment(GunSprite);


}

void ATopdownCharacter::BeginPlay()
{
	Super::BeginPlay();



	//Get Input Mapping system
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	
	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(true);

		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ATopdownCharacter::OverlapBegin);
	
	UE_LOG(LogTemp, Warning, TEXT("Is Bound: %d"),
		CapsuleComp->OnComponentBeginOverlap.IsBound());
	
}

bool ATopdownCharacter::IsInMapBoundsVertical(float XPos)
{
	bool Result = true;

	Result = (XPos > HorizontalLimit.X) && (XPos < HorizontalLimit.Y);

	return Result;
}

bool ATopdownCharacter::IsInMapBoundsHorizontal(float ZPos)
{
	bool Result = true;

	Result = (ZPos > VerticalLimit.X) && (ZPos < VerticalLimit.Y);

	return Result;
}

void ATopdownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Player movement between map bounds
	if (CanMove)
	{
		if (MovementDirection.Length() > 0)
		{
			if (MovementDirection.Length() > 1)
			{
				MovementDirection.Normalize();
			}

			FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;
			FVector CurrentLocation = GetActorLocation();
			FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.0f, 0.0f);

			if (!IsInMapBoundsHorizontal(NewLocation.X))
			{
				NewLocation -= FVector(DistanceToMove.X, 0.0f, 0.0f);
			}
			NewLocation += FVector(0.0f, 0.0f, DistanceToMove.Y);
			if (!IsInMapBoundsVertical(NewLocation.Z))
			{
				NewLocation -= FVector(0.0f, 0.0f, DistanceToMove.Y);
			}

			SetActorLocation(NewLocation);
		}
	}

	// Rotate the gun
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)
	{
		FVector MouseWorldLocation, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		FVector CurrentLocation = GetActorLocation();
		FVector Start = FVector(CurrentLocation.X, 0.0f, CurrentLocation.Z);
		FVector Target = FVector(MouseWorldLocation.X, 0.0f, MouseWorldLocation.Z);
		FRotator GunParentRotator = UKismetMathLibrary::FindLookAtRotation(Start,Target);

		GunParent->SetRelativeRotation(GunParentRotator);
	}

}

void ATopdownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Getting the input component
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopdownCharacter::MoveTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATopdownCharacter::MoveCompleted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ATopdownCharacter::MoveCompleted);


		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATopdownCharacter::Shoot);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ATopdownCharacter::Shoot);
	}
}

void ATopdownCharacter::MoveTriggered(const FInputActionValue& Value)
{
	//Changing player flipbook face direction
	FVector2D MoveActionValue = Value.Get<FVector2D>();
	if (CanMove)
	{
		MovementDirection = MoveActionValue;
		CharacterFlipbook->SetFlipbook(RunFlipbook);

		FVector FlipbookScale = CharacterFlipbook->GetComponentScale();

		if (MovementDirection.X < 0.0f)
		{
			if (FlipbookScale.X > 0.0f)
			{
				CharacterFlipbook->SetWorldScale3D(FVector(-2.0f, 2.0f, 2.0f));
			}
		}
		else if (MovementDirection.X > 0.0f)
		{
			if (FlipbookScale.X < 0.0f) 
			{
				CharacterFlipbook->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
			}
		}

	}

}

void ATopdownCharacter::MoveCompleted(const FInputActionValue& Value)
{
	MovementDirection = FVector2D(0.0f, 0.0f);
	if (IsAlive)
	{
		CharacterFlipbook->SetFlipbook(IdleFlipbook);
	}
	

}

void ATopdownCharacter::Shoot(const FInputActionValue& Value)
{
	if (CanShoot)
	{
		CanShoot = false;

		//Bullet spawn code...
		ABullet *Bullet = GetWorld()->SpawnActor<ABullet>(BulletActorSpawn, BulletSpawnPosition->GetComponentLocation(), FRotator(0.0f,0.0f,0.0f));
		check(Bullet);

		//Getting mouse world location
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		check(PlayerController);
		FVector MouseWorldLocation, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		// Getting bullet direction
		FVector CurrentLocation = GetActorLocation();
		FVector2D BulletDirection = FVector2D(MouseWorldLocation.X - CurrentLocation.X, MouseWorldLocation.Z - CurrentLocation.Z);
		BulletDirection.Normalize();

		//Launching the bullet
		float BulletMovementSpeed = 500.0f;
		Bullet->LaunchBullet(BulletDirection, BulletMovementSpeed);

		//Attack Speed
		GetWorldTimerManager().SetTimer(ShootCooldownTimer, this, &ATopdownCharacter::OnShootCooldownTimerTimeout, 1.0f, false, ShootCooldownDuration);
			
		UGameplayStatics::PlaySound2D(GetWorld(), ShootSound);
	}
}

void ATopdownCharacter::OnShootCooldownTimerTimeout()
{
	if(IsAlive)
	{
		CanShoot = true;
	}
}

void ATopdownCharacter::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("Player Overlap"));

	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	}
	if (Enemy && Enemy->IsAlive)
	{
		
		if (IsAlive)
		{
			IsAlive = false;
			CanMove = false;
			CanShoot = false;
			PlayerDiedDelegate.Broadcast();
			UE_LOG(LogTemp, Warning, TEXT("SIRVE?"));
			UGameplayStatics::PlaySound2D(GetWorld(), DieSound);
		}
	}
}