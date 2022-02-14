#include "MainPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
//#include "DrawDebugHelpers.h"

#include "WeaponActor.h"
#include "CustomWidget.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerScreenClass(TEXT("/Game/Widgets/BP_PlayerScreen"));
	ScreenWidgetClass = PlayerScreenClass.Class;
	if (ScreenWidgetClass==nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("(MainPlayer) screen widget class is nullptr., maybe you changed the path"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) screen widget class wound found."));
	}
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	//assigning some default values.
	bJumping = false;
	bAttacking = false;
	bMovingForward = false;
	bMovingRight = false;
	bStandingAttack = false;
	bEquipingMelee = false;
	bUnEquipingMelee = false;
	bSimpleFiring = false;
	bAiming = false;
	WeaponEquiped = 0;
	PunchNum = 0;
	Health = 100.0f;
	//getting the player's camera, for implementing the zoom.
	TArray<UCameraComponent*> Cameras;
	GetComponents(Cameras);
	for (UCameraComponent* c : Cameras)
	{
		if (c->GetName().Compare("PrimaryCamera") == 0)
		{
			CameraComponent = c;
			CameraComponent->Activate();
			UE_LOG(LogTemp,Warning,TEXT("(MainPlayer)Assigned primary cam."));
		}
		else if (c->GetName().Compare("FPSCamera") == 0)
		{
			FPSCameraComponent = c;
			UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Assigned fps cam."));
		}
	}

	//spawning and attaching the custom classes
	BaseballPtr = GetWorld()->SpawnActor<AWeaponActor>(BaseballClass);
	if (!BaseballPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("(MainPlayer)Couldn't spawn baseball ptr."));
		return;
	}
	BaseballPtr->SetPlayerRef(this);

	PistolPtr = GetWorld()->SpawnActor<AWeaponActor>(PistolClass);
	if (!PistolPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("(MainPlayer)Couldn't spawn pistol ptr."));
		return;
	}
	PistolPtr->SetPlayerRef(this);
	PistolPtr->BodyMesh->SetVisibility(false);

	RiflePtr = GetWorld()->SpawnActor<AWeaponActor>(RifleClass);
	if (!RiflePtr)
	{
		UE_LOG(LogTemp, Error, TEXT("(MainPlayer)Couldn't spawn Rifle ptr."));
		return;
	}
	RiflePtr->SetPlayerRef(this);
	RiflePtr->BodyMesh->SetVisibility(false);

	AttachWeaponToBack();

	CharacterMovement = GetCharacterMovement();
	if (CharacterMovement == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("(MainPlayer)CharacterMovement is null."));
	}
	else
	{
		CharacterMovement->MaxWalkSpeed = 180.0f;
	}

	//constructing the player's screen
	if (ScreenWidgetClass!=nullptr)
	{
		UGameInstance* gameInstance = GetGameInstance();
		if (gameInstance == nullptr)
		{
			UE_LOG(LogTemp,Error,TEXT("(MainPlayer)Got null for game instance"));
		}
		else
		{
			ScreenWidget = CreateWidget<UCustomWidget>(gameInstance, ScreenWidgetClass);
			if (ScreenWidget) ScreenWidget->AddToViewport();
			SetAmmoAndMag(true,0,0);
		}
	}

}

void AMainPlayer::SwitchCameras()
{
	if (CameraComponent == nullptr || FPSCameraComponent == nullptr) return;

	if (CameraComponent->IsActive())
	{
		FPSCameraComponent->Activate();
		CameraComponent->Deactivate();
	}
	else
	{
		CameraComponent->Activate();
		FPSCameraComponent->Deactivate();
	}
}

void AMainPlayer::SetAmmoAndMag(bool hide,int ammo,int mag)
{
	if (ScreenWidget != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Setting widget."));
		ScreenWidget->SetTextAmmo(ammo);
		ScreenWidget->SetTextMag(mag);
		ScreenWidget->SetHealth(Health);
		if (hide)
		{
			ScreenWidget->HideUI();
		}
		else
		{
			ScreenWidget->ShowUI();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("(MainPlayer)Widget not found."));
	}
}

