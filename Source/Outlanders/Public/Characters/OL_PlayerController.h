// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "OL_PlayerController.generated.h"

/**
 * 
 */
class AOL_Ability;

UCLASS()
class OUTLANDERS_API AOL_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AOL_PlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void SetPlayer(UPlayer* Player);

	/** respawn after dying */
	virtual void UnFreeze() override;

	UPROPERTY(Transient, Replicated)
		TArray<AOL_Ability*> Abilities;

	int32 GetNumAbilities();

	UFUNCTION(BlueprintCallable, Category = Ability)
		AOL_Ability* GetAbility(int32 abilityIndex);

	UFUNCTION(BlueprintCallable, Category = Ability)
		float GetAbilityCooldownMaxValue(int32 abilityIndex);

	UFUNCTION(BlueprintCallable, Category = Ability)
		float GetAbilityManaCostMaxValue(int32 abilityIndex);

	UFUNCTION(BlueprintCallable, Category = Ability)
		int32 GetAbilityLevel(int32 abilityIndex);

	UFUNCTION(BlueprintCallable, Category = Ability)
		float GetAbilityCooldownRemaining(int32 AbilityIndex);

	void InitializeAbilities(TArray<TSubclassOf<AOL_Ability>> AbilityClasses, APawn* Instigator, FVector Location, FRotator Rotation);
	void LevelUpAbility(int32 abilityIndex);
	
protected:

	/** after all game elements are created */
	virtual void PostInitializeComponents() override;
	
};
