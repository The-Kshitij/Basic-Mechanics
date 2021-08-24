// Fill out your copyright notice in the Description page of Project Settings.


#include "SpittingNotifies.h"
#include "SpittingEnemy.h"

void USpittingNotifies::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ASpittingEnemy* Enemy = Cast<ASpittingEnemy>(MeshComp->GetOwner());
	if (Enemy)
	{
		if (Enemy)
		{
			Enemy->SpawnDamagingBall();
		}
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("(SpittingNotifi)Casting to ASpittingEnemy failed."));
	}
}