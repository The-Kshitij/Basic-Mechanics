// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpittingEnemy.generated.h"

UCLASS()
class TESTGAME_API ASpittingEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpittingEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
		FName SpittingPropertyName;
	UPROPERTY(EditAnywhere)
		FName SpawningPoint;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ADamagingBall> DamageBallClass;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetbSpitting(bool val);

	void SpawnDamagingBall();
private:
	float Health;
	UPROPERTY(EditAnywhere)
		float disappearingFactor = 0.01f;
	bool bSpitting;
	bool bDead;
	APawn* PlayerPawn;
	class UBlackboardComponent* currentBB;
	class AAIController* customAIController;
	class UMaterialInstanceDynamic* dynamicMat;

	UFUNCTION(BlueprintCallable)
		void SpitBall();
	UFUNCTION(BlueprintCallable)
		bool GetbSpitting();
	UFUNCTION(BlueprintCallable)
		bool GetbDead();

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
