// Fill out your copyright notice in the Description page of Project Settings.

#include "Outlanders.h"
#include "AbilitySystem/OL_Ability.h"
#include "OL_Character.h"

AOL_Character::AOL_Character(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	Mesh1P = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Mesh1P"));
	Mesh1P->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	Mesh1P->bOnlyOwnerSee = true;
	Mesh1P->bOwnerNoSee = false;
	Mesh1P->bReceivesDecals = false;
	Mesh1P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh1P->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	//Mesh1P->bChartDistanceFactor = false;
	Mesh1P->SetCollisionObjectType(ECC_Pawn);
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	//PlayerStats = ObjectInitializer.CreateDefaultSubobject<UPlayerStats>(this, TEXT("PlayerStats"));

	ExpReward = 50;
}

void AOL_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AOL_Character::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		// The player state is created after the Character, so set a timer up to try to get it.
		GetWorldTimerManager().SetTimer(TimerHandle_TryGetPlayerState, this, &AOL_Character::TryGetPlayerState, .1f, false);
	}	
}

void AOL_Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(Role == ROLE_Authority) CheckLevelUp();
}

void AOL_Character::TryGetPlayerState()
{
	// Cast the player state to our custom PlayerState class
	AOL_PlayerState* OL_PlayerState = Cast<AOL_PlayerState>(PlayerState);

	// If the player state is valid and not initialized
	if (OL_PlayerState)
	{
		//AOL_GameMode* game = (AOL_GameMode*)GetWorld()->GetAuthGameMode();
		//PlayerStats->Init();
	}
	else // Player state wasn't valid
	{
		// Try to get it again
		GetWorldTimerManager().SetTimer(TimerHandle_TryGetPlayerState, this, &AOL_Character::TryGetPlayerState, .1f, false);
		return;
	}

	InitializeAbilities();

	// Clear the Get Player State timer
	GetWorldTimerManager().ClearTimer(TimerHandle_TryGetPlayerState);

	// Set timer to give experience
	GetWorldTimerManager().SetTimer(TimerHandle_AmbientExp, this, &AOL_Character::GiveAmbientExp, 2.0f, true);

	// Set timer for mp5 and hp5
	GetWorldTimerManager().SetTimer(TimerHandle_Regen, this, &AOL_Character::OnRegen, 5.0f, true);
}

void AOL_Character::InitializeAbilities()
{
	UWorld* world = GetWorld();
	TArray<AOL_Ability*> Abilities;

	//AOL_PlayerController* OL_PC = Cast<AOL_PlayerController>(Controller);

	if (world && Role == ROLE_Authority)
	{
		int32 numAbilities = 0;// OL_PC->GetNumAbilities();
		if (numAbilities <= 0)
		{ 
			// Send the classes array to our player controller so the controller spawns the abilities
			//OL_PC->InitializeAbilities(AbilityClasses, this, GetActorLocation(), GetActorRotation());
		}
		else
		{
			// Abilities already exist but we have to re-assign the instigator after the character dies
			for (int32 i = 0; i < numAbilities; i++)
			{
				/*AOL_Ability* ability = OL_PC->GetAbility(i);

				if (ability)
				{
					ability->Instigator = this;
				}*/
			}
		}
	}
}

void AOL_Character::GiveAmbientExp()
{
	if (Role == ROLE_Authority && !bIsDying)
	{
		//PlayerStats->AddExperience(AmbientExpPerTick);
	}
}

/* Timer called function that handles Mana and Health regen based off values in the upgradeable parameters */
void AOL_Character::OnRegen()
{
	// Make the server is handling this logic, the player state is valid, and the character isn't dead
	if (Role == ROLE_Authority && !bIsDying)
	{
		//PlayerStats->Regen();
	}
}

void AOL_Character::CheckLevelUp()
{
	//if (!PlayerStats || PlayerStats->GetMaxExp() <= 0 || Role < ROLE_Authority) return;

	//if (PlayerStats->GetCurrentExperience() >= PlayerStats->GetMaxExp())
	//{
	//	//PlayerStats->LevelUp();
	//}
}

