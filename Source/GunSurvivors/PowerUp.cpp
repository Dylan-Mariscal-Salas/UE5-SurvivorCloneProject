

#include "PowerUp.h"
#include "Kismet/GameplayStatics.h"



APowerUp::APowerUp()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	PowerUpFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PaperFlipbookComponent"));
	PowerUpFlipbookComponent->SetupAttachment(RootComponent);

}


void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::OverlapBegin);
	GetWorldTimerManager().SetTimer(DeleteTimer, this, &APowerUp::OnDestroyTimerTimeout, 0.1f, false, DeleteTime);
}


void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APowerUp::UpdateFlipbook(EPowerUpType Type)
{
	if(Type == EPowerUpType::MovementSpeed)
	{
		PowerUpFlipbookComponent->SetFlipbook(MovementSpeedFlipbookAsset);
	}
	else if(Type == EPowerUpType::AttackSpeed)
	{
		PowerUpFlipbookComponent->SetFlipbook(AttackSpeedFlipbookAsset);
	}
	else if (Type == EPowerUpType::Bomb)
	{
		PowerUpFlipbookComponent->SetFlipbook(BombFlipbookAsset);
	}
}

void APowerUp::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATopdownCharacter* Player = Cast<ATopdownCharacter>(OtherActor);

	if (Player && Player->IsAlive)
	{
		if (PowerUpType == EPowerUpType::MovementSpeed)
		{
			IncreaseMovementSpeed(Player);
		}
		else if (PowerUpType == EPowerUpType::AttackSpeed)
		{
			IncreaseAttackSpeed(Player);
		}
		else if (PowerUpType == EPowerUpType::Bomb)
		{
			KillAllEnemiesOnScreen();
		}
	}
	
}

void APowerUp::OnDestroyTimerTimeout()
{
	Destroy();
}

void APowerUp::IncreaseMovementSpeed(ATopdownCharacter* Player)
{
	
	if (Player && Player->IsAlive)
	{
		PlayerRef = Player;
		Player->MovementSpeed = MovementSpeedToAdd;
		GetWorldTimerManager().SetTimer(MovementSpeedPowerUpTimer, this, &APowerUp::SetMovementSpeedBackToNormal, 0.1f, false, MovementSpeedPowerUpTime);
	}
	

}

void APowerUp::IncreaseAttackSpeed(ATopdownCharacter* Player)
{
	if (Player && Player->IsAlive)
	{
		PlayerRef = Player;
		Player->ShootCooldownDuration = AttackSpeedToIncrease;
		GetWorldTimerManager().SetTimer(AttackSpeedPowerUpTimer, this, &APowerUp::SetAttackSpeedBackToNormal, 0.1f, false, MovementSpeedPowerUpTime);
	}
}

void APowerUp::SetMovementSpeedBackToNormal()
{
	if (PlayerRef && PlayerRef->IsAlive)
	{
		PlayerRef->MovementSpeed = 100.0f;
	}
}

void APowerUp::SetAttackSpeedBackToNormal()
{
	if (PlayerRef && PlayerRef->IsAlive)
	{
		PlayerRef-> ShootCooldownDuration = 0.3f;
	}
}

void APowerUp::KillAllEnemiesOnScreen()
{
	TArray<AActor*> EnemyActors;

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AEnemy::StaticClass(),
		EnemyActors);

	for (AActor* Actor : EnemyActors)
	{
		AEnemy* Enemy = Cast<AEnemy>(Actor);

		if (!Enemy)
		{
			continue;
		}

		if (!Enemy->IsAlive)
		{
			continue;
		}

		Enemy->CanFollow = false;
		Enemy->Die();
	}

}