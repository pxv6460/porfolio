// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "weapon.h"
#include "FPSMainChar.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create our sphere collider and make it the root component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(8.f);
	
	////Create a mesh to render the bullet
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Graphic"));
	BulletMesh->SetupAttachment(RootComponent);
	
	////Create a projectile movement component
	//ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	//ProjectileMovement->InitialSpeed = 5000.0f;
	//ProjectileMovement->MaxSpeed = 5000.0f;
	//ProjectileMovement->Velocity = FVector(5000.0f, 0.f, 0.f);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveBullet();
}

void ABullet::MoveBullet()
{
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * (BulletSpeed * GetWorld()->GetDeltaSeconds());
	SetActorLocation(NewLocation);
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Check if the bullet hit a character
	if (OtherActor == TargetActor)
	{

		if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());


			if (TargetActor->IsA<AFPSMainChar>())
			{
				AFPSMainChar* Charact = Cast<AFPSMainChar>(TargetActor);

				switch (Charact->CurrentIndex)
				{

					case 0:

						DamageAmount = 20.0f;
						break;

					case 1:

						DamageAmount = 10.0f;
						break;

					case 2:

						DamageAmount = 100.0f;
						break;

					case 3:

						DamageAmount = 150.0f;
						break;

					default:

						UE_LOG(LogTemp, Warning, TEXT("Weapon index is invalid"));
						break;
				}
				Charact->Health -= DamageAmount;

			}
			Destroy();
		}
	}
	// Destroy the bullet
	//Destroy();
}

//Check if the bullet hit a character
//if (OtherActor == TargetActor)
//{
//	//ABullet* Bullet = new ABullet();
//	// //OtherActor->ApplyDamage(DamageAmount, this, nullptr, nullptr, UDamageType::StaticClass());
//	//Deal damage to the character
//
//	if (OtherActor->IsA<ACharacter>())
//	{
//
//		UDamageType* DamageTypeClass = UDamageType::StaticClass();
//		FDamageEvent DamageEvent(DamageTypeClass, Hit, NormalImpulse, HitComponent);
//		OtherActor->TakeDamage(DamageAmount, DamageEvent, nullptr, this);
//	}
//}
//
//if (OtherActor)
//{
//	OtherActor->Destroy();
//}