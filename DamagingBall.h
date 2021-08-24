// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"

#include "DamagingBall.generated.h"

UCLASS()
class TESTGAME_API ADamagingBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamagingBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//maximum height that the projectile should go
	UPROPERTY(EditAnywhere)
		float HeightOfProjectile;
	//total time that the projectile should take to reach that spot
	UPROPERTY(EditAnywhere)
		float TotalTime;

	//x velocity for the projectile
	float xvel;
	//z velocity for the projectile
	float zvel;

	FVector TargetLocation;
	float TotalDistance;
	FVector InitialLocation;

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BodyMesh;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UDamageType> DamageClass;
	UPROPERTY(EditAnywhere)
		UCurveFloat* ZCurve;
	bool bRaising;
	float TimeOfSpawn;

	
	UFUNCTION(BlueprintCallable)
		void ApplyDamageToThisPawn(AActor* Player);
};
