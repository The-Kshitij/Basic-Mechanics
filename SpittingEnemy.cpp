#include "SpittingEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

#include "DamagingBall.h"

// Sets default values
ASpittingEnemy::ASpittingEnemy()
{ 	
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpittingEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if (PlayerPawn == nullptr) return;

	customAIController = Cast<AAIController>(GetController());
	if (customAIController == nullptr) return;

	currentBB = customAIController->GetBlackboardComponent();
	if (currentBB == nullptr) return;

	Health = 100.0f;
}

// Called every frame
void ASpittingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	The below code, gives a dissociation effect the body of the enemy once it is dead
	*/
	if (bDead)
	{
		if (dynamicMat)
		{
			float v;
			dynamicMat->GetScalarParameterValue(TEXT("Opacity"), v);
			float val = FMath::Lerp(v, 0.f, disappearingFactor);
			//UE_LOG(LogTemp,Warning,TEXT("(ZombieChar)Current: %f, new: %f"),v,val);
			dynamicMat->SetScalarParameterValue(TEXT("Opacity"), val);

			if (FMath::IsNearlyEqual(0.f, val, 0.01f))
			{
				//UE_LOG(LogTemp,Warning,TEXT("(ZombieChar) Going to destroy itself"));
				if (customAIController != nullptr) customAIController->Destroy();
				Destroy();
			}
		}
	}
}

// Called to bind functionality to input
void ASpittingEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASpittingEnemy::SpawnDamagingBall()
{
	UE_LOG(LogTemp,Warning,TEXT("(SpittingEnemy) Going to spawn the ball"));
	if (PlayerPawn == nullptr) return;

	FVector VectorToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	FRotator RotationToFacePlayer = VectorToPlayer.Rotation();
	//DrawDebugLine(GetWorld(), GetActorLocation(), PlayerPawn->GetActorLocation(), FColor::Blue, true, 3.0f);
	UE_LOG(LogTemp,Warning,TEXT("(SpittingEnemy) Vector facing player: %s, Rotator: %s"),*(VectorToPlayer.ToString()),*(RotationToFacePlayer.ToString()));
	

	ADamagingBall* ball = GetWorld()->SpawnActor<ADamagingBall>(DamageBallClass,GetMesh()->GetSocketLocation(SpawningPoint), RotationToFacePlayer);
	if (ball)
	{
		UE_LOG(LogTemp,Warning,TEXT("(SpittingEnemy) Ball spawned"));

		//calculating a parabolic path to the player
		FVector StartPoint = PlayerPawn->GetActorLocation();
		StartPoint.Z = ball->GetActorLocation().Z; //making the z values same, the goal is to make a parbolic projectile towards player
		ball->TargetLocation = StartPoint;
		float d = FVector::Dist(StartPoint,ball->GetActorLocation());
		ball->TotalDistance = d;
		float xVelReq = d / ball->TotalTime;
		float zVelReq = (2 * ball->HeightOfProjectile) / ball->TotalTime;
		ball->xvel = xVelReq;
		ball->zvel = zVelReq;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("(SpittingEnemy) Ball can't be spawned"));
	}
}

void ASpittingEnemy::SetbSpitting(bool val)
{
	bSpitting = val;
	if (currentBB)
	{
		currentBB->SetValueAsBool(SpittingPropertyName,val);
	}
}

bool ASpittingEnemy::GetbSpitting()
{
	return bSpitting;
}

void ASpittingEnemy::SpitBall()
{
	SetbSpitting(true);
}

float ASpittingEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);
	if (!bDead)
	{
		Health -= DamageAmount;
		if (Health < 0)
		{
			bDead = true;
			DetachFromControllerPendingDestroy();
			if (GetMesh() != nullptr)
			{
				//creating a dynamic mat, because don't want to destroy the enemy right away, instead slowly dissociate
				UMaterialInterface* skinMat = GetMesh()->GetMaterial(0);
				dynamicMat = UMaterialInstanceDynamic::Create(skinMat, GetMesh());
				GetMesh()->SetMaterial(0, dynamicMat);
			}
		}
	}
	return DamageAmount;
}

bool ASpittingEnemy::GetbDead()
{
	return bDead;
}
