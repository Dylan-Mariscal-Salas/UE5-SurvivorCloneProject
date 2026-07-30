


#include "Bullet.h"
#include "Enemy.h"

// Sets default values
ABullet::ABullet()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	BulletSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BulletSprite"));
	BulletSprite->SetupAttachment(SphereComp);

	BulletDirection = FVector2D(1.0f, 0.0f);
}


void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OverlapBegin);

}


void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLaunched)
	{
		FVector2D DistanceToMove = BulletDirection * BulletMovementSpeed * DeltaTime;
		FVector BulletNewLocation = GetActorLocation() + FVector(DistanceToMove.X, 0.0f, DistanceToMove.Y);
		SetActorLocation(BulletNewLocation);
	}

	
}

void ABullet::LaunchBullet(FVector2D Direction, float MovementSpeed)
{
	if (IsLaunched) return;

	IsLaunched = true;

	BulletDirection = Direction;
	BulletMovementSpeed = MovementSpeed;

	float DeleteTime = 5.0f;
	GetWorldTimerManager().SetTimer(DeleteTimer,this, &ABullet::DeleteBullet,1.0f,false,DeleteTime);

}

void ABullet::DeleteBullet()
{
	Destroy();
}

void ABullet::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (Enemy && Enemy->IsAlive)
	{
		DisableBullet();
		Enemy->Die();

	}
}

void ABullet::DisableBullet()
{
	if (IsDisabled) return;

	IsDisabled = true;

	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BulletSprite->DestroyComponent();
}