float AOL_Character::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	/*float currentHealth = PlayerStats->GetCurrentHealth();
	if (currentHealth <= 0.f)
	{
		return 0.f;
	}*/
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)
	{
		//PlayerStats->AddHealth(-ActualDamage);

		/*if (PlayerStats->GetCurrentHealth() <= 0.0f)
		{
			Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
		}*/
	}

	return Damage;
}

void AOL_Character::ReplicateHit(float Damage, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser, bool bKilled)
{
	const float TimeoutTime = GetWorld()->GetTimeSeconds() + 0.5f;

	FDamageEvent const& LastDamageEvent = LastTakeHitInfo.GetDamageEvent();
	if ((PawnInstigator == LastTakeHitInfo.PawnInstigator.Get()) && (LastDamageEvent.DamageTypeClass == LastTakeHitInfo.DamageTypeClass) && (LastTakeHitTimeTimeout == TimeoutTime))
	{
		// same frame damage
		if (bKilled && LastTakeHitInfo.bKilled)
		{
			// Redundant death take hit, just ignore it
			return;
		}

		// otherwise, accumulate damage done this frame
		Damage += LastTakeHitInfo.ActualDamage;
	}

	LastTakeHitInfo.ActualDamage = Damage;
	LastTakeHitInfo.PawnInstigator = Cast<AOL_Character>(PawnInstigator);
	LastTakeHitInfo.DamageCauser = DamageCauser;
	LastTakeHitInfo.SetDamageEvent(DamageEvent);
	LastTakeHitInfo.bKilled = bKilled;
	LastTakeHitInfo.EnsureReplication();

	LastTakeHitTimeTimeout = TimeoutTime;
}

void AOL_Character::OnRep_LastTakeHitInfo()
{
	if (LastTakeHitInfo.bKilled)
	{
		OnDeath(LastTakeHitInfo.ActualDamage, LastTakeHitInfo.GetDamageEvent(), LastTakeHitInfo.PawnInstigator.Get(), LastTakeHitInfo.DamageCauser.Get());
	}
}

bool AOL_Character::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	if (bIsDying										// already dying
		|| IsPendingKill()								// already destroyed
		|| Role != ROLE_Authority						// not authority
		|| GetWorld()->GetAuthGameMode() == NULL)	// level transition occurring
	{
		return false;
	}

	return true;
}

bool AOL_Character::Die(float KillingDamage, struct FDamageEvent const& DamageEvent, class AController* Killer, class AActor* DamageCauser)
{
	if (!CanDie(KillingDamage, DamageEvent, Killer, DamageCauser))
	{
		return false;
	}

	//float currentHealth = PlayerStats->GetCurrentHealth();

	//PlayerStats->SetCurrentHealth(FMath::Min(currentHealth, 0.0f));

	GetCharacterMovement()->ForceReplicationUpdate();

	OnDeath(KillingDamage, DamageEvent, Killer ? Killer->GetPawn() : NULL, DamageCauser);

	return true;
}

