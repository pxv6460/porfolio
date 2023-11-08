#pragma once

#include "CoreMinimal.h"
//#include "Kismet/KismetMathLibrary.h"
//#include "Math/Rotator.h"
//#include "Components/TimelineComponent.h"
#include "Animation/AnimInstance.h"
#include "Weapon.h"
#include "FPSAnimInst.generated.h"

UCLASS()
class FPSREP_API UFPSAnimInst : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Sets default values for this empty's properties
	UFPSAnimInst();

protected:
	///
	// Called when the game starts or when spawned
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UFUNCTION()
	virtual void CurrentWeaponChanged(class AWeapon* NewWeap, const class AWeapon* OldWeap);

	virtual void SetVariables(const float DeltaTime);
	virtual void CalWeapSway(const float DeltaTime);

	virtual void SetIKTransforms();

public:
	// the references 
	UPROPERTY(BlueprintReadWrite, Category = "Anim")
	class AFPSMainChar* MainChar;

	UPROPERTY(BlueprintReadWrite, Category = "Anim")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	class AWeapon* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	FIKProperties IKProperties;

	// IK variables 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	FTransform CamTransf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	FTransform RelativeCameraTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	FTransform RHandToSightsTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	float ADSWeight = 0.0f;
};