void AMainPlayer::AttachWeaponToHand()
{
	UE_LOG(LogTemp,Warning,TEXT("(MainPlayer)Attaching weapon to hand"));
	switch (WeaponEquiped)
	{
	case 1:
		if (BaseballPtr)
		{
			BaseballPtr->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			BaseballPtr->GetBodyMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("BaseballSocket"));
		}
		break;
	case 2:
		if (PistolPtr)
		{
			PistolPtr->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			PistolPtr->GetBodyMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("PistolSocket"));
		}
		break;
	case 3:
		if (RiflePtr)
		{
			RiflePtr->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			RiflePtr->GetBodyMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RifleSocket"));
		}
		break;
		
	default:
		UE_LOG(LogTemp,Error,TEXT("(MainPlayer)No attaching case for this weaponequiped."));
		break;
	}
}

void AMainPlayer::AttachWeaponToBack(int val)
{
	UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Attaching weapon to back"));
	switch (val)
	{
	case 0:
		UE_LOG(LogTemp,Warning,TEXT("Attaching all weapons to the back."));
		if (BaseballPtr)
		{
			BaseballPtr->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			BaseballPtr->GetBodyMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("BaseballEquipSocket"));
		}
		if (PistolPtr)
		{
			PistolPtr->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			PistolPtr->GetBodyMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("PistolEquipSocket"));
		}
		if (RiflePtr)
		{
			RiflePtr->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			RiflePtr->GetBodyMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RifleEquipSocket"));
		}
		break;

	case 1:
		if (BaseballPtr)
		{
			BaseballPtr->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			BaseballPtr->GetBodyMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("BaseballEquipSocket"));
		}
	case 2:
		if (PistolPtr)
		{
			PistolPtr->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			PistolPtr->GetBodyMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("PistolEquipSocket"));
		}
		break;
	case 3:
		if (RiflePtr)
		{
			RiflePtr->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			RiflePtr->GetBodyMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RifleEquipSocket"));
		}
		break;

	default:
		UE_LOG(LogTemp, Error, TEXT("(MainPlayer)No attaching case for this weaponequiped."));
		break;
	}
}


// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bJumping)
	{
		LineTraceForGround();
	}
	/*if (bEquipingMelee)
	{
		UE_LOG(LogTemp,Warning,TEXT("(MainPlayer) EquipingMelee is true."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) EquipingMelee is false."));
	}

	if (bUnEquipingMelee)
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) UnEquipingMelee is true."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) UnEquipingMelee is false."));
	}*/
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//axis bindings
	PlayerInputComponent->BindAxis(TEXT("Forward"),this,&AMainPlayer::MoveForwardBackward);
	PlayerInputComponent->BindAxis(TEXT("Right"),this,&AMainPlayer::MoveRightLeft);
	PlayerInputComponent->BindAxis(TEXT("Rotation"),this,&AMainPlayer::RotatePlayer);
	

	//action bindings
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AMainPlayer::StartAutomaticFire);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Released, this, &AMainPlayer::DoneAutomaticFire);
	PlayerInputComponent->BindAction(TEXT("Attack"),IE_Pressed,this,&AMainPlayer::StartSimpleFire);
	PlayerInputComponent->BindAction(TEXT("Attack"),IE_Pressed,this,&AMainPlayer::StartAttackFunction);
	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed,this,&AMainPlayer::JumpFunction);
	PlayerInputComponent->BindAction(TEXT("Zoom"),IE_Pressed,this,&AMainPlayer::StartZoom);
	PlayerInputComponent->BindAction(TEXT("Zoom"),IE_Released,this,&AMainPlayer::DoneZoom);
	PlayerInputComponent->BindAction(TEXT("EquipMelee"),IE_Released,this,&AMainPlayer::EquipMelee);
	PlayerInputComponent->BindAction(TEXT("EquipPistol"),IE_Released,this,&AMainPlayer::EquipPistol);
	PlayerInputComponent->BindAction(TEXT("EquipRifle"),IE_Released,this,&AMainPlayer::EquipRifle);
	PlayerInputComponent->BindAction(TEXT("Sprint"),IE_Pressed,this,&AMainPlayer::StartSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"),IE_Released,this,&AMainPlayer::EndSprint);
	PlayerInputComponent->BindAction(TEXT("Reload"),IE_Pressed,this,&AMainPlayer::ReloadCustom);
	PlayerInputComponent->BindAction(TEXT("ChangeCam"),IE_Pressed,this,&AMainPlayer::SwitchCameras);
	PlayerInputComponent->BindAction(TEXT("Crouch"),IE_Pressed,this,&AMainPlayer::CrouchFunction);
}

