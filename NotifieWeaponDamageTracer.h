/*
This is called when a weapon fires
*/
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NotifieWeaponDamageTracer.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME_API UNotifieWeaponDamageTracer : public UAnimNotify
{
	GENERATED_BODY()
private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	
};
