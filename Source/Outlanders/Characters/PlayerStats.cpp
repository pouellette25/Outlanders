// Fill out your copyright notice in the Description page of Project Settings.

#include "Outlanders.h"
#include "PlayerStats.h"

UPlayerStats::UPlayerStats()
{
	
}

void UPlayerStats::Init(AOL_PlayerState* playerState, AOL_GameMode* game)
{
	OL_PlayerState = playerState;

	// If the player state is valid and not initialized
	if (!OL_PlayerState->GetIsInitialized())
	{
		Level = 1;

		Mana.Upgrade(Level);
		Health.Upgrade(Level);
		Experience.Upgrade(Level);
		MP5.Upgrade(Level);
		HP5.Upgrade(Level);

		// Initialize the player state
		InitilizePlayerState(game);		
	}
	else  // Player state is valid and has been initialized
	{
		// Reset the characters level
		SetLevel();

		// Set the current values of health, mana, and experience by the ones that are stored in the player state
		Health.CurrentValue = OL_PlayerState->GetMaxHealth();
		Mana.CurrentValue = OL_PlayerState->GetMaxMana();
		Experience.CurrentValue = OL_PlayerState->GetCurrentExperience();

		// Reset the player states current health and mana values
		SetCurrentHealth(Health.CurrentValue);
		SetCurrentMana(Mana.CurrentValue);		
	}	
}

void UPlayerStats::InitilizePlayerState(AOL_GameMode* game)
{
	SetMaxHealth(GetDefaultHealth());
	SetMaxMana(GetDefaultMana());
	SetMaxExperience(GetDefaultExp());

	OL_PlayerState->SetUpgradePoints(game ? game->InitialUpgradePoints : 1);
	OL_PlayerState->SetInitialized(true);

	SetCurrentHealth(GetMaxHealth());
	SetCurrentMana(GetMaxMana());
	SetCurrentExperience(0);
}

void UPlayerStats::LevelUp()
{
	Level++;

	float prevHealthPercent = Health.MaxValue != 0 ? Health.CurrentValue / Health.MaxValue : 1.0f;
	float prevManaPercent = Mana.MaxValue != 0 ? Mana.CurrentValue / Mana.MaxValue : 1.0f;

	float overExp = Experience.CurrentValue - Experience.MaxValue;

	Mana.Upgrade(Level);
	Health.Upgrade(Level);
	Experience.Upgrade(Level);

	MP5.Upgrade(Level);
	HP5.Upgrade(Level);

	OL_PlayerState->AddUpgradePoint();
	OL_PlayerState->SetLevel(Level);
	SetMaxHealth(Health.MaxValue);
	SetMaxMana(Mana.MaxValue);

	Experience.CurrentValue = overExp > 0 ? overExp : 0;
	SetCurrentExperience(Experience.CurrentValue);
	SetMaxExperience(Experience.MaxValue);

	Health.CurrentValue = FMath::FloorToInt(Health.MaxValue * prevHealthPercent);
	Mana.CurrentValue = FMath::FloorToInt(Mana.MaxValue * prevManaPercent);

	SetCurrentHealth(Health.CurrentValue);
	SetCurrentMana(Mana.CurrentValue);
}

void UPlayerStats::SetLevel()
{
	// Get the current level of the character from the Player state
	int32 level = OL_PlayerState->GetCurrentLevel();

	// Loop till we get to desired level
	for (int32 i = 0; i < level; i++)
	{
		// Increase the level variable
		Level++;
		// Upgrade attributes based off the current level
		Mana.Upgrade(Level);
		Health.Upgrade(Level);
		Experience.Upgrade(Level);
		MP5.Upgrade(Level);
		HP5.Upgrade(Level);
	}
}

void UPlayerStats::Regen()
{
	if (Health.CurrentValue < Health.MaxValue && Health.MaxValue > 0)
	{
		AddHealth(FMath::FloorToFloat(HP5.MaxValue));
	}

	if (Mana.CurrentValue < Mana.MaxValue && Mana.MaxValue > 0)
	{
		AddMana(FMath::FloorToFloat(MP5.MaxValue));
	}
}

void UPlayerStats::AddHealth(float incr)
{
	Health.CurrentValue += incr;
	Health.CurrentValue = FMath::Min(Health.CurrentValue, Health.MaxValue);
	OL_PlayerState->SetCurrentHealth(Health.CurrentValue);
}
void UPlayerStats::AddMana(float incr)
{
	Mana.CurrentValue += incr;
	Mana.CurrentValue = FMath::Min(Mana.CurrentValue, Mana.MaxValue);
	OL_PlayerState->SetCurrentMana(Mana.CurrentValue);
}
void UPlayerStats::AddExperience(float incr)
{
	Experience.CurrentValue += incr;
	OL_PlayerState->SetCurrentExperience(Experience.CurrentValue);
}

void UPlayerStats::AddDeath()
{
	OL_PlayerState->AddDeath();
}
void UPlayerStats::AddKill()
{
	OL_PlayerState->AddKill();
}
void UPlayerStats::AddAssist()
{
	OL_PlayerState->AddAssist();
}

float UPlayerStats::GetCurrentHealth()
{
	return Health.CurrentValue;
}
float UPlayerStats::GetCurrentMana()
{
	return Mana.CurrentValue;
}
float UPlayerStats::GetCurrentExperience()
{
	return Experience.CurrentValue;
}

int32 UPlayerStats::GetUpgradePoints()
{
	return OL_PlayerState->GetUpgradePoints();
}

float UPlayerStats::GetDefaultHealth() const
{
	return Health.BaseValue;
}

float UPlayerStats::GetDefaultMana() const
{
	return Mana.BaseValue;
}

float UPlayerStats::GetDefaultExp() const
{
	return Experience.BaseValue;
}

float UPlayerStats::GetMaxHealth()
{
	return OL_PlayerState->GetMaxHealth();
}
float UPlayerStats::GetMaxMana()
{
	return OL_PlayerState->GetMaxMana();
}
float UPlayerStats::GetMaxExp()
{
	if (!OL_PlayerState) return 0;

	return OL_PlayerState->GetMaxExperience();
}

void UPlayerStats::SetCurrentHealth(float newHealth)
{
	Health.CurrentValue = newHealth;
	OL_PlayerState->SetCurrentHealth(Health.CurrentValue);
}
void UPlayerStats::SetCurrentMana(float newMana)
{
	Mana.CurrentValue = newMana;
	OL_PlayerState->SetCurrentMana(Mana.CurrentValue);
}
void UPlayerStats::SetCurrentExperience(float newExp)
{
	Experience.CurrentValue = newExp;
	OL_PlayerState->SetCurrentExperience(Experience.CurrentValue);
}

void UPlayerStats::SetMaxHealth(float newHealth)
{
	Health.MaxValue = newHealth;
	OL_PlayerState->SetMaxHealth(Health.MaxValue);
}
void UPlayerStats::SetMaxMana(float newMana)
{
	Mana.MaxValue = newMana;
	OL_PlayerState->SetMaxMana(Mana.MaxValue);
}
void UPlayerStats::SetMaxExperience(float newExp)
{
	Experience.MaxValue = newExp;
	OL_PlayerState->SetMaxExperience(Experience.MaxValue);
}

void UPlayerStats::SetUpgradePoints(int32 newUpPoints)
{
	OL_PlayerState->SetUpgradePoints(newUpPoints);
}

//void UPlayerStats::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	//DOREPLIFETIME_CONDITION(UPlayerStats, MagicalPower, COND_OwnerOnly);
//}




