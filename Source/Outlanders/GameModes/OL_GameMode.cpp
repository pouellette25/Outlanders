

#include "Outlanders.h"
#include "OL_GameMode.h"

AOL_GameMode::AOL_GameMode(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(TEXT("'/Game/Characters/Mage/Mage'"));
	DefaultPawnClass = PlayerPawnObject.Class;

	PlayerControllerClass = AOL_PlayerController::StaticClass();
	PlayerStateClass = AOL_PlayerState::StaticClass();

	InitialUpgradePoints = 1;
}

void AOL_GameMode::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Begin Play Called in Game Mode"));
	}
}


