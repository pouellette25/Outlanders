// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../OutlandersTypes.h"
#include "GameFramework/Actor.h"
#include "OL_Ability.generated.h"

/**
 * Class to handle base functionality of abilities
 */
UCLASS()
class OUTLANDERS_API AOL_Ability : public AActor
{
	GENERATED_BODY()
	
public:
	AOL_Ability(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaSeconds) override;

	void ToggleHidden();

	UFUNCTION(client, reliable)
		void ClientToggleHidden();
		void ClientToggleHidden_Implementation();

	UPROPERTY(Transient, Replicated)
		float CooldownRemaining;
	FTimerHandle TimerHandle_CoolDown;
	FTimerHandle TimerHandle_CastTime;
	

	/* Overriden in Blueprint. Called after activation, and cast time is complete. Don't forget call to parent function in blueprint */
	UFUNCTION(BlueprintNativeEvent, Category = Ability)
		void CastAbility();
		void CastAbility_Implementation();

	/* Overriden in Blueprint. Activates the ability EG. Handles spawn points and any other initializations needed. Don't forget call to parent function in blueprint */
	UFUNCTION(BlueprintNativeEvent, Category = Ability)
		void Activate();
		void Activate_Implementation();

	UPROPERTY(EditDefaultsOnly, Category = Root)
		USphereComponent* CollisionComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = UpgradeableParameters)
		FUpgradeableParameter ManaCost;

	void DestroyDecal();
	bool CanBeCast();

	virtual void LevelUp();

	UPROPERTY(EditDefaultsOnly, Replicated, Category = Ability)
		int32 AbilityLevel;

	void SetLevel(int32 level);

	float GetCooldownTimeRemaining();

	float GetCooldownMaxValue();
	float GetManaCostMaxValue();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Ability)
		TEnumAsByte<EAbilityAimTypes::Type> AbilityAimType;	

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = UpgradeableParameters)
		FUpgradeableParameter BaseDamage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = UpgradeableParameters)
		FUpgradeableParameter CooldownTime;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = UpgradeableParameters)
		FUpgradeableParameter Range;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = UpgradeableParameters)
		FUpgradeableParameter CastTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = UpgradeableParameters, meta = (DisplayName = "UpgradeableParameters"))
		TArray<FUpgradeableParameter> BpUpgradeableParameters;
	
	UFUNCTION(reliable, client)
		void ClientLevelUp();
		void ClientLevelUp_Implementation();

	void StartCooldown();
	void OnCooldownComplete();

	UPROPERTY(EditDefaultsOnly, Category = Decal)
		UDecalComponent* AimDecal;

private:
	/**
	* Handles Free Aiming mode of the aiming decal
	*/
	void HandleFreeAiming();

	bool IsBeingCast();
	bool IsOnCooldown();
	
};
