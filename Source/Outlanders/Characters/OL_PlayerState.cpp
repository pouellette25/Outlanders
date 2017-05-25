// Fill out your copyright notice in the Description page of Project Settings.

#include "Outlanders.h"
#include "OL_PlayerState.h"

AOL_PlayerState::AOL_PlayerState(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	CurrentExperience = 0;
	Level = 1;
	NetUpdateFrequency = 5;
}

float AOL_PlayerState::GetCurrentHealth() const
{
	return CurrentHealth;
}

float AOL_PlayerState::GetMaxHealth() const
{
	return MaxHealth;
}

float AOL_PlayerState::GetCurrentMana() const
{
	return CurrentMana;
}

float AOL_PlayerState::GetMaxMana() const
{
	return MaxMana;
}

float AOL_PlayerState::GetMaxExperience() const
{
	return MaxExperience;
}

float AOL_PlayerState::GetCurrentExperience() const
{
	return CurrentExperience;
}

int32 AOL_PlayerState::GetCurrentLevel() const
{
	return Level;
}

int32 AOL_PlayerState::GetUpgradePoints()
{
	return UpgradePoints;
}

bool AOL_PlayerState::GetIsInitialized()
{
	return bInitialized;
}

int32 AOL_PlayerState::GetKills() const
{
	return Kills;
}

int32 AOL_PlayerState::GetDeaths() const
{
	return Deaths;
}

int32 AOL_PlayerState::GetAssists() const
{
	return Assists;
}

void AOL_PlayerState::SetLevel(int32 newLevel)
{
	Level = newLevel > 0 ? newLevel : 0;
}

void AOL_PlayerState::AddUpgradePoint()
{
	UpgradePoints++;
}

void AOL_PlayerState::AddUpgradePoints(int32 incr)
{
	UpgradePoints += incr;
}

void AOL_PlayerState::SetUpgradePoints(int32 newUpgradePoints)
{
	UpgradePoints = newUpgradePoints;
}

void AOL_PlayerState::AddKill()
{
	Kills++;
}

void AOL_PlayerState::AddDeath()
{
	Deaths++;
}

void AOL_PlayerState::AddAssist()
{
	Assists++;
}

void AOL_PlayerState::SetCurrentHealth(float newHealth)
{
	CurrentHealth = newHealth > 0 ? newHealth : 0;
}

void AOL_PlayerState::SetMaxHealth(float newHealth)
{
	MaxHealth = newHealth > 0 ? newHealth : 0;
}

void AOL_PlayerState::SetCurrentMana(float newMana)
{
	CurrentMana = newMana > 0 ? newMana : 0;
}

void AOL_PlayerState::SetMaxMana(float newMana)
{
	MaxMana = newMana > 0 ? newMana : 0;
}

void AOL_PlayerState::SetMaxExperience(float newExperience)
{
	MaxExperience = newExperience > 0 ? newExperience : 0;
}

void AOL_PlayerState::SetCurrentExperience(float newExperience)
{
	CurrentExperience = newExperience > 0 ? newExperience : 0;
}

void AOL_PlayerState::SetInitialized(bool newInitialized)
{
	bInitialized = newInitialized;
}

void AOL_PlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOL_PlayerState, Level);
	DOREPLIFETIME(AOL_PlayerState, CurrentHealth);
	DOREPLIFETIME(AOL_PlayerState, CurrentMana);
	DOREPLIFETIME(AOL_PlayerState, Kills);
	DOREPLIFETIME(AOL_PlayerState, Deaths);
	DOREPLIFETIME(AOL_PlayerState, Assists);
	
	DOREPLIFETIME_CONDITION(AOL_PlayerState, CurrentExperience, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AOL_PlayerState, MaxExperience, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AOL_PlayerState, MaxHealth, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AOL_PlayerState, MaxMana, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AOL_PlayerState, UpgradePoints, COND_OwnerOnly);
}


