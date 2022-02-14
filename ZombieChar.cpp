#include "ZombieChar.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AZombieChar::AZombieChar()
{ 	
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZombieChar::BeginPlay()
{
	Super::BeginPlay();
	
	customAIController = Cast<AAIController>(GetController());
	if (customAIController == nullptr) return;

	currentBB = customAIController->GetBlackboardComponent();
	if (currentBB == nullptr) return;

	Health = 100.0f;
}

// Called every frame
void AZombieChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//once the enemy is dead, dont want the enemy is disappear directly or stay there forever, instead slowly dissociate into the air, the following code handles the 
	//dissociation
	if (bDead)
	{
		if (dynamicMat)
		{
			float v;
			dynamicMat->GetScalarParameterValue(TEXT("Opacity"),v);
			float val = FMath::Lerp(v,0.f,disappearingFactor);
			//UE_LOG(LogTemp,Warning,TEXT("(ZombieChar)Current: %f, new: %f"),v,val);
			dynamicMat->SetScalarParameterValue(TEXT("Opacity"), val);

			if (FMath::IsNearlyEqual(0.f, val,0.01f))
			{
				//UE_LOG(LogTemp,Warning,TEXT("(ZombieChar) Going to destroy itself"));
				if (customAIController!=nullptr) customAIController->Destroy();
				Destroy();
			}
		}
	}
}

// Called to bind functionality to input
void AZombieChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int AZombieChar::GetAttackNum()
{
	return AttackNum;
}

bool AZombieChar::GetbAttacking()
{
	return bAttacking;
}

bool AZombieChar::GetbDead()
{
	return bDead;
}

//called in behaviour tree from  a task
void AZombieChar::AttackPlayer()
{
	if (!bHit)
	{
		SetbAttack(true);

		if (customAIController != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("(ZombieChar)Found controller, name: %s"), *(customAIController->GetName()));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("(ZombieChar)Couldn't get the controller."));
		}
	}
}

void AZombieChar::SetbAttack(bool val)
{
	//don't want to change the attack number in between the attacks
	if (!bAttacking) AttackNum = rand() % 4;

	bAttacking = val;
	if (val)
	{
		UE_LOG(LogTemp,Warning,TEXT("(ZombieChar) Setting bAttacking to true."));
		if (currentBB != nullptr && customAIController!=nullptr)
		{
			customAIController->SetFocus(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
			currentBB->SetValueAsBool(TEXT("bAttackingPlayer"), true);
		}
	}
	else
	{
		if (currentBB != nullptr && customAIController != nullptr)
		{
			currentBB->SetValueAsBool(TEXT("bAttackingPlayer"), false);
			customAIController->ClearFocus(EAIFocusPriority::Default);
		}
		UE_LOG(LogTemp, Warning, TEXT("(ZombieChar) Setting bAttacking to false."));
	}
}

void AZombieChar::SetbDead(bool val)
{
	bDead = val;
	if (val)
	{
		UE_LOG(LogTemp, Warning, TEXT("(ZombieChar) Setting bDead to true."));
		/*GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("LeftUpLeg"),true);
		GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("RightUpLeg"),true);*/

		UMaterialInterface* skinMat = GetMesh()->GetMaterial(0);
		dynamicMat = UMaterialInstanceDynamic::Create(skinMat,GetMesh());
		GetMesh()->SetMaterial(0,dynamicMat);

		if (customAIController != nullptr)
		{
			customAIController->UnPossess();
			UE_LOG(LogTemp, Warning, TEXT("(ZombieChar) UnPossessed the ai controller of enemy."));
		}
		SetbHit(false);
		SetbAttack(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("(ZombieChar) Setting bDead to false."));
	}
}

bool AZombieChar::GetbHit()
{
	return bHit;
}

void AZombieChar::SetbHit(bool val)
{
	bHit = val;
	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("(ZombieChar)Setting bHit to true."));
		SetbAttack(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("(ZombieChar) Setting bhit to false."));
	}
}

float AZombieChar::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);
	if (Health > 0)
	{
		Health -= DamageAmount;
		UE_LOG(LogTemp,Warning,TEXT("(ZombieChar)Zombie took damage, remaining health: %f"),Health);
		if (Health > 0)
		{
			SetbHit(true);
		}
		else
		{
			SetbDead(true);
		}
	}
	return DamageAmount;
}

void AZombieChar::LineTraceForDamage()
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + GetActorRotation().Vector() * RangeForDamage;

	FHitResult hitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(hitResult,StartLocation,EndLocation,ECollisionChannel::ECC_Pawn,CollisionParams))
	{
		if (hitResult.GetActor()->GetClass() != this->GetClass())
		{
			UGameplayStatics::ApplyDamage(hitResult.GetActor(), DamageToApply, GetInstigatorController(), this, DamageClass);
		}
	}
}