void AMainPlayer::StartAutomaticFire()
{
	if (!bReloading && !bJumping && !bEquipingMelee && !bUnEquipingMelee)
	{
		if (RiflePtr == nullptr) return;

		if (RiflePtr->CurrentAmmo > 0)
		{
			if (WeaponEquiped == 3)
			{
				UE_LOG(LogTemp, Warning, TEXT("Starting Continous firing"));
				SetbAutomaticFire(true);
				RiflePtr->FireGun(true, true);
			}
		}
	}
}

void AMainPlayer::DoneAutomaticFire()
{
	if (RiflePtr == nullptr) return;
	if (WeaponEquiped == 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) Ending Continous firing"));
		SetbAutomaticFire(false);
		RiflePtr->StopFiring();
	}
}

bool AMainPlayer::GetbAutomaticFire()
{
	return bAutomaticFire;
}

void AMainPlayer::SetbAutomaticFire(bool val)
{
	bAutomaticFire = val;
	if (RiflePtr == nullptr) return;

	RiflePtr->bFiring = val;
}

void AMainPlayer::StartSprint()
{
	UE_LOG(LogTemp,Warning,TEXT("(MainPlayer) Starting sprint"));
	if (CharacterMovement == nullptr) return;

	CharacterMovement->MaxWalkSpeed = 500.0f;
}

void AMainPlayer::EndSprint()
{
	UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) Ending sprint"));

	if (CharacterMovement == nullptr) return;

	CharacterMovement->MaxWalkSpeed = 180.0f;
}

void AMainPlayer::StartZoom()
{
	UE_LOG(LogTemp,Warning,TEXT("(MainPlayer)Starting to zoom"));
	if (CameraComponent == nullptr) return;
	CameraComponent->SetFieldOfView(CameraZoominInValue);

	//stop the player from rotating character when jumping
	if (!bJumping)
	{
		SetbCanRotate(true);
		if (!bEquipingMelee && !bUnEquipingMelee && !bReloading)
		{
			if (WeaponEquiped == 2) SetbAiming(true);
		}
	}
}

void AMainPlayer::DoneZoom()
{
	UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Done zooming"));
	if (CameraComponent == nullptr) return;
	CameraComponent->SetFieldOfView(CameraNormalValue);

	if (!bJumping)
	{
		SetbCanRotate(false); 
		if (!bEquipingMelee && !bUnEquipingMelee && !bReloading)
		{
			if (WeaponEquiped == 2) SetbAiming(false);
		}
	}
}

bool AMainPlayer::GetbAiming()
{
	return bAiming;
}

void AMainPlayer::SetbAiming(bool val)
{
	bAiming = val;
	if (val)
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Setting aiming to true."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Setting aiming to false."));
	}
}

void AMainPlayer::SetbCanRotate(bool val)
{
	bCanRotate = val;
	if (val)
	{
		UE_LOG(LogTemp,Warning,TEXT("(MainPlayer)Allowing rotation."));
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("(MainPlayer)Disabling rotation."));
	}
}

void AMainPlayer::RotatePlayer(float val)
{
	if (bCanRotate) AddControllerYawInput(val);
}

