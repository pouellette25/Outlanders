// Copyright LP Creative Ventures LLC.

#pragma once

#include "Components/ActorComponent.h"
#include "OL_AbilityManager.generated.h"

class AOL_Ability;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OUTLANDERS_API UOL_AbilityManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(Transient, Replicated)
		TArray<AOL_Ability*> Abilities;

	// Sets default values for this component's properties
	UOL_AbilityManager();

	/**
	* Activates the requested ability or casts it if the player was already in aiming mode.
	*
	* @param AbilityIndex The index of the ability to use.
	*/
	UFUNCTION(BlueprintCallable, Category = Ability)
		void BlueprintActivateAbility(int32 AbilityIndex);

	/**
	* Activate ability
	* Activates the requested ability or casts it if the player was already in aiming mode
	*
	* @param abilityIndex Index of the ability to use
	* @param CalledFromClient Whether or not this call came from a client
	*/
	void ActivateAbility(int32 AbilityIndex, bool CalledFromClient = false);

	/**
	* Deactivate ability
	* DeActivates the requested ability
	*
	* @param AbilityIndex Index of the ability to deactivate
	* @param CalledFromClient Whether or not this call came from a client
	*/
	void DeActivateAbility(int32 AbilityIndex, bool CalledFromClient = false);

	UFUNCTION(BlueprintCallable, Category = Ability)
		AOL_Ability* GetAbility(int32 abilityIndex);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Ability)
		TArray<TSubclassOf<AOL_Ability>> AbilityClasses;

	UFUNCTION(server, reliable, WithValidation)
		void ServerActivateAbility(int32 aIndex, bool calledFromClient = false);
		bool ServerActivateAbility_Validate(int32 aIndex, bool calledFromClient = false);
		void ServerActivateAbility_Implementation(int32 aIndex, bool calledFromClient = false);

	UFUNCTION(server, reliable, WithValidation)
		void ServerDeActivateAbility(int32 aIndex, bool CalledFromClient = false);
		bool ServerDeActivateAbility_Validate(int32 aIndex, bool CalledFromClient = false);
		void ServerDeActivateAbility_Implementation(int32 aIndex, bool CalledFromClient = false);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
