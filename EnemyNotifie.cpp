// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyNotifie.h"
#include "ZombieChar.h"
#include "SpittingEnemy.h"

void UEnemyNotifie::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AZombieChar* Enemy = Cast<AZombieChar>(MeshComp->GetOwner());
	if (Enemy != nullptr)
	{
		FString AnimationName = Animation->GetName();
		if (AnimationName.Compare("Zombie_GetHit") == 0)
		{
			Enemy->SetbHit(false);
			UE_LOG(LogTemp, Warning, TEXT("(EnemyNotifie) Calling to Set hit to false on enemy."));
		}
		else if (AnimationName.Compare("Zombie_Attack1") == 0 || AnimationName.Compare("Zombie_Attack2") == 0 || AnimationName.Compare("Zombie_Attack3") == 0 || AnimationName.Compare("Zombie_Attack4") == 0)
		{
			UE_LOG(LogTemp,Warning,TEXT("(EnemyNotifie) Setting attacking to false."));
			Enemy->SetbAttack(false);
		}
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("(EnemyNotifie) Casting to AZombieChar failed."));
		ASpittingEnemy* enemy = Cast<ASpittingEnemy>(MeshComp->GetOwner());
		if (enemy)
		{
			enemy->SetbSpitting(false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("(EnemyNotifie) Casting to ASpittingEnemy failed."));
		}
	}
}