void AMainPlayer::MoveForwardBackward(float val)
{
	//to avoid moving if in between a punching animation.
	if (!bStandingAttack)
	{
		if (val != 0)
		{
			AddMovementInput(val * GetActorForwardVector());
			bMovingForward = true;
		}
		else
		{
			bMovingForward = false;
		}
	}
}

void AMainPlayer::MoveRightLeft(float val)
{
	//to avoid moving if in between a punching animation.
	if (!bStandingAttack)
	{
		if (val != 0)
		{
			AddMovementInput(val * GetActorRightVector());
			bMovingRight = true;
		}
		else
		{
			bMovingRight = false;
		}
	}
}

//bool AMainPlayer::GetbFiring()
//{
//	return bFiring;
//}

//void AMainPlayer::SetbFiring(bool val)
//{
//	bFiring = val;
//	SetbAttacking(val);
//	if (val)
//	{
//		UE_LOG(LogTemp,Warning,TEXT("(MainPlayer) setting bFiring to true"));
//		if (WeaponEquiped == 2)
//		{
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp,Warning,TEXT("(MainPlayer) setting bFiring to false"));
//	}
//}

/*
void AMainPlayer::FireIfPossible()
{	
	if (bAttacking)
	{
		if (PistolPtr && WeaponEquiped == 2)
		{
			PistolPtr->FireGun(false);
			DoneAttackFunction();
		}
	}
}
*/

void AMainPlayer::JumpFunction()
{
	if (!bJumping && !bEquipingMelee && !bUnEquipingMelee && !bReloading)
	{
		ACharacter::Jump();
		//if jumping, then we want to stop all other animations, only allow them to zoom in or out.
		SetbJumping(true);
		DoneAutomaticFire();
		DoneAttackFunction();
		DoneSimpleFire();
		SetbAiming(false);
	}
	
}

void AMainPlayer::LineTraceForGround()
{
	FVector StartLocation = GetActorLocation();
	StartLocation.Z -= 50.0f;
	FVector EndLocation = StartLocation;
	EndLocation.Z -= 45;
	//DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,true,2.0f);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	FHitResult hitResult;
	if (GetWorld()->LineTraceSingleByChannel(hitResult,StartLocation,EndLocation,ECollisionChannel::ECC_WorldStatic,CollisionParams))
	{
		UE_LOG(LogTemp,Warning,TEXT("has hit %s"),*(hitResult.GetActor()->GetName()));
		bJumping = false;
	}
}

bool AMainPlayer::GetbJumping()
{
	return bJumping;
}

void AMainPlayer::SetbJumping(bool val)
{
	bJumping = val;
	if (val)
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) set jumping to true"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) set jumping to false"));
	}
}

void AMainPlayer::StartSimpleFire()
{
	if (WeaponEquiped == 2 && !bSimpleFiring && !bJumping && !bReloading && !bEquipingMelee && !bUnEquipingMelee)
	{
		if (PistolPtr == nullptr) return;

		if (PistolPtr->CurrentAmmo > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) Starting Simple Fire"));
			SetbSimpleFire(true);
			//if the player is already aiming then we can simply shoot the pistol, otherwise
			//first there has to be a transition into the firing pose and then the pistol should be fired.
			if (bAiming)
			{
				PistolPtr->FireGun(false, false);
			}
			else
			{
				PistolPtr->FireGun(false, true);
			}
		}
	}
}

void AMainPlayer::DoneSimpleFire()
{
	UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) Stopping Simple Fire"));
	SetbSimpleFire(false);
}

void AMainPlayer::SetbSimpleFire(bool val)
{
	if (WeaponEquiped == 2)
	{
		bSimpleFiring = val;
		if (val)
		{
			UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) Setting bSimpleFire to true"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) Setting bSimpleFire to false"));
		}
	}
}

bool AMainPlayer::GetbSimpleFiring()
{
	return bSimpleFiring;
}


int AMainPlayer::GetWeaponEquiped()
{
	return WeaponEquiped;
}

