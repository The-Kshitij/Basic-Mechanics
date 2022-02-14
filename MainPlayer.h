/*
The player can use a pistol or a melee weapon to damage the enemies.
There are several punching animations.
On right clicking , the camera is zoomed in, like when the player is aiming.
The player can use two types of weapon, an automatic weapon(like rifle) or a pistol
The player can rotate only when the player is right clicking
*/
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

	//used to set attacking
	void SetbAttacking(bool val);
	
	//used to set the melee weapon like fist, baseball bat
	void SetbEquipingMelee(bool val, int weaponNum);
	
	//is set to true when the player is unequiping a weapon, as we dont want the player to attack while changing weapons
	void SetbUnEquipingMelee(bool val);
	
	//There is a combo for punching, so this is handling that
	void ChangePunchNum();
	
	//called to set done attacking after attacking has been done
	void DoneAttackFunction();
	
	//When an animation of equping a weapon is playing then at the end of the animation this function is called to attach the weapon to player's hand
	void AttachWeaponToHand();
	
	//When an animation of unequping a weapon is playing then at the end of the animation this function is called to attach the weapon to player's back
	void AttachWeaponToBack(int val = 0);
	
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
	bool bAutomaticFire;//since we have two types of weapon implemented
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
		float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AWeaponActor> BaseballClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AWeaponActor> PistolClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AWeaponActor> RifleClass;
		
	
	//sound to be player when the player is hit by an enemy
	UPROPERTY(EditAnywhere)
		USoundBase* TakingHitSound;
	//sound to be player when a zombie spits poision at the player
	UPROPERTY(EditAnywhere)
		USoundBase* PoisonSound;
	

	//returns crouching
	UFUNCTION(BlueprintCallable)
		bool GetbCrouching();
		
	//returns jumping
	UFUNCTION(BlueprintCallable)
		bool GetbJumping();
		
	//returns attacking
	UFUNCTION(BlueprintCallable)
		bool GetbAttacking();
		
	//returns if player is in transition of equiping a melee weapon
	UFUNCTION(BlueprintCallable)
		bool GetbEquipingMelee();
		
	//returns if player is in transition of unequiping a melee weapon
	UFUNCTION(BlueprintCallable)
		bool GetbUnEquipingMelee();
	
	//returns if player is aiming
	UFUNCTION(BlueprintCallable)
		bool GetbAiming();
	
	//returns true if player is using an automatic firing weapon like a rifle
	UFUNCTION(BlueprintCallable)
		bool GetbAutomaticFire();
		
	//returns true if player is using a single shot weapon like a pistol
	UFUNCTION(BlueprintCallable)
		bool GetbSimpleFiring();
	
	//returns true if player is reloading the weapon
	UFUNCTION(BlueprintCallable)
		bool GetbReloading();
	
	//returns the weapon equiped
	UFUNCTION(BlueprintCallable)
		int GetWeaponEquiped();
	
	//returns the index of the punch being performed. As there are multiple punching animations
	UFUNCTION(BlueprintCallable)
		int GetPunchNum();
	
	//sets the property if player is using a Pistol or not
	UFUNCTION(BlueprintCallable)
		void SetbHasPistol(bool val);
		
	//sets the property if player is using a rifle or not
	UFUNCTION(BlueprintCallable)
		void SetbHasRifle(bool val);

	//called when player wants to start sprinting
	void StartSprint();
	
	//called when player wants to end sprinting
	void EndSprint();

	//sets if player is firing an automatic weapon
	void SetbAutomaticFire(bool val);

	//sets variable for the weapon type equiped
	void SetWeaponEquiped(int val);
	
	//sets jumping true or false
	void SetbJumping(bool val);

	//to  move forward or backward
	void MoveForwardBackward(float val);
	
	//to  move left or right
	void MoveRightLeft(float val);

	//called when player wants to attack
	void StartAttackFunction();
	
	//sets variable which handles if player is performing a standing attack or not
	void SetStandingAttack(bool val);

	//called when the player wants to jump
	void JumpFunction();

	//Called in situtations when the camera should zoom in like when aiming
	void StartZoom();
	
	//Called to stop zooming in and set camera to original fov
	void DoneZoom();

	//called to enable rotation of player
	void SetbCanRotate(bool val);
	
	//function which handles the rotation of the  player
	void RotatePlayer(float val);
	
	//sets the baiming variable
	void SetbAiming(bool val);
	
	//called when player wants to equip a melee weapon
	void EquipMelee();
	
	//called when player wants to equip a pistol
	void EquipPistol();
	
	//called when player wants to equip a rifle
	void EquipRifle();

	//for rifle type weapon
	void StartAutomaticFire();
	//for rifle type weapon
	void DoneAutomaticFire();

	//for pistol type weapon
	void StartSimpleFire();
	void SetbSimpleFire(bool val);

	//to check for ground, this is used to see if the player is in air, like when jumping or falling from some height
	void LineTraceForGround();

	void ReloadCustom(); // the reload function is called by the weapon and not the player directly, so this functions calls the reload on the current weapon

	//To switch between the first person and third person camera
	void SwitchCameras();

	//called to set bcrouching
	void SetbCrouching(bool val);

	//handles the damage taken by player, also checks if the player's health has fallen below 0, i.e if the player is dead
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

};
