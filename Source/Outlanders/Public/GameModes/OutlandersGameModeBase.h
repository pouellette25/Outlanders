// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "OutlandersGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class OUTLANDERS_API AOutlandersGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	AOutlandersGameModeBase(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
		int32 InitialUpgradePoints;
	
};
