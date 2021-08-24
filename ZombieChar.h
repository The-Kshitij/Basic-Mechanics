// Fill out your copyright notice in the Description page of Project Settings.

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

	void SetbAttack(bool val);
	void SetbDead(bool val);
	void SetbHit(bool val);

	UFUNCTION(BlueprintCallable)
		void AttackPlayer();

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


	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
