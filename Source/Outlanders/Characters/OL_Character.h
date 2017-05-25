// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "OL_PlayerState.h"
#include "PlayerStats.h"
#include "OL_Character.generated.h"

/** replicated information on a hit we've taken */
USTRUCT()
struct FTakeHitInfo
{
	GENERATED_USTRUCT_BODY()

	/** The amount of damage actually applied */
	UPROPERTY()
	float ActualDamage;

	/** The damage type we were hit with. */
	UPROPERTY()
		UClass* DamageTypeClass;

	/** Who hit us */
	UPROPERTY()
		TWeakObjectPtr<class AOL_Character> PawnInstigator;

	/** Who actually caused the damage */
	UPROPERTY()
		TWeakObjectPtr<class AActor> DamageCauser;

	/** Specifies which DamageEvent below describes the damage received. */
	UPROPERTY()
		int32 DamageEventClassID;

	/** Rather this was a kill */
	UPROPERTY()
		uint32 bKilled : 1;

private:

	/** A rolling counter used to ensure the struct is dirty and will replicate. */
	UPROPERTY()
		uint8 EnsureReplicationByte;

	/** Describes general damage. */
	UPROPERTY()
		FDamageEvent GeneralDamageEvent;

	/** Describes point damage, if that is what was received. */
	UPROPERTY()
		FPointDamageEvent PointDamageEvent;

	/** Describes radial damage, if that is what was received. */
	UPROPERTY()
		FRadialDamageEvent RadialDamageEvent;

public:
	FTakeHitInfo()
		: ActualDamage(0)
		, DamageTypeClass(NULL)
		, PawnInstigator(NULL)
		, DamageCauser(NULL)
		, DamageEventClassID(0)
		, bKilled(false)
		, EnsureReplicationByte(0)
	{}

	FDamageEvent& GetDamageEvent()
	{
		switch (DamageEventClassID)
		{
		case FPointDamageEvent::ClassID:
			if (PointDamageEvent.DamageTypeClass == NULL)
			{
				PointDamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass();
			}
			return PointDamageEvent;

		case FRadialDamageEvent::ClassID:
			if (RadialDamageEvent.DamageTypeClass == NULL)
			{
				RadialDamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass();
			}
			return RadialDamageEvent;

		default:
			if (GeneralDamageEvent.DamageTypeClass == NULL)
			{
				GeneralDamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass();
			}
			return GeneralDamageEvent;
		}
	}

	void SetDamageEvent(const FDamageEvent& DamageEvent)
	{
		DamageEventClassID = DamageEvent.GetTypeID();
		switch (DamageEventClassID)
		{
		case FPointDamageEvent::ClassID:
			PointDamageEvent = *((FPointDamageEvent const*)(&DamageEvent));
			break;
		case FRadialDamageEvent::ClassID:
			RadialDamageEvent = *((FRadialDamageEvent const*)(&DamageEvent));
			break;
		default:
			GeneralDamageEvent = DamageEvent;
		}

		DamageTypeClass = DamageEvent.DamageTypeClass;
	}

	void EnsureReplication()
	{
		EnsureReplicationByte++;
	}
};


class AOL_Ability;

/**
 * 
 */
UCLASS(abstract)
class OUTLANDERS_API AOL_Character : public ACharacter
{
	GENERATED_BODY()
	
public:

	AOL_Character(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	//////////////////////////////////////////////////////////////////////////
	// Input handlers

	/** setup pawn specific input handlers */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/**
	* Move forward/back
	*
	* @param Val Movment input to apply
	*/
	void MoveForward(float Val);

	/**
	* Strafe right/left
	*
	* @param Val Movment input to apply
	*/
	void MoveRight(float Val);

	/**
	* Mouse look up/down.
	*
	* @param Val Movment input to apply
	*/
	void Look(float Val);

	void CancelFire();

	void StartFire();
	
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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Default)
		float AmbientExpPerTick;

	/** get default health */
	/*float GetDefaultHealth() const;
	float GetDefaultMana() const;
	float GetDefaultExp() const;*/

	void ConsumeResources(AOL_Ability* Ability);

	/** Take damage, handle death */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual void UnPossessed() override;
	 
	/**
	* Kills pawn.  Server/authority only.
	* @param KillingDamage - Damage amount of the killing blow
	* @param DamageEvent - Damage event of the killing blow
	* @param Killer - Who killed this pawn
	* @param DamageCauser - the Actor that directly caused the damage (i.e. the Projectile that exploded, the Weapon that fired, etc)
	* @returns true if allowed
	*/
	virtual bool Die(float KillingDamage, struct FDamageEvent const& DamageEvent, class AController* Killer, class AActor* DamageCauser);

	void AbilityKey1Pressed();
	void AbilityKey2Pressed();
	void AbilityKey3Pressed();
	void AbilityKey4Pressed();

	void UpgradeAbilityKey1Pressed();
	void UpgradeAbilityKey2Pressed();
	void UpgradeAbilityKey3Pressed();
	void UpgradeAbilityKey4Pressed();

