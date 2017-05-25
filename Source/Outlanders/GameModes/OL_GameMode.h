

#pragma once

#include "GameFramework/GameMode.h"
#include "../Characters/OL_PlayerState.h"
#include "OL_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class OUTLANDERS_API AOL_GameMode : public AGameMode
{
	GENERATED_BODY()
	AOL_GameMode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
		int32 InitialUpgradePoints;
	
};
