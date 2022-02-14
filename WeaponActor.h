#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

UCLASS()
class TESTGAME_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

	
	USkeletalMeshComponent* GetBodyMesh();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	bool bFiring;
	
	//returns if the weapon is firing or not
	UFUNCTION(BlueprintCallable)
		bool GetbFiring();
		
	UPROPERTY(EditAnywhere)
		int MaxAmmo;
	UPROPERTY(EditAnywhere)
		int MaxMag;

	int CurrentAmmo;
	int CurrentMag;

	//function for performing line trace 
	void LineTraceForDamage();

	class AMainPlayer* Player;
	void SetPlayerRef(AMainPlayer* player);
	
	//function to reduce the ammo of the player, like when a shot is fired
	void ReduceAmmo();
	
	//function to reload the weapon
	void Reload();

	// Called every frame
	virtual void Tick(float DeltaTime) override; 
	
	//although this can be done from code, but it was behaving slightly so doing this in blueprint
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void PlayReloadAnimation();
	
	//looping is set to true if this is an automatic weapon
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
		void FireGun(bool looping,bool shoulddelay);
	
	//stop firing the weapon
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void StopFiring();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		USkeletalMeshComponent* BodyMesh;
private:
	//parameters used for line tracing
	FCollisionQueryParams CollisionParams;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDamageType> DamageClass;
	UPROPERTY(EditAnywhere)
		bool bDamageTraceFromSocket = true;
	UPROPERTY(EditAnywhere)
		FName SocketName = "";
	UPROPERTY(EditAnywhere)
		float DamageRange = 150.0f;
	UPROPERTY(EditAnywhere)
		float DamageAmount = 20.0f;
	UPROPERTY(EditAnywhere)
		UAnimationAsset* ReloadAnimation;

};
