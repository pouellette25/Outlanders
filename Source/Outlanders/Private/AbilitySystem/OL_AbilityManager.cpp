// Copyright LP Creative Ventures LLC.

#include "Outlanders.h"
#include "OL_AbilityManager.h"
#include "AbilitySystem/OL_Ability.h"

AOL_Ability* UOL_AbilityManager::GetAbility(int32 abilityIndex)
{
	return Abilities.Num() > abilityIndex ? Abilities[abilityIndex] : NULL;
}

// Sets default values for this component's properties
UOL_AbilityManager::UOL_AbilityManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOL_AbilityManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UOL_AbilityManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UOL_AbilityManager::BlueprintActivateAbility(int32 AbilityIndex)
{
	ActivateAbility(AbilityIndex);
}

void UOL_AbilityManager::ActivateAbility(int32 AbilityIndex, bool CalledFromClient)
{	
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerActivateAbility(AbilityIndex, true);
		return;
	}

	AOL_Ability* ability = OL_PC->GetAbility(AbilityIndex);

	/*if (ability && ability->CanBeCast() && PlayerStats->GetCurrentMana() >= ability->ManaCost.MaxValue)
	{
	if (bIsCasting)
	{
	ability->Activate();
	}

	if (CalledFromClient)
	{
	ability->ClientToggleHidden();
	}
	else
	{
	ability->ToggleHidden();
	}

	SetCasting(!bIsCasting);
	}*/
}

bool UOL_AbilityManager::ServerActivateAbility_Validate(int32 aIndex, bool calledFromClient)
{
	return true;
}

void UOL_AbilityManager::ServerActivateAbility_Implementation(int32 aIndex, bool calledFromClient)
{
	ActivateAbility(aIndex, calledFromClient);
}

void UOL_AbilityManager::DeActivateAbility(int32 AbilityIndex, bool CalledFromClient)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerDeActivateAbility(AbilityIndex, true);
		return;
	}
	//AOL_PlayerController* OL_PC = Cast<AOL_PlayerController>(Controller);

	/*if (!OL_PC)
	{
	return;
	}

	AOL_Ability* ability = OL_PC->GetAbility(AbilityIndex);

	if (ability)
	{
	if (CalledFromClient)
	{
	ability->ClientToggleHidden();
	}
	else
	{
	ability->ToggleHidden();
	}
	SetCasting(!bIsCasting);
	}*/
}

bool UOL_AbilityManager::ServerDeActivateAbility_Validate(int32 aIndex, bool CalledFromClient)
{
	return true;
}

void UOL_AbilityManager::ServerDeActivateAbility_Implementation(int32 aIndex, bool CalledFromClient)
{
	DeActivateAbility(aIndex, CalledFromClient);
}

