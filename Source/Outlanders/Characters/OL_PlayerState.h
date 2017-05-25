// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "OL_PlayerState.generated.h"

/**
 * 
 */

UCLASS()
class OUTLANDERS_API AOL_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
	AOL_PlayerState(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, Replicated)
		float CurrentHealth;	
	UPROPERTY(Transient, Replicated)
		float MaxHealth;

	UPROPERTY(Transient, Replicated)
		float CurrentMana;
	UPROPERTY(Transient, Replicated)
		float MaxMana;

	UPROPERTY(Transient, Replicated)
		float MaxExperience;
	UPROPERTY(Transient, Replicated)
		float CurrentExperience;

	UPROPERTY(Transient, Replicated)
		int32 Level;

	UPROPERTY(Transient, Replicated)
		int32 UpgradePoints;

	UPROPERTY(Transient, Replicated)
		int32 Kills;

	UPROPERTY(Transient, Replicated)
		int32 Deaths;

	UPROPERTY(Transient, Replicated)
		int32 Assists;

	bool bInitialized;

public:
	
	UFUNCTION(BlueprintCallable, Category = Attributes)
		float GetCurrentHealth() const;
	UFUNCTION(BlueprintCallable, Category = Attributes)
		float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = Attributes)
		float GetCurrentMana() const;
	UFUNCTION(BlueprintCallable, Category = Attributes)
		float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = Attributes)
		float GetMaxExperience() const;
	UFUNCTION(BlueprintCallable, Category = Attributes)
		float GetCurrentExperience() const;

	bool GetIsInitialized();

	UFUNCTION(BlueprintCallable, Category = Attributes)
		int32 GetUpgradePoints();

	UFUNCTION(BlueprintCallable, Category = Attributes)
		int32 GetCurrentLevel() const;

	UFUNCTION(BlueprintCallable, Category = Stats)
		int32 GetKills() const;

	UFUNCTION(BlueprintCallable, Category = Stats)
		int32 GetDeaths() const;

	UFUNCTION(BlueprintCallable, Category = Stats)
		int32 GetAssists() const;

	void SetCurrentHealth(float newHealth);
	void SetMaxHealth(float newHealth);
	void SetCurrentMana(float newMana);
	void SetMaxMana(float newMana);
	void SetMaxExperience(float newExperience);
	void SetCurrentExperience(float newExperience);
	void SetLevel(int32 newLevel);

	void AddUpgradePoint();
	void AddUpgradePoints(int32 incr);
	void SetUpgradePoints(int32 newUpgradePoints);

	void AddKill();
	void AddDeath();
	void AddAssist();

	void SetInitialized(bool newInitialized);

};
