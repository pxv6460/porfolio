#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
//#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
//#include "Bullet.h"
class ABullet;
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FIKProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimSequence* AnimPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimOffset = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform CustomOffsetTransform;
};

UCLASS(Abstract)
class FPSREP_API AWeapon : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate; // Fire rate in rounds per second.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MagazineSize;

	int32 CurrentAmmo;

public:
	void Fire();
	void StopFire();
	void Reload();
	void EnableShooting();
	FVector GetMuzzleLocation();
	FRotator GetMuzzleRotation();


private:
	// ...

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<ABullet> BulletClass;

public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class USkeletalMeshComponent* Mesh;



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	class AFPSMainChar* CurrentOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configurations")
	FIKProperties IKProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configurations")
	FTransform PlacementTransform;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetDamageAmount() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DamageAmount;

	FTimerHandle FireRateHandle;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IK")
	FTransform GetSightWrdTransf() const;
	// look up below
	virtual FORCEINLINE FTransform GetSightWrdTransf_Implementation() const { return Mesh->GetSocketTransform(FName("Sights")); }

};