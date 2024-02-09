// Fill out your copyright notice in the Description page of Project Settings.


//#include "FPSMainChar.h"
//#include "Camera/CameraComponent.h"
//#include "Components/SkinnedMeshComponent.h"
//#include <Net/UnrealNetwork.h>
//
//// Sets default values
//AFPSMainChar::AFPSMainChar()
//{
//	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//	GetMesh()->SetTickGroup(ETickingGroup::TG_PostUpdateWork);
//	GetMesh()->bCastHiddenShadow = true;
//	GetMesh()->bVisibleInReflectionCaptures = true;
//
//	ClientMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClientMesh"));
//	ClientMesh->SetCastShadow(false);
//	ClientMesh->bCastHiddenShadow = false;
//	ClientMesh->bVisibleInReflectionCaptures = false;
//	ClientMesh->SetTickGroup(ETickingGroup::TG_PostUpdateWork);
//	ClientMesh->SetupAttachment(GetMesh());
//
//	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
//	Camera->bUsePawnControlRotation = true;
//	Camera->SetupAttachment(GetMesh(), FName("head"));
//
//	// AR WeaponmaxRayCastDistance
//	WeaponMaxRaycastDistances.Add(0, 700.0f);
//	// Shotgun WeaponmaxRayCastDistance
//	WeaponMaxRaycastDistances.Add(1, 25.0f);
//	// Pistol WeaponmaxRayCastDistance
//	WeaponMaxRaycastDistances.Add(2, 100.0f);
//	// Sniper WeaponmaxRayCastDistance
//	WeaponMaxRaycastDistances.Add(3, 800.0f);
//
//	//bIsHeadBoneVisible = false;
//}
//
//// Called when the game starts or when spawned
//void AFPSMainChar::BeginPlay()
//{
//	Super::BeginPlay();
//
//	//ADS timeline setup
//	if (AimingCurve)
//	{
//		FOnTimelineFloat TimelineFloat;
//		TimelineFloat.BindDynamic(this, &AFPSMainChar::TimelineProg);
//
//		AimingTimeline.AddInterpFloat(AimingCurve, TimelineFloat);
//	}
//
//	// Logic for the clientmesh
//	if (IsLocallyControlled())
//	{
//		ClientMesh->HideBoneByName(FName("neck_01"), EPhysBodyOp::PBO_None);
//		GetMesh()->SetVisibility(false);
//	}
//	else
//	{
//		ClientMesh->DestroyComponent();
//	}
//
//	//LOOKUP
//	//Spawning weapons 
//	if (HasAuthority())
//	{
//		for (const TSubclassOf<AWeapon>& WeaponClass : DefaultWeapons)
//		{
//			if (!WeaponClass) continue;
//			FActorSpawnParameters Params;
//			Params.Owner = this;
//			//spawn
//			AWeapon* SpawnWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, Params);
//			const int32 Index = Weapons.Add(SpawnWeapon);
//			if (Index == CurrentIndex)
//			{
//				CurrentWeapon = SpawnWeapon;
//				OnRep_CurrentWeapon(nullptr);
//			}
//		}
//	}
//}
//
//void AFPSMainChar::Tick(const float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	// allows the track to run 
//	AimingTimeline.TickTimeline(DeltaTime);
//}
//
//
//void AFPSMainChar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	//LOOK UP
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(AFPSMainChar, ADSWeight);
//
//	// Define the replicated property for the Weapons array
//	DOREPLIFETIME_CONDITION(AFPSMainChar, Weapons, COND_None);
//	DOREPLIFETIME_CONDITION(AFPSMainChar, CurrentWeapon, COND_None);
//}
//
//void AFPSMainChar::InflictDamage(AActor* Target, float DamageAmount, float Range)
//{
//	if (Target && DamageAmount > 0)
//	{
//		// Apply damage to the target
//		Target->TakeDamage(DamageAmount, FDamageEvent(), GetController(), this);
//	}
//}
//
//void AFPSMainChar::EquipWeapon(const int32 Index, AActor* Target)
//{
//	if (!Weapons.IsValidIndex(Index) || CurrentWeapon == Weapons[Index]) return;
//
//	if (IsLocallyControlled() || HasAuthority())
//	{
//		CurrentIndex = Index;
//
//		const AWeapon* OldWeapon = CurrentWeapon;
//		CurrentWeapon = Weapons[Index];
//		OnRep_CurrentWeapon(OldWeapon);
//
//		// Inflict damage using the current weapon's damage value
//		if (CurrentWeapon)
//		{
//			int32 WeaponIndex = Weapons.Find(CurrentWeapon);
//			WeaponRaycastDistance = WeaponMaxRaycastDistances.Contains(WeaponIndex) ? WeaponMaxRaycastDistances[WeaponIndex] : DefaultMaxRaycastDistance;
//
//			InflictDamage(Target, CurrentWeapon->GetDamageAmount(), WeaponRaycastDistance);
//		}
//	}
//
//	if (!HasAuthority())
//	{
//		Server_SetCurrentWeapon(Weapons[Index], Target);
//	}
//}
//
//AActor* AFPSMainChar::GetTargetActorInFront() const
//{
//	FVector Start = Camera->GetComponentLocation();
//	FVector ForwardVector = Camera->GetForwardVector();
//	FVector End = Start + (ForwardVector * WeaponRaycastDistance); // Define MaxRaycastDistance as needed
//
//	FHitResult HitResult;
//	FCollisionQueryParams CollisionParams;
//	CollisionParams.AddIgnoredActor(this); // Ignore the character itself
//
//	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
//	{
//		return HitResult.GetActor();
//	}
//
//	return nullptr;
//}
//
//void AFPSMainChar::Server_SetCurrentWeapon_Implementation(AWeapon* NewWeapon, AActor* Target)
//{
//	const AWeapon* OldWeapon = CurrentWeapon;
//	CurrentWeapon = NewWeapon;
//	OnRep_CurrentWeapon(OldWeapon);
//
//	if (CurrentWeapon)
//	{
//		int32 WeaponIndex = Weapons.Find(CurrentWeapon);
//		WeaponRaycastDistance = WeaponMaxRaycastDistances.Contains(WeaponIndex) ? WeaponMaxRaycastDistances[WeaponIndex] : DefaultMaxRaycastDistance;
//
//		InflictDamage(Target, CurrentWeapon->GetDamageAmount(), WeaponRaycastDistance);
//	}
//}
//
//void AFPSMainChar::Multi_Aim_Implementation(const bool bForward)
//{
//	if (bForward == true)
//	{
//		AimingTimeline.Play();
//	}
//	else
//	{
//		AimingTimeline.Reverse();
//	}
//}
//
//// aim stuff
//void AFPSMainChar::TimelineProg(const float Val)
//{
//	ADSWeight = Val;
//}
//
//void AFPSMainChar::OnRep_CurrentWeapon(const AWeapon* OldWeapon)
//{
//	if (CurrentWeapon) {
//		if (!CurrentWeapon->CurrentOwner)
//		{
//			// look up 
//			const FTransform& PlacementTransform = CurrentWeapon->PlacementTransform * GetMesh()->GetSocketTransform(FName("weaponsocket_r"));
//			CurrentWeapon->SetActorTransform(PlacementTransform, false, nullptr, ETeleportType::TeleportPhysics);
//			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("weaponsocket_r"));
//
//			CurrentWeapon->CurrentOwner = this;
//		}
//		CurrentWeapon->Mesh->SetVisibility(true);
//	}
//
//	if (OldWeapon)
//	{
//		OldWeapon->Mesh->SetVisibility(false);
//
//	}
//
//	//if (GetMesh()->GetBoneIndex(FName("head")) != INDEX_NONE)
//	//{
//	//	// Hide the head bone
//	//	GetMesh()->HideBoneByName(FName("head"), EPhysBodyOp::PBO_None);
//	//}
//
//	CurrentWeaponChangedDelegate.Broadcast(CurrentWeapon, OldWeapon);
//}
//
//// Called to bind functionality to input
//void AFPSMainChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//	PlayerInputComponent->BindAction(FName("ADS"), EInputEvent::IE_Pressed, this, &AFPSMainChar::ToADS);
//	PlayerInputComponent->BindAction(FName("ADS"), EInputEvent::IE_Released, this, &AFPSMainChar::BackToHip);
//
//	PlayerInputComponent->BindAction(FName("NextWeapon"), EInputEvent::IE_Pressed, this, &AFPSMainChar::NextWeapon);
//	PlayerInputComponent->BindAction(FName("LastWeapon"), EInputEvent::IE_Pressed, this, &AFPSMainChar::LastWeapon);
//
//	PlayerInputComponent->BindAxis(FName("Move Forward / Backward"), this, &AFPSMainChar::MoveForward);
//	PlayerInputComponent->BindAxis(FName("Move Right / Left"), this, &AFPSMainChar::MoveRight);
//	PlayerInputComponent->BindAxis(FName("Look Up / Down Mouse"), this, &AFPSMainChar::LookUp);
//	PlayerInputComponent->BindAxis(FName("Turn Right / Left Mouse"), this, &AFPSMainChar::LookRight);
//
//}
//
///// <summary>
///// ACTIONS
///// </summary>
//
//// stuf to ads
//void AFPSMainChar::ToADS()
//{
//	if (IsLocallyControlled() || HasAuthority())
//	{
//		//sets bForward to true
//		Multi_Aim_Implementation(true);
//	}
//
//	if (!HasAuthority())
//	{
//		//sets bForward to true
//		Server_Aim(true);
//	}
//}
//
//void AFPSMainChar::BackToHip()
//{
//	if (IsLocallyControlled() || HasAuthority())
//	{
//		//sets bForward to false
//		Multi_Aim_Implementation(false);
//	}
//
//	if (!HasAuthority())
//	{
//		//sets bForward to false
//		Server_Aim(false);
//	}
//}
//
////weapon swaping
//void AFPSMainChar::NextWeapon()
//{
//	if (Weapons.Num() == 0)
//	{
//		return;
//	}
//
//	const int32 Index = Weapons.IsValidIndex(CurrentIndex + 1) ? CurrentIndex + 1 : 0; //LOOKKKKKKKKKKKK UPPPPPPPPPPPPPPPPP
//	EquipWeapon(Index, GetTargetActorInFront());
//}
//
//void AFPSMainChar::LastWeapon()
//{
//
//	if (Weapons.Num() == 0)
//	{
//		return;
//	}
//
//	const int32 Index = Weapons.IsValidIndex(CurrentIndex - 1) ? CurrentIndex - 1 : Weapons.Num() - 1; //LOOKKKKKKKKKKKK UPPPPPPPPPPPPPPPPP
//	EquipWeapon(Index, GetTargetActorInFront());
//}
//
//
//// movement
//void AFPSMainChar::MoveForward(const float Val)
//{
//	const FVector& Direction = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X);
//	AddMovementInput(Direction, Val);
//}
//
//void AFPSMainChar::MoveRight(const float Val)
//{
//	const FVector& Direction = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y);
//	AddMovementInput(Direction, Val);
//}
//
//void AFPSMainChar::LookUp(const float Val)
//{
//	AddControllerPitchInput(Val);
//}
//
//void AFPSMainChar::LookRight(const float Val)
//{
//	AddControllerYawInput(Val);
//}