void AMainPlayer::SetWeaponEquiped(int val)
{
	WeaponEquiped = val;
	switch (val)
	{
	case 2:
		if (PistolPtr && ScreenWidget)
		{
			ScreenWidget->SetTextAmmo(PistolPtr->CurrentAmmo);
			ScreenWidget->SetTextMag(PistolPtr->CurrentMag * PistolPtr->MaxAmmo);
		}
		break;
	
	case 3:
		if (RiflePtr && ScreenWidget)
		{
			ScreenWidget->SetTextAmmo(RiflePtr->CurrentAmmo);
			ScreenWidget->SetTextMag(RiflePtr->CurrentMag * RiflePtr->MaxAmmo);
		}
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)This weapon doesn't have ammo and mag"));
	}
	UE_LOG(LogTemp,Warning,TEXT("(MainPlayer) Set weapon equiped to %d"),val);
}

bool AMainPlayer::GetbAttacking()
{
	return bAttacking;
}

void AMainPlayer::SetbAttacking(bool val)
{
	bAttacking = val;
	if (val)
	{
		UE_LOG(LogTemp,Warning,TEXT("(MainPlayer) Set bAttacking to true."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) Set bAttacking to false."));
	}
}

void AMainPlayer::StartAttackFunction()
{
	if (WeaponEquiped != 3 && !bSimpleFiring && !bJumping && !bEquipingMelee && !bUnEquipingMelee && !bReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Equiped: %d"), WeaponEquiped);
		if (!bJumping && !bAttacking && !bEquipingMelee && !bUnEquipingMelee && !bAiming && !(WeaponEquiped == 2))
		{
			UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Starting to attack"));
			SetbAttacking(true);
			if ((WeaponEquiped == 0 || WeaponEquiped == 1) && bAttacking && !bMovingForward && !bMovingRight)
			{
				SetStandingAttack(true);
			}
		}
		//else if (!bJumping && !bAttacking && !bEquipingMelee && !bUnEquipingMelee && WeaponEquiped == 2)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) going to set firing to true"));
		//	SetbAttacking(true);

		//	//don't want the player to move in between a standing still firing animation.
		//	/*if (!bMovingForward && !bMovingRight)
		//	{
		//		SetStandingAttack(true);
		//	}*/
		//	//SetbFiring(true);
		//}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("(MainPlayer) No case for this type of attack"));
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("(MainPlayer) Either WeaponEquiped is 3 or bSimpleFiring is true"));
	}
}

void AMainPlayer::SetStandingAttack(bool val)
{
	bStandingAttack = val;
	if (val)
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Setting standing attack to true."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Setting standing attack to false."));
	}
}

void AMainPlayer::DoneAttackFunction()
{
	if (WeaponEquiped != 3)
	{
		SetbAttacking(false);
		SetStandingAttack(false);
		//SetbFiring(false);
		bMovingForward = false;
		bMovingRight = false;
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Completed done attack function."));
	}
}

//used to determine the number for standing still punches.
int AMainPlayer::GetPunchNum()
{
	return PunchNum;
}

void AMainPlayer::ChangePunchNum()
{
	//only to change the punch number after the current animation is done playing, otherwise no animation will be able to complete itself.
	if (!bAttacking)
	{
		PunchNum++;
		PunchNum = PunchNum > 1 ? 0 : PunchNum;
	}
}

void AMainPlayer::EquipMelee()
{
	UE_LOG(LogTemp,Warning,TEXT("(MainPlayer)Starting to decide the type of melee equip"));
	//avoid calling when in between any other animation.
	if (!bJumping && !bAttacking && !bEquipingMelee && !bUnEquipingMelee && !bAutomaticFire && !bReloading)
	{
		//if the below is true, that means we already have a melee weapon equiped, and the player wants to
		//unequip it.
		if (WeaponEquiped == 1)
		{
			UE_LOG(LogTemp,Warning,TEXT("(MainPlayer)Decided, it's unequiping melee"));
			SetbUnEquipingMelee(true);
			SetbEquipingMelee(false,1);
		}
		else if (WeaponEquiped == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Decided, it's equiping melee"));
			SetbUnEquipingMelee(false);
			SetbEquipingMelee(true,1);
			SetAmmoAndMag(true,0,0);
		}
		else
		{
			UE_LOG(LogTemp,Error,TEXT("(MainPlayer)Couldn't decide the type of weapon."));
		}
	}
}

