/*
This type of enemy maintains its distance from the player and spits a ball of poison at the player after certain time intervals
*/
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

	//spawns the ball of poison
	void SpawnDamagingBall();
private:
	float Health;
	
	//controls the rate at which the body of the enemy dissociates after it is dead
	UPROPERTY(EditAnywhere)
		float disappearingFactor = 0.01f;
	bool bSpitting;
	bool bDead;
	APawn* PlayerPawn;
	class UBlackboardComponent* currentBB;
	class AAIController* customAIController;
	class UMaterialInstanceDynamic* dynamicMat;

	//function to spit a ball of poison at the player
	UFUNCTION(BlueprintCallable)
		void SpitBall();
		
	//returns if the enemy is spitting or not
	UFUNCTION(BlueprintCallable)
		bool GetbSpitting();
		
	//returns if the enemy is dead
	UFUNCTION(BlueprintCallable)
		bool GetbDead();

	//damage handler of this enemy, called when enemy receives a damage
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
