// Fill out your copyright notice in the Description page of Project Settings.

#include "Outlanders.h"
#include "AbilitySystem/OL_Ability.h"
#include "OL_PlayerController.h"


AOL_PlayerController::AOL_PlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TRACE("PC Created");
}

void AOL_PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AOL_PlayerController::SetPlayer(UPlayer* PlayerIn)
{
	Super::SetPlayer(PlayerIn);
}

void AOL_PlayerController::UnFreeze()
{
	FTimerHandle TimerHandle_Respawn;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Respawn, this, &AOL_PlayerController::ServerRestartPlayer, 5.f, false);
}

void AOL_PlayerController::InitializeAbilities(TArray<TSubclassOf<AOL_Ability>> AbilityClasses, APawn* InstigatorIn, FVector Location, FRotator Rotation)
{
	UWorld* world = GetWorld();
	int32 numAbilities = AbilityClasses.Num();

	for (int32 i = 0; i < numAbilities; i++)
	{
		FActorSpawnParameters SpawnInfo;

		//SpawnInfo.bNoCollisionFail = true;
		SpawnInfo.Instigator = InstigatorIn;
		SpawnInfo.Owner = this;

		AOL_Ability* NewAbility = world->SpawnActor<AOL_Ability>(AbilityClasses[i], Location, Rotation, SpawnInfo);

		if (NewAbility)
		{
			Abilities.Add(NewAbility);
		}
	}
}

// TODO: Remove these ability functions

int32 AOL_PlayerController::GetNumAbilities()
{
	return Abilities.Num();
}

float AOL_PlayerController::GetAbilityCooldownMaxValue(int32 abilityIndex)
{
	AOL_Ability* ability = GetAbility(abilityIndex);
	return ability ? ability->GetCooldownMaxValue() : 0;
}

float AOL_PlayerController::GetAbilityManaCostMaxValue(int32 abilityIndex)
{
	AOL_Ability* ability = GetAbility(abilityIndex);
	return ability ? ability->GetManaCostMaxValue() : 0;
}

float AOL_PlayerController::GetAbilityCooldownRemaining(int32 abilityIndex)
{
	return Abilities.Num() > abilityIndex ? Abilities[abilityIndex]->GetCooldownTimeRemaining() : 0;
}

int32 AOL_PlayerController::GetAbilityLevel(int32 abilityIndex)
{
	int32 level = Abilities.Num() > abilityIndex ? Abilities[abilityIndex]->AbilityLevel : 0;
	return level;
}

AOL_Ability* AOL_PlayerController::GetAbility(int32 abilityIndex)
{
	return Abilities.Num() > abilityIndex ? Abilities[abilityIndex] : NULL;
}

void AOL_PlayerController::LevelUpAbility(int32 abilityIndex)
{
	if (Abilities.Num() > abilityIndex)
	{
		Abilities[abilityIndex]->LevelUp();
	}
}

// end TODO: remove ability functions

void AOL_PlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AOL_PlayerController, Abilities, COND_OwnerOnly);
}