// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "MainPlayer.h"
#include "Kismet/GameplayStatics.h"
//#include "DrawDebugHelpers.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body of weapon"));
	SetRootComponent(BodyMesh);
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
	CurrentMag = MaxMag;

	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USkeletalMeshComponent* AWeaponActor::GetBodyMesh()
{
	return BodyMesh;
}

void AWeaponActor::SetPlayerRef(AMainPlayer* player)
{
	if (player == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("(WeaponActor) Null supplied as player reference"));
		return;
	}

	Player = Cast<AMainPlayer>(player);
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("(WeaponActor) Casting to player failed."));
		return;
	}
}

void AWeaponActor::ReduceAmmo()
{
	CurrentAmmo--;

	if (Player == nullptr) return;

	Player->SetAmmoAndMag(false, CurrentAmmo, CurrentMag * MaxAmmo);

	if (CurrentAmmo == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("(WeaponActor) Going to call reload"));
		Reload();
	}
}


//called when the current ammo is zero or the player wants to reload by themself.
void AWeaponActor::Reload()
{
	if (BodyMesh == nullptr || Player==nullptr) return;

	UE_LOG(LogTemp,Warning,TEXT("(WeaponActor) Starting to reload"));
	StopFiring();
	//just as a precaution, rather than having current mag !=0 doing this, because it can be somehow that the current mag becomes less than zero.
	if (CurrentAmmo != MaxAmmo && CurrentMag > 0)
	{
		if (!BodyMesh->IsActive())
		{
			UE_LOG(LogTemp, Warning, TEXT("(WeaponActor) Body of weapon was not active"));
			BodyMesh->Activate();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("(WeaponActor) Body of weapon was active"));
		}
		PlayReloadAnimation();
		Player->Reload();
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("(WeaponActor) Can't reload, weapon ammo: %d, weapon mag: %d"),CurrentAmmo,CurrentMag);
	}
}

bool AWeaponActor::GetbFiring()
{
	return bFiring;
}

void AWeaponActor::LineTraceForDamage()
{
	FHitResult hitResult;
	FVector StartPoint;
	FVector EndPoint;
	if (bDamageTraceFromSocket)
	{
		StartPoint = BodyMesh->GetSocketLocation(SocketName);
		EndPoint = StartPoint + BodyMesh->GetSocketRotation(SocketName).Vector() * DamageRange;
	}
	else
	{
		//since, in this game the player can only have weapons that's why using player's location. Otherwise
		//owners location should be used for a more genric implementation.
		StartPoint = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
		EndPoint = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorRotation().Vector() * DamageRange + StartPoint;
	}
	if (GetWorld()->LineTraceSingleByChannel(hitResult, StartPoint, EndPoint, ECollisionChannel::ECC_Pawn, CollisionParams))
	{
		UGameplayStatics::ApplyDamage(hitResult.GetActor(),DamageAmount,GetInstigatorController(),this,DamageClass);
		UE_LOG(LogTemp, Warning, TEXT("(WeaponActor) Weapon has hit: %s"),*(hitResult.GetActor()->GetName()));
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("(WeaponActor) Nothing has been hit."));
	}
	//DrawDebugLine(GetWorld(),StartPoint,EndPoint,FColor::Red,true,3.0f);
}