void AOL_Character::OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	if (bIsDying)
	{
		return;
	}

	bReplicateMovement = false;
	bTearOff = true;
	bIsDying = true;

	if (Role == ROLE_Authority)
	{
		ReplicateHit(KillingDamage, DamageEvent, PawnInstigator, DamageCauser, true);

		// Clear timers
		GetWorldTimerManager().ClearTimer(TimerHandle_AmbientExp);
		GetWorldTimerManager().ClearTimer(TimerHandle_Regen);

		if (PawnInstigator)
		{
			AOL_Character* OtherCharacter = Cast<AOL_Character>(PawnInstigator);
			if (OtherCharacter)
			{
				//UPlayerStats* OtherPlayerStats = OtherCharacter->PlayerStats;

				//if (OtherPlayerStats)
				//{
				//	// add experience
				//	OtherPlayerStats->AddExperience(ExpReward);
				//	// add kill
				//	OtherPlayerStats->AddKill();
				//}				
			}
		}

		//PlayerStats->AddDeath();		
	}

	DeathMomentum = GetDeathMomentum(DamageEvent, DamageCauser);
	
	//DestroyAbilities();
	//DetachAbilities();

	DetachFromControllerPendingDestroy();

	USkeletalMeshComponent* UseMesh = GetMesh();
	if (UseMesh && UseMesh->AnimScriptInstance)
	{
		UseMesh->AnimScriptInstance->Montage_Stop(0.0f);
		UseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UseMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	UseMesh = GetMesh1P();
	if (UseMesh && UseMesh->AnimScriptInstance)
	{
		UseMesh->AnimScriptInstance->Montage_Stop(0.0f);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	if (GetMesh())
	{
		static FName CollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetCollisionProfileName(CollisionProfileName);
	}
	SetActorEnableCollision(true);

	SetRagdollPhysics();
}

FVector AOL_Character::GetDeathMomentum(struct FDamageEvent const& DamageEvent, class AActor* DamageCauser)
{
	if (!DamageEvent.DamageTypeClass && !DamageCauser)
	{
		return FVector(0, 0, 0);
	}

	UDamageType* DamageType = Cast<UDamageType>(DamageEvent.DamageTypeClass.GetDefaultObject());

	FVector Momentum = (GetActorLocation() - DamageCauser->GetActorLocation());

	Momentum.Normalize();

	if (DamageType)
	{
		Momentum *= DamageType->DamageImpulse;
	}
	else
	{
		Momentum *= 0;
	}

	return Momentum;
}

void AOL_Character::Destroyed()
{
	Super::Destroyed();
	//DestroyAbilities();
	//DetachAbilities();
}

void AOL_Character::UnPossessed()
{
}

void AOL_Character::TornOff()
{
	SetLifeSpan(25.f);
}

void AOL_Character::SetRagdollPhysics()
{
	bool bInRagdoll = false;

	if (IsPendingKill())
	{
		bInRagdoll = false;
	}
	else if (!GetMesh() || !GetMesh()->GetPhysicsAsset())
	{
		bInRagdoll = false;
	}
	else
	{
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;

		GetMesh()->AddImpulseAtLocation(DeathMomentum, GetActorLocation());

		bInRagdoll = true;
	}


	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	if (!bInRagdoll)
	{
		// hide and set short lifespan
		TurnOff();
		SetActorHiddenInGame(true);
		SetLifeSpan(1.0f);
	}
	else
	{
		SetLifeSpan(10.0f);
	}
}

void AOL_Character::DetachAbilities()
{
	//AOL_PlayerController* OL_PC = Cast<AOL_PlayerController>(Controller);
	if (Role < ROLE_Authority)// || !OL_PC)
	{
		return;
	}

	int32 numAbilities = 0;// OL_PC->GetNumAbilities();
	for (int32 i = 0; i < numAbilities; i++)
	{
		/*AOL_Ability* ability = OL_PC->GetAbility(i);

		if (ability)
		{
			ability->Instigator = NULL;
			ability->SetOwner(NULL);
		}*/
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AOL_Character::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AOL_Character::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AOL_Character::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &AOL_Character::Look);

	InputComponent->BindAction("Ability1", IE_Pressed, this, &AOL_Character::AbilityKey1Pressed);
	InputComponent->BindAction("Ability2", IE_Pressed, this, &AOL_Character::AbilityKey2Pressed);
	InputComponent->BindAction("Ability3", IE_Pressed, this, &AOL_Character::AbilityKey3Pressed);
	InputComponent->BindAction("Ability4", IE_Pressed, this, &AOL_Character::AbilityKey4Pressed);
	InputComponent->BindAction("UpgradeAbility1", IE_Pressed, this, &AOL_Character::UpgradeAbilityKey1Pressed);
	InputComponent->BindAction("UpgradeAbility2", IE_Pressed, this, &AOL_Character::UpgradeAbilityKey2Pressed);
	InputComponent->BindAction("UpgradeAbility3", IE_Pressed, this, &AOL_Character::UpgradeAbilityKey3Pressed);
	InputComponent->BindAction("UpgradeAbility4", IE_Pressed, this, &AOL_Character::UpgradeAbilityKey4Pressed);


	InputComponent->BindAction("CancelFire", IE_Pressed, this, &AOL_Character::CancelFire);
	//InputComponent->BindAction("Fire", IE_Pressed, this, &AOL_Character::StartFire);
}

void AOL_Character::StartFire()
{
	//if (Role < ROLE_Authority)
	//{
	//	ServerStartFire();
	//}

	if (bIsCasting)
	{
		ActivateAbility(CurrentActiveAbilityIndex);
	}
}

bool AOL_Character::ServerStartFire_Validate()
{
	return true;
}

void AOL_Character::ServerStartFire_Implementation()
{
	if (bIsCasting)
	{
		ActivateAbility(CurrentActiveAbilityIndex);
	}
}

void AOL_Character::CancelFire()
{
	if (bIsCasting)
	{
		DeActivateAbility(CurrentActiveAbilityIndex);
	}
}

void AOL_Character::Look(float Val)
{
	if (Controller && Val != 0.0f)
	{
		AddControllerPitchInput(Val);
	}

	SetPitch();

	if (Role < ROLE_Authority)
	{
		ServerSetPitch();
	}
}

void AOL_Character::MoveForward(float Val)
{
	if (Controller && Val != 0.f)
	{
		// Limit pitch when walking or falling
		const bool bLimitRotation = (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling());
		const FRotator Rotation = bLimitRotation ? GetActorRotation() : Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Val);
	}

	SetWalking(Val);

	if (Role < ROLE_Authority)
	{
		ServerSetWalking(Val);
	}
}

