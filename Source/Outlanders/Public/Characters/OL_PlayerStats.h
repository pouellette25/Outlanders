// Copyright LP Creative Ventures LLC.

#pragma once

#include "Components/ActorComponent.h"
#include "OL_PlayerStats.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OUTLANDERS_API UOL_PlayerStats : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UOL_PlayerStats();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FUpgradeableParameter MagicalPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FUpgradeableParameter PhysicalPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FUpgradeableParameter HP5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FUpgradeableParameter MP5;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attributes")
		FUpgradeableParameter Mana;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attributes")
		FUpgradeableParameter Health;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attributes")
		FUpgradeableParameter Experience;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
};
