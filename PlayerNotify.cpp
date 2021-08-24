// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNotify.h"
#include "MainPlayer.h"

void UPlayerNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AMainPlayer* player = Cast<AMainPlayer>(MeshComp->GetOwner());
	if (player == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Casting to player failed in PlayerNotify.cpp. "));
		return;
	}
	
	FString AnimationName = Animation->GetName();

	//these are animations that are used for punching.
	if (AnimationName.Compare("P_Punching1") == 0 || AnimationName.Compare("P_Punching2") == 0 || AnimationName.Compare("P_Running_Punch") == 0)
	{
		player->DoneAttackFunction();

		//only call change punch after done attack because, it works only if attacking is set to false, which is
		//done in ChangePunchNum.
		player->ChangePunchNum();
	}
	else if (AnimationName.Compare("P_Melee_Attack2") == 0 || AnimationName.Compare("P_Melee_Attack1") == 0)
	{
		player->DoneAttackFunction();
	}
	else if (AnimationName.Compare("P_Equip_from_back") == 0 || AnimationName.Compare("P_GrabPistol") == 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("(PlayerNotify) Setting EquipMelee to false"));
		//for SetbEquipingMelee, the integer matters only if the boolean is true
		player->SetbEquipingMelee(false,0);
	}
	else if (AnimationName.Compare("P_UnEquipFromBack") == 0 || AnimationName.Compare("P_UnequipPistol") == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("(PlayerNotify) Setting UnEquipMelee to false"));
		player->SetbUnEquipingMelee(false);
	}
	/*else if (AnimationName.Compare("P_PistolAim") == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("(PlayerNotify) Calling FireIfPossible."));
		player->FireIfPossible();
	}*/
	/*else if (AnimationName.Compare("P_PistolAim_Moving") == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("(PlayerNotify) Calling DoneAttack."));
		player->DoneAttackFunction();
	}*/
	else
	{
		UE_LOG(LogTemp,Error,TEXT("No case in PlayerNotify found for this AnimationName."));
	}
}