void AOL_Character::MoveRight(float Val)
{
	if (Controller && Val != 0.f)
	{
		const FRotator Rotation = GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Val);
	}

	SetStrafing(Val);

	if (Role < ROLE_Authority)
	{
		ServerSetStrafing(Val);
	}
}

void AOL_Character::AbilityKey1Pressed()
{
	UE_LOG(LogTemp, Warning, TEXT("STuff and things"));
	CurrentActiveAbilityIndex = 0;
	ActivateAbility(0);
}
void AOL_Character::AbilityKey2Pressed()
{
	CurrentActiveAbilityIndex = 1;
	ActivateAbility(1);
}
void AOL_Character::AbilityKey3Pressed()
{
	CurrentActiveAbilityIndex = 2;
	ActivateAbility(2);
}
void AOL_Character::AbilityKey4Pressed()
{
	CurrentActiveAbilityIndex = 3;
	ActivateAbility(3);
}

void AOL_Character::UpgradeAbilityKey1Pressed()
{
	UE_LOG(LogTemp, Warning, TEXT("More stuff and things"));
	SpendUpgradePoint(0);
}
void AOL_Character::UpgradeAbilityKey2Pressed()
{
	SpendUpgradePoint(1);
}
void AOL_Character::UpgradeAbilityKey3Pressed()
{
	SpendUpgradePoint(2);
}
void AOL_Character::UpgradeAbilityKey4Pressed()
{
	SpendUpgradePoint(3);
}

void AOL_Character::SpendUpgradePoint(int32 abilityIndex)
{
	// If not the server, call the server to spend the point
	if (Role < ROLE_Authority)
	{
		ServerSpendUpgradePoint(abilityIndex);
		return;
	}

	//AOL_PlayerController* OL_PC = Cast<AOL_PlayerController>(Controller);
	int32 numAbilities = 0;// OL_PC->GetNumAbilities();

	if (numAbilities == 0 || numAbilities <= abilityIndex)
	{
		return;
	}

	// Get the current upgrade points from the player state
	//int32 uPoints = PlayerStats->GetUpgradePoints();

	// If there's points available, upgrade the ability and subtract point
	/*if (uPoints > 0)
	{
		OL_PC->LevelUpAbility(abilityIndex);
		--uPoints;

		PlayerStats->SetUpgradePoints(uPoints);
	}*/
	
}

