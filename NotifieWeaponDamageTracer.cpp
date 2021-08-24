// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifieWeaponDamageTracer.h"
#include "WeaponActor.h"
#include "ZombieChar.h"

void UNotifieWeaponDamageTracer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AWeaponActor* weapon = Cast<AWeaponActor>(MeshComp->GetOwner());
	if (weapon!=nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("(NotifieWeaponDamageTracer) going to call line trace"));
		weapon->LineTraceForDamage();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("(NotifieWeaponDamageTracer) Casting to weapon failed."));
		AZombieChar* enemy = Cast<AZombieChar>(MeshComp->GetOwner());
		if (enemy)
		{
			enemy->LineTraceForDamage();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("(NotifieWeaponDamageTracer) Casting to AZombieChar also failed failed."));
		}
	}
}
