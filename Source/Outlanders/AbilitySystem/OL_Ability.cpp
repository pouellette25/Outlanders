// Fill out your copyright notice in the Description page of Project Settings.

#include "Outlanders.h"
#include "OL_Ability.h"
#include "../Characters/OL_Character.h"

AOL_Ability::AOL_Ability(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	AimDecal = ObjectInitializer.CreateDefaultSubobject<UDecalComponent>(this, TEXT("AimingDecal"));
	CollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("RootComponent"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetVisibility(true, true);

	RootComponent = CollisionComponent;
	AimDecal->AttachTo(RootComponent);
	AimDecal->SetVisibility(false);

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AOL_Ability::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AOL_Ability::ToggleHidden()
{
	AimDecal->SetHiddenInGame(!AimDecal->bHiddenInGame);
}

void AOL_Ability::ClientToggleHidden_Implementation()
{
	ToggleHidden();
}

void AOL_Ability::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Role == ROLE_Authority && IsOnCooldown())
	{
		CooldownRemaining = GetWorldTimerManager().GetTimerRemaining(TimerHandle_CoolDown);
	}

	if (Instigator)
	{
		switch (AbilityAimType)
		{
			case EAbilityAimTypes::AAT_FREE:
				HandleFreeAiming();
				break;
			case EAbilityAimTypes::AAT_FIXED:
				break;
			case EAbilityAimTypes::AAT_MOVEMENT_BASED:
				break;
			case EAbilityAimTypes::AAT_ACTORTARGET:
				break;
			case EAbilityAimTypes::AAT_FIXEDCONE:
				break;
			case EAbilityAimTypes::AAT_FIXEDSQUARE:
				break;
			case EAbilityAimTypes::AAT_INSTANT:
				break;
			case EAbilityAimTypes::AAT_NONE:
				break;
		}
	}
}

void AOL_Ability::HandleFreeAiming()
{
	FRotator OwnerRotation = Instigator->GetControlRotation();

	FVector ForwardVect = OwnerRotation.Vector();
	FVector OwnerLocation = Instigator->GetActorLocation();
	FVector MaxRangeVect = OwnerLocation + (ForwardVect * Range.MaxValue);

	FHitResult HitOut(ForceInit);

	FCollisionQueryParams TraceParams(FName(TEXT("AbilityTrace")), true, Instigator);

	//GetWorld()->DebugDrawTraceTag = TraceParams.TraceTag;

	bool hitActor = GetWorld()->LineTraceSingleByChannel(HitOut, OwnerLocation, MaxRangeVect, ECollisionChannel::ECC_WorldStatic, TraceParams);

	if (!hitActor)
	{
		FVector DownVect = MaxRangeVect;
		DownVect.Z -= 1000;

		hitActor = GetWorld()->LineTraceSingleByChannel(HitOut, MaxRangeVect, DownVect, ECollisionChannel::ECC_WorldStatic, TraceParams);
	}
	if (hitActor)
	{
		SetActorRotation(FRotator(-90.0f, OwnerRotation.Yaw, OwnerRotation.Roll));
		SetActorLocation(HitOut.Location);
	}

}

void AOL_Ability::CastAbility_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_CastTime);
	StartCooldown();
	AOL_Character* Owner = Cast<AOL_Character>(Instigator);

	if (Owner)
	{
		Owner->ConsumeResources(this);
	}
}

void AOL_Ability::Activate_Implementation()
{
	if (CastTime.MaxValue <= 0)
	{
		// TODO: Add aiming decal logic for showing cast time

		CastAbility();
	}
	else
	{
		// TODO: Add aiming decal logic for showing cast time

		GetWorldTimerManager().SetTimer(TimerHandle_CastTime, this, &AOL_Ability::CastAbility, CastTime.MaxValue, false);
	}
}

void AOL_Ability::StartCooldown()
{
	// TODO: Add support for cooldown reduction and free abilities

	GetWorldTimerManager().SetTimer(TimerHandle_CoolDown, this, &AOL_Ability::OnCooldownComplete, CooldownTime.MaxValue, false);
}

bool AOL_Ability::CanBeCast()
{
	return AbilityLevel > 0 && !IsBeingCast() && !IsOnCooldown();
}

bool AOL_Ability::IsBeingCast()
{
	return GetWorldTimerManager().IsTimerActive(TimerHandle_CastTime);
}

bool AOL_Ability::IsOnCooldown()
{
	return GetWorldTimerManager().IsTimerActive(TimerHandle_CoolDown);
}

float AOL_Ability::GetCooldownTimeRemaining()
{
	return CooldownRemaining;
}

float AOL_Ability::GetCooldownMaxValue()
{
	return CooldownTime.MaxValue;
}

float AOL_Ability::GetManaCostMaxValue()
{
	return ManaCost.MaxValue;
}

void AOL_Ability::OnCooldownComplete()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_CoolDown);
}

void AOL_Ability::SetLevel(int32 level)
{
	for (int32 i = AbilityLevel; i < level; i++)
	{
		LevelUp();
	}
}

void AOL_Ability::LevelUp()
{
	AbilityLevel++;
	BaseDamage.Upgrade(AbilityLevel);
	CooldownTime.Upgrade(AbilityLevel);
	ManaCost.Upgrade(AbilityLevel);
	Range.Upgrade(AbilityLevel);
	CastTime.Upgrade(AbilityLevel);

	// Check to see if we have added any Upgradeable parameters to the array
	int32 BpUpCount = BpUpgradeableParameters.Num();
	if (BpUpCount > 0)
	{
		// If we have, upgrade them
		for (int32 i = 0; i < BpUpCount; i++)
		{
			BpUpgradeableParameters[i].Upgrade(AbilityLevel);
		}
	}
}

void AOL_Ability::ClientLevelUp_Implementation()
{
	LevelUp();
}

void AOL_Ability::DestroyDecal()
{
	RootComponent->DestroyComponent();
}

void AOL_Ability::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AOL_Ability, BaseDamage, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AOL_Ability, AbilityLevel, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AOL_Ability, CooldownTime, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AOL_Ability, ManaCost, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AOL_Ability, Range, COND_OwnerOnly); 
	DOREPLIFETIME_CONDITION(AOL_Ability, CastTime, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AOL_Ability, BpUpgradeableParameters, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AOL_Ability, CooldownRemaining, COND_OwnerOnly);

}

