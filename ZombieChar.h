/*
The zombie attacks the player if it saw the player, otherwise it moves randomly. This task of movement and perception of seeing the player is handled in blueprint using ai 
perception component, blackboard and assigning tasks in the behaviour tree. 
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieChar.generated.h"

UCLASS()
class TESTGAME_API AZombieChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//set attacking
	void SetbAttack(bool val);
	
	//set if dead or not
	void SetbDead(bool val);
	
	//set if hit
	void SetbHit(bool val);
	
	//function for attacking the player
	UFUNCTION(BlueprintCallable)
		void AttackPlayer();

	//function to perform line trace and apply damage to the player
	void LineTraceForDamage();

private:
	float Health;
	bool bAttacking;
	bool bDead;
	int AttackNum;
	bool bHit;
	
	class UBlackboardComponent* currentBB;
	class AAIController* customAIController;
	class UMaterialInstanceDynamic* dynamicMat;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDamageType> DamageClass;
	UPROPERTY(EditAnywhere)
		float RangeForDamage;
	UPROPERTY(EditAnywhere)
		float DamageToApply;
	UPROPERTY(EditAnywhere)
		float disappearingFactor = 0.01f;
	UFUNCTION(BlueprintCallable)
		bool GetbHit();
	UFUNCTION(BlueprintCallable)
		bool GetbAttacking();
	UFUNCTION(BlueprintCallable)
		bool GetbDead();
	UFUNCTION(BlueprintCallable)
		int GetAttackNum();

	//damage handler
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
