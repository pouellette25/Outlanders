

#pragma once

#include "Engine.h"
#include "UnrealNetwork.h"
#include "OutlandersTypes.h"

#define COLLISION_PROJECTILE	ECC_GameTraceChannel1

//// General Logging
DECLARE_LOG_CATEGORY_EXTERN(OL, Log, All)
//
//// Game Mode Logging
//DECLARE_LOG_CATEGORY_EXTERN(OL_Game, Log, All)
//
//// Menu Logging
//DECLARE_LOG_CATEGORY_EXTERN(OL_Menu, Log, All)
//
//// HUD Logging
//DECLARE_LOG_CATEGORY_EXTERN(OL_HUD, Log, All)

#define ISDEDICATED (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
#define ISLISTEN (GEngine->GetNetMode(GetWorld()) == NM_ListenServer)
#define ISSTANDALONE (GEngine->GetNetMode(GetWorld()) == NM_Standalone)
#define ISCLIENT (GEngine->GetNetMode(GetWorld()) == NM_Client)

#define NETMODE_WORLD (((GEngine == NULL) || (GetWorld() == NULL)) ? TEXT("") \
	: ISCLIENT ? TEXT("[Client] ") \
	: ISLISTEN ? TEXT("[ListenServer] ") \
	: ISDEDICATED ? TEXT("[DedicatedServer] ") \
	: TEXT("[Standalone] "))

#if _MSC_VER
#define FUNC_NAME    TEXT(__FUNCTION__)
#else // FIXME - GCC?
#define FUNC_NAME    TEXT(__func__)
#endif

#define TRACE(Format, ...) \
{\
	SET_WARN_COLOR(COLOR_CYAN); \
	FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(OL, Warning, TEXT("%s%s() : %s"), NETMODE_WORLD, FUNC_NAME, *Msg); \
	CLEAR_WARN_COLOR(); \
}



