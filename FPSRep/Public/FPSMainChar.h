#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
//#include "Weapon.h"
//#include "Engine/DamageEvents.h"

class AWeapon;
#include "FPSMainChar.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCurrentWeaponChangedDelegate, class AWeapon*, CurrentWeapon, const class AWeapon*, OldWeapon);

UCLASS()
class FPSREP_API AFPSMainChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSMainChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USkeletalMeshComponent* ClientMesh;

protected:

	// the weapon classes that are spwaned by default 
	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	TArray<TSubclassOf<class AWeapon>> DefaultWeapons;

	// map that stores the max raycast for each weapon type (sniper, ar, etc...)
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TMap<int32, float> WeaponMaxRaycastDistances;

	// set the default max raycastdistance 
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float DefaultMaxRaycastDistance = 2000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float WeaponRaycastDistance = 2000.0f;

	//UPROPERTY(Replicated, BlueprintReadWrite)
	//bool bIsHeadBoneVisible;


public:
	// LOOK UPPPPPPPPPPPPPPPPPPPPPPP
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated, Category = "State")
	TArray<class AWeapon*> Weapons;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void InflictDamage(AActor* Target, float DamageAmount, float Range);

	// A single weapon reference
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentWeapon, Category = "State")
	class AWeapon* CurrentWeapon;

	// called when the current weapon has been changed 
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FCurrentWeaponChangedDelegate CurrentWeaponChangedDelegate;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State")
	int32 CurrentIndex = 0;

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void EquipWeapon(const int32 Index, AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	AActor* GetTargetActorInFront() const;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character")
	float Health = 100;
	// LOOK UP WHOLE PUBLIC SECTION

protected:
	UFUNCTION()
	virtual void OnRep_CurrentWeapon(const class AWeapon* OldWeapon);

	// server rpc
	UFUNCTION(Server, Reliable)
	void Server_SetCurrentWeapon(class AWeapon* Weapon, AActor* Target);
	virtual void Server_SetCurrentWeapon_Implementation(class AWeapon* NewWeapon, AActor* Target);

	UFUNCTION(Server, Reliable)
	void Server_UpdateHealth(float NewHealth);
	virtual void Server_UpdateHealth_Implementation(float NewHealth);

	//UFUNCTION()
	//void OnRep_IsHeadVisible();
	//
	//UFUNCTION(Server, Reliable, WithValidation)
	//void Server_ToggleHeadBoneVisibility(bool bShouldShowHeadBone);


public:
	UFUNCTION(BlueprintCallable, Category = "Anim")
	virtual void ToADS();

	UFUNCTION(BlueprintCallable, Category = "Anim")
	virtual void BackToHip();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Anim")
	float ADSWeight = 0.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configurations|Anim")
	class UCurveFloat* AimingCurve;

	FTimeline AimingTimeline;

	/// <summary>
	/// Look up
	/// </summary>
	/// <param name="bForward"></param>
	UFUNCTION(Server, Reliable)
	void Server_Aim(const bool bForward = true);
	virtual FORCEINLINE void Server_Aim_Implementation(const bool bForward)
	{
		Multi_Aim(bForward);
		Multi_Aim_Implementation(bForward);
	}

	//LOOK UPPPPPPPPPPPPPP NetMulticast
	UFUNCTION(NetMulticast, Reliable)
	void Multi_Aim(const bool bForward);
	virtual void Multi_Aim_Implementation(const bool bForward);

	UFUNCTION()
	virtual void TimelineProg(const float Val);

protected:
	virtual void NextWeapon();
	virtual void LastWeapon();

	void MoveForward(const float Val);
	void MoveRight(const float Val);
	void LookUp(const float Val);
	void LookRight(const float Val);

	void Fire();
};