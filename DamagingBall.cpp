#include "DamagingBall.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
ADamagingBall::ADamagingBall()
{ 	
	PrimaryActorTick.bCanEverTick = true;
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body of ball"));
	SetRootComponent(BodyMesh);
}

// Called when the game starts or when spawned
void ADamagingBall::BeginPlay()
{
	Super::BeginPlay();
	TimeOfSpawn = UGameplayStatics::GetTimeSeconds(GetWorld());
	InitialLocation = GetActorLocation();
	bRaising = true;
}

// Called every frame
void ADamagingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float t = UGameplayStatics::GetTimeSeconds(GetWorld()) - TimeOfSpawn;
	if (FMath::IsNearlyEqual(t,TotalTime,0.001f))
	{
		Destroy();
	}
	//UE_LOG(LogTemp,Warning,TEXT("Location: %s"),*(GetActorLocation().ToString()));
	float xVal = xvel * t;
	FVector newLocation = GetActorRotation().Vector() * xVal + InitialLocation;
	//DrawDebugLine(GetWorld(),InitialLocation,newLocation,FColor::Blue,true,3.0f);
	float z = ZCurve->GetFloatValue(t);
	
	//bRaising is used to ensure that the gun doesn't fire before the player has transitioned into a shooting pose
	if (bRaising && FMath::IsNearlyEqual(TotalTime * 0.5f, t, 0.01f))
	{
		bRaising = false;
	}
	float newZ = InitialLocation.Z;
	float b = FMath::Clamp(ZCurve->GetFloatValue(t), 0.0f, HeightOfProjectile);
	newZ += b;
	//UE_LOG(LogTemp,Warning,TEXT("Newz value: %f"),newZ);
	newLocation.Z = newZ;
	SetActorLocation(newLocation);
}

void ADamagingBall::ApplyDamageToThisPawn(AActor* Player)
{
	UE_LOG(LogTemp,Warning,TEXT("(DamagingBall)Player: %s"),*(Player->GetName()));
	UGameplayStatics::ApplyDamage(Player, 5.0f, GetInstigatorController(), this, DamageClass);
	Destroy();
}
