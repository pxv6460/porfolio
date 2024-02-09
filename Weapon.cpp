//#include "Weapon.h"
//
//// Sets default values
//AWeapon::AWeapon()
//{
//	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = false;
//
//	SetReplicates(true);
//
//	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
//	RootComponent = Root;
//
//	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
//	Mesh->SetupAttachment(Root);
//
//	DamageAmount = 0.0f;
//
//}
//
//// Called when the game starts or when spawned
//void AWeapon::BeginPlay()
//{
//	Super::BeginPlay();
//	if (!CurrentOwner)
//		Mesh->SetVisibility(false);
//}
//
//float AWeapon::GetDamageAmount() const
//{
//	return DamageAmount;
//}

