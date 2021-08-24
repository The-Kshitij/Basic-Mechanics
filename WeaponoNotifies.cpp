// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponoNotifies.h"
#include "WeaponActor.h"
#include "MainPlayer.h"

void UWeaponoNotifies::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AWeaponActor* weapon = Cast<AWeaponActor>(MeshComp->GetOwner());
	if (weapon!=nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("(WeaponoNotifies) Weapon name: %s"),*(weapon->GetName()));
		if (weapon->Player != nullptr)
		{
			FString AnimationName = Animation->GetName();
			if (AnimationName.Compare("Fire_Pistol_W") == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("(WeaponoNotifies) Going to call DoneSimple Fire and ReduceAmmo"));
				weapon->Player->DoneSimpleFire();
				weapon->ReduceAmmo();
			}
			else if (AnimationName.Compare("Fire_Rifle_W") == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("(WeaponoNotifies) Going to call ReduceAmmo"));
				weapon->ReduceAmmo();
			}
			else if (AnimationName.Compare("Reload_Rifle_Hip_W") == 0 || AnimationName.Compare("Reload_Pistol_W") == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("(WeaponoNotifies) Going to call DoneReload"));
				weapon->Player->DoneReload();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("(WeaponoNotifies) Reference of player in weapon is null."));
		}
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("(WeaponoNotifies) weapon is null."));
	}
}