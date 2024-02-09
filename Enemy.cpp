// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"


#include "FPSChar.h"
#include "Components/BoxComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(RootComponent);

	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = 1250.0f;
	SightConfig->LoseSightRadius = 1280.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(0.1f);

	AIPerComp->ConfigureSense(*SightConfig);
	AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerComp->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnSensed);

	CurrentVel = FVector::ZeroVector;
	MoveSpeed = 360.0f;

	DistanceSquared = BIG_NUMBER;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnHit);

	BaseLocation = this->GetActorLocation();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentVel.IsZero())
	{
		NewLocation = GetActorLocation() + CurrentVel * DeltaTime;

		if (BackToBaseLoc) 
		{
			if ((NewLocation - BaseLocation).SizeSquared2D() < DistanceSquared)// look up
			{
				DistanceSquared = (NewLocation - BaseLocation).SizeSquared2D();
			}
			else // only triggers if we are at the new location which happens to be the baselocation 
			{
				CurrentVel = FVector::ZeroVector;
				DistanceSquared = BIG_NUMBER;
				BackToBaseLoc = false;

				SetNewRotation(GetActorForwardVector(), GetActorLocation());
			}
		}

		SetActorLocation(NewLocation);
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AFPSChar* Character = Cast<AFPSChar>(OtherActor);

	if (Character)
	{
		Character->DealDamage(DamageVal);
	}

}

void AEnemy::OnSensed(const TArray<AActor*>& UpdatedActors)
{
	//LOOK UP Better way to do the info and dir
	for (int i = 0; i < UpdatedActors.Num(); i++) {
		FActorPerceptionBlueprintInfo Info;
		AIPerComp->GetActorsPerception(UpdatedActors[i], Info);

		if (Info.LastSensedStimuli[0].WasSuccessfullySensed())
		{
			FVector dir = UpdatedActors[i]->GetActorLocation() - GetActorLocation();
			dir.Z = 0.0f;

			CurrentVel = dir.GetSafeNormal() * MoveSpeed;

			SetNewRotation(UpdatedActors[i]->GetActorLocation(), GetActorLocation());
		}
		else {
			FVector dir = BaseLocation - GetActorLocation();
			dir.Z = 0.0f;

			if (dir.SizeSquared2D() > 1.0f) 
			{
				CurrentVel = dir.GetSafeNormal() * MoveSpeed;
				BackToBaseLoc = true; 

				SetNewRotation(BaseLocation, GetActorLocation());
			}
		}

	}

}

void AEnemy::SetNewRotation(FVector TargetPos, FVector CurrentPos)
{

	FVector NewDirection = TargetPos - CurrentPos;
	NewDirection.Z = 0.0f;

	EnemyRotation = NewDirection.Rotation();

	SetActorRotation(EnemyRotation);


}

void AEnemy::DealDamage(float DamageAmount)
{
	Health -= DamageAmount; 

	if (Health <= 0.0f) {
		Destroy();
	}
}