bool AOL_Character::ServerSpendUpgradePoint_Validate(int32 abilityIndex)
{
	return true;
}

void AOL_Character::ServerSpendUpgradePoint_Implementation(int32 abilityIndex)
{
	SpendUpgradePoint(abilityIndex);
}

void AOL_Character::BlueprintActivateAbility(int32 AbilityIndex)
{
	ActivateAbility(AbilityIndex);
}

void AOL_Character::ActivateAbility(int32 AbilityIndex, bool CalledFromClient)
{
	if (Role < ROLE_Authority)
	{
		ServerActivateAbility(AbilityIndex, true);
		return;
	}

	//AOL_PlayerController* OL_PC = Cast<AOL_PlayerController>(Controller);

	/*if (!OL_PC)
	{
		return;
	}*/

	//AOL_Ability* ability = OL_PC->GetAbility(AbilityIndex);

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

bool AOL_Character::ServerActivateAbility_Validate(int32 aIndex, bool calledFromClient)
{
	return true;
}

void AOL_Character::ServerActivateAbility_Implementation(int32 aIndex, bool calledFromClient)
{
	ActivateAbility(aIndex, calledFromClient);
}

void AOL_Character::DeActivateAbility(int32 AbilityIndex, bool CalledFromClient)
{
	if (Role < ROLE_Authority)
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

bool AOL_Character::ServerDeActivateAbility_Validate(int32 aIndex, bool CalledFromClient)
{
	return true;
}

void AOL_Character::ServerDeActivateAbility_Implementation(int32 aIndex, bool CalledFromClient)
{
	DeActivateAbility(aIndex, CalledFromClient);
}

void AOL_Character::ConsumeResources(AOL_Ability* Ability)
{
	if (Role == ROLE_Authority && Ability)
	{
		//PlayerStats->AddMana(-Ability->ManaCost.MaxValue);
	}
}

void AOL_Character::SetCasting(bool newCasting)
{
	bIsCasting = newCasting;
}

bool AOL_Character::ServerSetCasting_Validate(bool newCasting)
{
	return true;
}

void AOL_Character::ServerSetCasting_Implementation(bool newCasting)
{
	SetCasting(newCasting);
}

void AOL_Character::SetStrafing(float Val)
{
	if (Val == 0.0f)
	{
		bIsStrafingLeft = bIsStrafingRight = false;
		return;
	}

	bIsStrafingRight = Val > 0.0f;
	bIsStrafingLeft = Val < 0.0f;
}

bool AOL_Character::ServerSetStrafing_Validate(float Val)
{
	return true;
}

void AOL_Character::ServerSetStrafing_Implementation(float Val)
{
	SetStrafing(Val);
}

void AOL_Character::SetPitch()
{
	Pitch = GetControlRotation().Pitch;
}

bool AOL_Character::ServerSetPitch_Validate()
{
	return true;
}

void AOL_Character::ServerSetPitch_Implementation()
{
	SetPitch();
}

void AOL_Character::SetWalking(float Val)
{
	if (Val == 0.0f)
	{
		bIsMovingForward = bIsMovingBackwards = false;
		return;
	}

	bIsMovingForward = Val > 0.0f;
	bIsMovingBackwards = Val < 0.0f;
}

bool AOL_Character::ServerSetWalking_Validate(float Val)
{
	return true;
}

void AOL_Character::ServerSetWalking_Implementation(float Val)
{
	SetWalking(Val);
}

USkeletalMeshComponent* AOL_Character::GetMesh1P() const
{
	return Mesh1P;
}

void AOL_Character::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOL_Character, bIsCasting);

	DOREPLIFETIME_CONDITION(AOL_Character, Pitch, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AOL_Character, bIsMovingBackwards, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AOL_Character, bIsMovingForward, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AOL_Character, bIsStrafingLeft, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AOL_Character, bIsStrafingRight, COND_SkipOwner);

	DOREPLIFETIME_CONDITION(AOL_Character, LastTakeHitInfo, COND_Custom);
}