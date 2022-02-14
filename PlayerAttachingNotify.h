/*
Called when a weapon is being equiped or unequiped
*/
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PlayerAttachingNotify.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME_API UPlayerAttachingNotify : public UAnimNotify
{
	GENERATED_BODY()
private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
