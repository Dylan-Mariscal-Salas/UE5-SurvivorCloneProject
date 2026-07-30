

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
	
}


void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerUp::KillAllEnemiesOnScreen()
{
	TArray<AActor*> EnemiesOnScreen;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), EnemiesOnScreen);

	for (AActor* Actor : EnemiesOnScreen)
	{
		AEnemy* Enemy = Cast<AEnemy>(Actor);
		if (Enemy && Enemy->IsAlive)
		{
			Enemy->CanFollow = false;
			Enemy->Die();
		}
	}

}

void APowerUp::ApplyPowerUp(ATopdownCharacter* Player)
{

}

void APowerUp::ChoosePowerUp()
{

}