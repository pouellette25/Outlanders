

#include "Outlanders.h"

// General Logging
DEFINE_LOG_CATEGORY(OL);

//// Game Mode Logging
//DEFINE_LOG_CATEGORY(OL_Game);
//
//// Menu Logging
//DEFINE_LOG_CATEGORY(OL_Menu);
//
//// HUD Logging
//DEFINE_LOG_CATEGORY(OL_HUD);


class FOutlandersGameModule : public FDefaultGameModuleImpl
{
	
};

IMPLEMENT_PRIMARY_GAME_MODULE(FOutlandersGameModule, Outlanders, "Outlanders");
