// Copyright LP Creative Ventures LLC.

#include "Outlanders.h"
#include "OL_PlayerStats.h"


// Sets default values for this component's properties
UOL_PlayerStats::UOL_PlayerStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOL_PlayerStats::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UOL_PlayerStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

