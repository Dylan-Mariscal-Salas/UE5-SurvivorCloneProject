


#include "EnemySpawner.h"
#include "Kismet/GameplayStatics.h"


AEnemySpawner::AEnemySpawner()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}


void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase *GameModeBase = UGameplayStatics::GetGameMode(GetWorld());
	if (GameModeBase)
	{
		MyGameMode = Cast<AGunSurvivorsGameMode1>(GameModeBase);
		check(MyGameMode);
	}


	AActor* PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATopdownCharacter::StaticClass());
	if (PlayerActor)
	{
		Player = Cast<ATopdownCharacter>(PlayerActor);
		Player->PlayerDiedDelegate.AddDynamic(this, &AEnemySpawner::OnPlayerDied);
		
	}

	StartSpawning();
	
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AEnemySpawner::StartSpawning()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::OnSpawnTimerTimeout, SpawnTime, true, SpawnTime);
}
void AEnemySpawner::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}
void AEnemySpawner::OnSpawnTimerTimeout()
{
	SpawnEnemy();
}
void AEnemySpawner::SpawnEnemy()
{
	//Use VRAND to create a random location where the enemy will spawn in a circle
	FVector2D RandomPosition = FVector2D(FMath::VRand());

	// Normalize that vector
	RandomPosition.Normalize();

	// multiply that vector to the spawn location
	RandomPosition *= SpawnPosition;

	//Add the Random position to the new enemy spawn Location
	FVector EnemySpawnLocation = GetActorLocation() + FVector(RandomPosition.X, 0.0f, RandomPosition.Y);
	AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(EnemyActorToSpawn, EnemySpawnLocation, FRotator::ZeroRotator);
	
	SetupEnemy(Enemy);
	
	NumberOfEnemiesSpawned += 1;

	if ((NumberOfEnemiesSpawned % DifficultySpawnInterval) == 0)
	{
		if (SpawnTime > SpawnTimeMinimumLimit)
		{
			SpawnTime -= DecreaseSpawnTimeByEveryInterval;

			if (SpawnTime < DecreaseSpawnTimeByEveryInterval)
			{
				SpawnTime = DecreaseSpawnTimeByEveryInterval;
			}

			StopSpawning();
			StartSpawning();
		}
	}
		
	
	
}

void AEnemySpawner::SetupEnemy(AEnemy* Enemy)
{
	if (Enemy)
	{
		Enemy->Player = Player;
		Enemy->CanFollow = true;
		Enemy->EnemyDiedDelegate.AddDynamic(this, &AEnemySpawner::OnEnemyDied);
	}
	
}

void AEnemySpawner::OnEnemyDied()
{
	
	
	int ScoreToAdd = 10;
	MyGameMode->AddScore(ScoreToAdd);
}

void AEnemySpawner::OnPlayerDied()
{
	StopSpawning();

	TArray<AActor*> EnemyArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), EnemyArray);
	for (AActor* EnemyActor : EnemyArray)
	{
		AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
		if (Enemy && Enemy->IsAlive)
		{
			Enemy->CanFollow = false;
		}
	}

	MyGameMode->RestartGame();
}
