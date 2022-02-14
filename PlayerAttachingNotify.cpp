#include "PlayerAttachingNotify.h"

#include "MainPlayer.h"

void UPlayerAttachingNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AMainPlayer* player = Cast<AMainPlayer>(MeshComp->GetOwner());
	if (player == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Casting to player failed in PlayerNotify.cpp. "));
		return;
	}

	FString AnimationName = Animation->GetName();
	/*
	The code below is dependent on the animation
	*/
	if (AnimationName.Compare("P_Equip_from_back") == 0 || AnimationName.Compare("P_GrabPistol") == 0)
	{
		player->AttachWeaponToHand();
	}
	else if (AnimationName.Compare("P_UnEquipFromBack") == 0 || AnimationName.Compare("P_UnequipPistol") == 0)
	{
		player->AttachWeaponToBack();
	}
}