	void SpendUpgradePoint(int32 abilityIndex);

	UFUNCTION(server, reliable, WithValidation)
		void ServerSpendUpgradePoint(int32 abilityIndex);
		bool ServerSpendUpgradePoint_Validate(int32 abilityIndex);
		void ServerSpendUpgradePoint_Implementation(int32 abilityIndex);

	UFUNCTION(BlueprintCallable, Category=Mesh)
		USkeletalMeshComponent* GetMesh1P() const;

	/** cleanup inventory */
	virtual void Destroyed() override;

	void LevelUp();

	bool bIsDying;

	/** Returns True if the pawn can die in the current state */
	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const;

	FTimerHandle TimerHandle_TryGetPlayerState;
	FTimerHandle TimerHandle_AmbientExp;
	FTimerHandle TimerHandle_Regen;

private:
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
		USkeletalMeshComponent* Mesh1P;

	void SetCasting(bool newCasting);
	UFUNCTION(server, reliable, WithValidation)
		void ServerSetCasting(bool newCasting);
		bool ServerSetCasting_Validate(bool newCasting);
		void ServerSetCasting_Implementation(bool newCasting);

	void SetStrafing(float Val);
	UFUNCTION(server, reliable, WithValidation)
		void ServerSetStrafing(float Val);
		bool ServerSetStrafing_Validate(float Val);
		void ServerSetStrafing_Implementation(float Val);

	void SetWalking(float Val);
	UFUNCTION(server, reliable, WithValidation)
		void ServerSetWalking(float Val);
		bool ServerSetWalking_Validate(float Val);
		void ServerSetWalking_Implementation(float Val);

	void SetPitch();
	UFUNCTION(server, reliable, WithValidation)
		void ServerSetPitch();
		bool ServerSetPitch_Validate();
		void ServerSetPitch_Implementation();

	UFUNCTION(server, reliable, WithValidation)
		void ServerStartFire();
		bool ServerStartFire_Validate();
		void ServerStartFire_Implementation();

	void TryGetPlayerState();

	void GiveAmbientExp();
	void OnRegen();

	void InitializeAbilities();

	void SetLevel();
	void CheckLevelUp();

	FVector GetDeathMomentum(struct FDamageEvent const& DamageEvent, class AActor* DamageCauser);

	int32 Level;

	FVector DeathMomentum;

protected:

	UPROPERTY(Transient, Replicated, BlueprintReadOnly)
		bool bIsMovingBackwards;

	UPROPERTY(Transient, Replicated, BlueprintReadOnly)
		bool bIsMovingForward;

	UPROPERTY(Transient, Replicated, BlueprintReadOnly)
		bool bIsStrafingLeft;

	UPROPERTY(Transient, Replicated, BlueprintReadOnly)
		bool bIsStrafingRight;

	UPROPERTY(Transient, Replicated, BlueprintReadOnly)
		bool bIsCasting;

	UPROPERTY(Transient, Replicated, BlueprintReadOnly)
		float Pitch;

	/* The currently active ability. Client Only */
	UPROPERTY(BlueprintReadOnly, Category = Ability)
		int32 CurrentActiveAbilityIndex;

	UPROPERTY(EditDefaultsOnly, Category=Ability)
		TArray<TSubclassOf<AOL_Ability>> AbilityClasses;

	UFUNCTION(server, reliable, WithValidation)
		void ServerActivateAbility(int32 aIndex, bool calledFromClient = false);
		bool ServerActivateAbility_Validate(int32 aIndex, bool calledFromClient = false);
		void ServerActivateAbility_Implementation(int32 aIndex, bool calledFromClient = false);

	UFUNCTION(server, reliable, WithValidation)
		void ServerDeActivateAbility(int32 aIndex, bool CalledFromClient = false);
		bool ServerDeActivateAbility_Validate(int32 aIndex, bool CalledFromClient = false);
		void ServerDeActivateAbility_Implementation(int32 aIndex, bool CalledFromClient = false);

	void DetachAbilities();

	virtual void TornOff();

	void SetRagdollPhysics();

	/** notification when killed, for both the server and client. */
	virtual void OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser);

	/** sets up the replication for taking a hit */
	void ReplicateHit(float Damage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser, bool bKilled);

	/** Replicate where this pawn was last hit and damaged */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_LastTakeHitInfo)
		FTakeHitInfo LastTakeHitInfo;

	/** play hit or death on client */
	UFUNCTION()
		void OnRep_LastTakeHitInfo();

	/** Time at which point the last take hit info for the actor times out and won't be replicated; Used to stop join-in-progress effects all over the screen */
	float LastTakeHitTimeTimeout;

	UPROPERTY(EditDefaultsOnly, Category = Default)
		float ExpReward;

	UPROPERTY(EditDefaultsOnly, Instanced, Replicated, Category = Attributes)
		UPlayerStats* PlayerStats;
};
