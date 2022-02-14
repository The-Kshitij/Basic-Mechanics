#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "WeaponoNotifies.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME_API UWeaponoNotifies : public UAnimNotify
{
	GENERATED_BODY()
private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	
};
