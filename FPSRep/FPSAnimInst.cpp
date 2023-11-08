#include "FPSAnimInst.h"
#include "FPSMainChar.h"

UFPSAnimInst::UFPSAnimInst()
{
}

void UFPSAnimInst::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	/*
	MainChar = Cast<AFPSMainChar>(TryGetPawnOwner());
	if (MainChar)
	{
		Mesh = MainChar->GetMesh();
		MainChar->CurrentWeaponChangedDelegate.AddDynamic(this, &UFPSAnimInst::CurrentWeaponChanged);
		CurrentWeaponChanged(MainChar->CurrentWeapon, nullptr);
	}*/

}

void UFPSAnimInst::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!MainChar)
	{
		MainChar = Cast<AFPSMainChar>(TryGetPawnOwner());
		if (MainChar)
		{
			Mesh = MainChar->GetMesh();
			MainChar->CurrentWeaponChangedDelegate.AddDynamic(this, &UFPSAnimInst::CurrentWeaponChanged);
			CurrentWeaponChanged(MainChar->CurrentWeapon, nullptr);
		}
		else
		{
			return;
		}
	}

	SetVariables(DeltaTime);
	CalWeapSway(DeltaTime);

}

void UFPSAnimInst::CurrentWeaponChanged(AWeapon* NewWeap, const AWeapon* OldWeap)
{
	CurrentWeapon = NewWeap;
	if (CurrentWeapon)
	{
		IKProperties = CurrentWeapon->IKProperties;
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UFPSAnimInst::SetIKTransforms);
	}
}

void UFPSAnimInst::SetVariables(const float DeltaTime)
{
	CamTransf = FTransform(MainChar->GetBaseAimRotation(), MainChar->Camera->GetComponentLocation());

	///LOOOOOOOOOOOOOOOK UPPPPPPPPPPPPP
	const FTransform& RootOffset = Mesh->GetSocketTransform(FName("root"), RTS_Component).Inverse() * Mesh->GetSocketTransform(FName("ik_hand_root"));
	RelativeCameraTransform = CamTransf.GetRelativeTransform(RootOffset);

	ADSWeight = MainChar->ADSWeight;

}

// calculates the weapon sway 
void UFPSAnimInst::CalWeapSway(const float DeltaTime)
{

}

void UFPSAnimInst::SetIKTransforms()
{
	RHandToSightsTransform = CurrentWeapon->GetSightWrdTransf().GetRelativeTransform(Mesh->GetSocketTransform(FName("hand_r")));
}