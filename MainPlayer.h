// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

UCLASS()
class TESTGAME_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	int  WeaponEquiped; // 0-None,1-Melee Type,2-Pistol type(Single shot),3 - Rifle type(Continuous Shot)


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetbAttacking(bool val);
	void SetbEquipingMelee(bool val, int weaponNum);
	void SetbUnEquipingMelee(bool val);
	void ChangePunchNum();
	void DoneAttackFunction();
	void AttachWeaponToHand();
	void AttachWeaponToBack(int val = 0);
	//void FireIfPossible();
	//for pistol type weapon
	void DoneSimpleFire();//called through anim notify
	void Reload(); // called by weaponActor
	void DoneReload();
	void SetAmmoAndMag(bool hide, int ammo, int mag);

	//inverts the current crouching state
	void CrouchFunction();

	//void SetbFiring(bool val);
	UFUNCTION(BlueprintImplementableEvent)
		void ShowDamageEffect();

private:
	bool bHasPistol;
	bool bHasRifle;
	bool bCrouching;
	bool bCanRotate;
	bool bAutomaticFire;
	bool bSimpleFiring;
	bool bMovingForward;//moving forward or backward
	bool bMovingRight;//moving right or left
	bool bStandingAttack; //when weapon equiped is 0,attacking and not moving, only then
	bool bJumping;
	bool bAttacking;
	bool bAiming;
	bool bEquipingMelee; //to be true when equiping a weapon from the back
	bool bUnEquipingMelee; //to be true when un equiping a weapon from the back
	bool bReloading;
	// since, using two animations for punching animation, called in blueprint to determine, which animation to play
	int PunchNum; 
	float Health;
	//int Ammo;//to store the ammo from the weapon
	//int Mag;//to store the mag from the weapon
	

	class UCameraComponent* CameraComponent;
	UCameraComponent* FPSCameraComponent;
	class AWeaponActor* BaseballPtr;
	AWeaponActor* PistolPtr;
	AWeaponActor* RiflePtr;
	UCharacterMovementComponent* CharacterMovement;
	TSubclassOf<UUserWidget> ScreenWidgetClass;
	class UCustomWidget* ScreenWidget;
	
	UPROPERTY(EditAnywhere)
		float CameraZoominInValue;
	UPROPERTY(EditAnywhere)
		float CameraNormalValue;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AWeaponActor> BaseballClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AWeaponActor> PistolClass;
	UPROPERTY(EditAnywhere)
		USoundBase* TakingHitSound;
	UPROPERTY(EditAnywhere)
		USoundBase* PoisonSound;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AWeaponActor> RifleClass;

	UFUNCTION(BlueprintCallable)
		bool GetbCrouching();
	UFUNCTION(BlueprintCallable)
		bool GetbJumping();
	UFUNCTION(BlueprintCallable)
		bool GetbAttacking();
	UFUNCTION(BlueprintCallable)
		bool GetbEquipingMelee();
	UFUNCTION(BlueprintCallable)
		bool GetbUnEquipingMelee();
	UFUNCTION(BlueprintCallable)
		bool GetbAiming();
	UFUNCTION(BlueprintCallable)
		bool GetbAutomaticFire();
	UFUNCTION(BlueprintCallable)
		bool GetbSimpleFiring();
	UFUNCTION(BlueprintCallable)
		bool GetbReloading();
	UFUNCTION(BlueprintCallable)
		int GetWeaponEquiped();
	UFUNCTION(BlueprintCallable)
		int GetPunchNum();
	UFUNCTION(BlueprintCallable)
		void SetbHasPistol(bool val);
	UFUNCTION(BlueprintCallable)
		void SetbHasRifle(bool val);

	void StartSprint();
	void EndSprint();

	void SetbAutomaticFire(bool val);

	void SetWeaponEquiped(int val);
	void SetbJumping(bool val);

	void MoveForwardBackward(float val);
	void MoveRightLeft(float val);

	void StartAttackFunction();
	void SetStandingAttack(bool val);

	void JumpFunction();


	void StartZoom();
	void DoneZoom();

	void SetbCanRotate(bool val);
	void RotatePlayer(float val);
	void SetbAiming(bool val);
	

	void EquipMelee();
	void EquipPistol();
	void EquipRifle();

	//for rifle type weapon
	void StartAutomaticFire();
	//for rifle type weapon
	void DoneAutomaticFire();

	//for pistol type weapon
	void StartSimpleFire();
	void SetbSimpleFire(bool val);

	void LineTraceForGround();

	void ReloadCustom(); // the reload function is called by the weapon and not the player directly, so this functions calls the reload on the current weapon

	void SwitchCameras();

	void SetbCrouching(bool val);

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

};