void AMainPlayer::EquipPistol()
{
	if (bHasPistol)
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Starting to decide the type of pistol equip"));
		//avoid calling when in between any other animation.
		if (!bJumping && !bAttacking && !bEquipingMelee && !bUnEquipingMelee && !bAutomaticFire && !bReloading)
		{
			//if the below is true, that means we already have a pistol equiped, and the player wants to
			//unequip it.
			if (WeaponEquiped == 2)
			{
				UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Decided, it's unequiping pistol"));
				SetbUnEquipingMelee(true);
				SetbEquipingMelee(false, 2);
			}
			else if (WeaponEquiped == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Decided, it's equiping pistol"));
				SetbUnEquipingMelee(false);
				SetbEquipingMelee(true, 2);
				if (PistolPtr) SetAmmoAndMag(false, PistolPtr->CurrentAmmo, PistolPtr->CurrentMag * PistolPtr->MaxAmmo);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("(MainPlayer)Couldn't decide the type of weapon."));
			}
		}
	}
}

void AMainPlayer::EquipRifle()
{
	if (bHasRifle)
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Starting to decide the type of rifle equip"));
		//avoid calling when in between any other animation.
		if (!bJumping && !bAttacking && !bEquipingMelee && !bUnEquipingMelee && !bAutomaticFire && !bReloading)
		{
			//if the below is true, that means we already have a rifle equiped, and the player wants to
			//unequip it.
			if (WeaponEquiped == 3)
			{
				UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Decided, it's unequiping rifle"));
				SetbUnEquipingMelee(true);
				SetbEquipingMelee(false, 3);
			}
			else if (WeaponEquiped == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)Decided, it's equiping rifle"));
				SetbUnEquipingMelee(false);
				SetbEquipingMelee(true, 3);
				if (RiflePtr) SetAmmoAndMag(false, RiflePtr->CurrentAmmo, RiflePtr->CurrentMag * RiflePtr->MaxAmmo);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("(MainPlayer)Couldn't decide the type of weapon."));
			}
		}
	}
}

bool AMainPlayer::GetbEquipingMelee()
{
	return bEquipingMelee;
}

bool AMainPlayer::GetbUnEquipingMelee()
{
	return bUnEquipingMelee;
}

void AMainPlayer::SetbEquipingMelee(bool val, int weaponNum)
{
	bEquipingMelee = val;
	if (val)
	{
		UE_LOG(LogTemp,Warning,TEXT("(MainPlayer)Equiping melee weapon"));
		SetWeaponEquiped(weaponNum);
	}
}

void AMainPlayer::SetbUnEquipingMelee(bool val)
{
	bUnEquipingMelee = val;
	if (val)
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)UnEquiping weapon"));
	}
	else if (!val)
	{
		SetWeaponEquiped(0);
	}
}

