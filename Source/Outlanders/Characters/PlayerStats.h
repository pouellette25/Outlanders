// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "PlayerStats.generated.h"

/**
 * 
 */
UCLASS()
class OUTLANDERS_API UPlayerStats : public UObject
{
	GENERATED_BODY()
	
		UPlayerStats();
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		FUpgradeableParameter MagicalPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		FUpgradeableParameter PhysicalPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		FUpgradeableParameter HP5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		FUpgradeableParameter MP5;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Attributes)
		FUpgradeableParameter Mana;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Attributes)
		FUpgradeableParameter Health;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Attributes)
		FUpgradeableParameter Experience;

	void LevelUp();
	void SetLevel();
	void Regen();
	void Init(AOL_PlayerState* playerState, AOL_GameMode* game);

	void AddHealth(float incr);
	void AddMana(float incr);
	void AddExperience(float incr);

	void AddDeath();
	void AddKill();
	void AddAssist();

	float GetCurrentHealth();
	float GetCurrentMana();
	float GetCurrentExperience();

	float GetDefaultHealth() const;
	float GetDefaultMana() const;
	float GetDefaultExp() const;

	float GetMaxHealth();
	float GetMaxMana();
	float GetMaxExp();

	int32 GetUpgradePoints();

	void SetCurrentHealth(float newHealth);
	void SetCurrentMana(float newMana);
	void SetCurrentExperience(float newExp);

	void SetMaxHealth(float newHealth);
	void SetMaxMana(float newMana);
	void SetMaxExperience(float newExp);
	void SetUpgradePoints(int32 newUpPoints);

	bool bPlayerStateInitialized;

private:
	int32 Level;

	AOL_PlayerState* OL_PlayerState;

	void InitilizePlayerState(AOL_GameMode* game);
};
