#include "Bullet.h"
#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	DamageAmount = 0.0f;


	FireRate = 2.0f;   // Example fire rate.
	MagazineSize = 30;  // Example magazine size.
	CurrentAmmo = MagazineSize; // Initialize the current ammo.
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (!CurrentOwner)
		Mesh->SetVisibility(false);
}

float AWeapon::GetDamageAmount() const
{
	return DamageAmount;
}

void AWeapon::Fire()
{
	// Check if the current ammo is greater than zero.
	if (CurrentAmmo > 0)
	{
		// Fire logic: Instantiate bullets and apply force along the raycast.
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, GetMuzzleLocation(), GetMuzzleRotation());

		// Reduce current ammo.
		CurrentAmmo--;

		// Set up a timer to control the fire rate.
		GetWorldTimerManager().SetTimer(FireRateHandle, this, &AWeapon::EnableShooting, 1.0f / FireRate, false);
	}
}

void AWeapon::StopFire()
{
	// dont use 
}

void AWeapon::EnableShooting()
{
	// Reset the fire rate timer.
	GetWorldTimerManager().ClearTimer(FireRateHandle);

	// Allow shooting after the fire rate timer expires.
	// Implement any additional logic you need for re-enabling shooting.
}

FVector AWeapon::GetMuzzleLocation()
{
	FVector MuzzleLoc;
	// Implement this method to return the location of the weapon's muzzle.
	MuzzleLoc =  Mesh->GetSocketLocation("Muzzle");

	return MuzzleLoc;
}

 FRotator AWeapon::GetMuzzleRotation()
{
	 FRotator MuzzleRot;
	 // Implement this method to return the location of the weapon's muzzle.
	 MuzzleRot = Mesh->GetSocketRotation("Muzzle");

	 return MuzzleRot;
}

void AWeapon::Reload()
{
	// Implement logic to reload the weapon, refilling the magazine.
	CurrentAmmo = MagazineSize;
}


/*FTransform AWeapon::GetSightWrdTransf() const
{
	return FTransform();
}*/

