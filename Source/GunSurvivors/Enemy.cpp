#include "Enemy.h"
#include "PowerUp.h"
#include "Kismet/GameplayStatics.h"


AEnemy::AEnemy()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	EnemyFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemyFlipbook"));
	EnemyFlipbook->SetupAttachment(RootComponent);

	





}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Follow the player
	if (IsAlive && CanFollow && Player)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector PlayerLocation = Player->GetActorLocation();
		FVector DirectionToPlayer = PlayerLocation - CurrentLocation;
		
		float DistanceToPlayer = DirectionToPlayer.Length();

		if (DistanceToPlayer >= StopDistance) 
		{
			DirectionToPlayer.Normalize();

			FVector NewLocation = CurrentLocation + (DirectionToPlayer * MovementSpeed * DeltaTime);
			SetActorLocation(NewLocation);
		}

		CurrentLocation = GetActorLocation();
		float FlipbookScaleX = EnemyFlipbook->GetComponentScale().X;

		if ((PlayerLocation.X - CurrentLocation.X) >= 0.0f) // Player is on the right side of the enemy
		{
			if (FlipbookScaleX < 0.0f)
			{
				EnemyFlipbook->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}
		else  // Player is on the left side of the enemy
		{
			if (FlipbookScaleX > 0.0f)
			{
				EnemyFlipbook->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
			}
		}
	}

	
	

}

void AEnemy::Die() {
	if (!IsAlive) return;

	IsAlive = false;

	CanFollow = false;

	EnemyFlipbook->SetFlipbook(EnemyDeadFlipbookAsset);
	EnemyFlipbook->SetTranslucentSortPriority(-5);

	EnemyDiedDelegate.Broadcast();
	UGameplayStatics::PlaySound2D(GetWorld(), EnemyDeadSound);

	SpawnPowerUp();


	float DeleteTime = 5.0f;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEnemy::OnDestroyTimerTimeout, 0.1f, false, DeleteTime);

	
}
void AEnemy::OnDestroyTimerTimeout() {
	Destroy();
}

void AEnemy::SpawnPowerUp()
{

	float Roll = FMath::FRandRange(0.0f, 100.0f);

	if (Roll <= 80.0f)
	{
		APowerUp* PowerUp = GetWorld()->SpawnActor<APowerUp>(PowerUpToSpawn, GetActorLocation(), FRotator::ZeroRotator);
		if (PowerUp)
		{
			UE_LOG(LogTemp, Warning, TEXT("PowerUp spawned successfully"));
		}
	}
}