void AMainPlayer::Reload()
{

	if (!bReloading && (WeaponEquiped == 2 || WeaponEquiped == 3))
	{
		int mag = 0;
		bool bCanReload = true;
		if (WeaponEquiped == 2 && PistolPtr != nullptr)
		{
			if (PistolPtr->CurrentMag != 0)
			{
				mag = PistolPtr->CurrentMag;
				ScreenWidget->SetTextAmmo(PistolPtr->MaxAmmo);
				ScreenWidget->SetTextMag(PistolPtr->MaxAmmo * (PistolPtr->CurrentMag - 1));
				PistolPtr->CurrentAmmo = PistolPtr->MaxAmmo;
				PistolPtr->CurrentMag = PistolPtr->MaxMag - 1;
			}
			else
			{
				bCanReload = false;
			}
		}
		if (WeaponEquiped == 3 && RiflePtr != nullptr)
		{
			if (RiflePtr->CurrentMag != 0)
			{
				mag = RiflePtr->CurrentMag;
				ScreenWidget->SetTextAmmo(RiflePtr->MaxAmmo);
				ScreenWidget->SetTextMag(RiflePtr->MaxAmmo * (RiflePtr->CurrentMag - 1));
				RiflePtr->CurrentAmmo = RiflePtr->MaxAmmo;
				RiflePtr->CurrentMag = RiflePtr->MaxMag - 1;
			}
			else
			{
				bCanReload = false;
			}
		}
		if (bCanReload)
		{
			bReloading = true;
			SetbSimpleFire(false);
			DoneAttackFunction();
			DoneAutomaticFire();
			SetbAiming(false);

			if (ScreenWidget)
			{
				ScreenWidget->HideUI();
			}
			UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) Current weapon's mag: %d"), mag);
		}
		else
		{
			UE_LOG(LogTemp,Error,TEXT("(MainPlayer) Can't reload the weapon"));
		}
	}
}

void AMainPlayer::DoneReload()
{
	bReloading = false;
	if (ScreenWidget)
	{
		ScreenWidget->ShowUI();
	}
}

bool AMainPlayer::GetbReloading()
{
	return bReloading;
}

void AMainPlayer::ReloadCustom()
{
	if (!bEquipingMelee && !bUnEquipingMelee && !bReloading)
	{
		if (WeaponEquiped == 2 && PistolPtr!=nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)  Going to call reload on Pistol."));
			PistolPtr->Reload();
		}
		else if (WeaponEquiped == 3 && RiflePtr != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("(MainPlayer)  Going to call reload on Rifle."));
			RiflePtr->Reload();
		}
		else
		{
			UE_LOG(LogTemp,Error,TEXT("(MainPlayer) This weapon doesn't support reloading."));
		}
	}
}

bool AMainPlayer::GetbCrouching()
{
	return bCrouching;
}

void AMainPlayer::SetbCrouching(bool val)
{
	bCrouching = val;
}

void AMainPlayer::CrouchFunction()
{
	bCrouching = !bCrouching;
}

void AMainPlayer::SetbHasPistol(bool val)
{
	bHasPistol = val;
	if (val)
	{
		UE_LOG(LogTemp, Warning, TEXT("(MainPlayer) Has found pistol"));
		if (PistolPtr != nullptr && PistolPtr->BodyMesh != nullptr)
		{
			PistolPtr->BodyMesh->SetVisibility(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("(Mainplayer) pistol has been hidden"));
		if (PistolPtr != nullptr && PistolPtr->BodyMesh != nullptr)
		{
			PistolPtr->BodyMesh->SetVisibility(false);
		}
	}
}

void AMainPlayer::SetbHasRifle(bool val)
{
	bHasRifle = val;
	if (val)
	{
		UE_LOG(LogTemp,Warning,TEXT("(MainPlayer) Has found rifle"));
		if (RiflePtr != nullptr && RiflePtr->BodyMesh != nullptr)
		{
			RiflePtr->BodyMesh->SetVisibility(true);
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("(Mainplayer) Rifle has been hidden"));
		if (RiflePtr != nullptr && RiflePtr->BodyMesh != nullptr)
		{
			RiflePtr->BodyMesh->SetVisibility(false);
		}
	}
}

float AMainPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= DamageAmount;
	if (DamageAmount == 5.0f)
	{
		ShowDamageEffect();
		if (PoisonSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(),PoisonSound);
		}
	}
	if (Health < 0)
	{
		APlayerController* controllerOfThis = UGameplayStatics::GetPlayerController(GetWorld(),0);
		if (controllerOfThis)
		{
			controllerOfThis->RestartLevel();
		}
	}
	if (ScreenWidget != nullptr)
	{
		ScreenWidget->SetHealth(Health);
		if (TakingHitSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), TakingHitSound);
		}
	}
	return DamageAmount;
}
