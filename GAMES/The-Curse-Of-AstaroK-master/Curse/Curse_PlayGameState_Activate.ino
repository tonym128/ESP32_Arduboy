#include "src/utils/Arduboy2Ext.h"

constexpr World world PROGMEM {
	30,  /* cloudsXPos */
	120, /* buildingXPos */
	60,  /* foregroundXPos */
	// -80,  /* cloudsXPos */
	// -170, /* buildingXPos */
	// -110,  /* foregroundXPos */
	1,  /* dungeon */
	{ 
		48, 159,
	},
	{
		TownItem(ItemType::HP, -140, RenderPosition::FrontOfBuildings),
		TownItem(ItemType::NPC, Constants::NPC_Locations[0], RenderPosition::FrontOfBuildings),
		TownItem(ItemType::NPC, Constants::NPC_Locations[1], RenderPosition::FrontOfBuildings),
		TownItem(ItemType::NPC, Constants::NPC_Locations[2], RenderPosition::FrontOfBuildings),
		TownItem(ItemType::Pumpkin, -155),
		TownItem(ItemType::Castle_01, 293),
		TownItem(ItemType::Castle_02, 293, RenderPosition::FrontOfPlayer),
		TownItem(ItemType::Crates, -256, RenderPosition::FrontOfBuildings),
		TownItem(ItemType::Tombstone, Constants::Tombstone_Locations[0], RenderPosition::FrontOfBuildings),
		TownItem(ItemType::Tombstone, Constants::Tombstone_Locations[1], RenderPosition::FrontOfBuildings),
		TownItem(ItemType::Tombstone, Constants::Tombstone_Locations[2], RenderPosition::FrontOfBuildings),
		TownItem(ItemType::SavingPost, Constants::SavingPost_Location, RenderPosition::FrontOfBuildings),
	}
};

constexpr uint8_t player[] PROGMEM {  
	100, //HP
	5, // gp
	0, // sp
	0, // def
	Constants::Player_Ground_Level, // y	
	0, // frame
	0, // recast
	0, // jumping
	0, // jumpingIndex
	0, 0, 0, 0, // RuneCombinations_Count
	static_cast<uint8_t>(PlayerDirection::IdleRight), // playerDirection
	static_cast<uint8_t>(HelmetType::None), // helmetType
	0,0,0,0,0,0,0, // Player items
};


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void playGame_Activate() {

	playGame_ChangeViewState(ViewState::InTown);
	playGame_ResetGame();

	playGameVars.inventory.runeIdx = Constants::RuneNoValue;
	playGameVars.world.townItems[Constants::HP_Index].setEnabled(false);
	memcpy_P(&playGameVars.player, &player, sizeof(player));

	playGameVars.player.setX(Constants::Player_CentrePos);
	 
}


// ----------------------------------------------------------------------------
// Reset the game in the event that the player dies ..
//
void playGame_ResetGame() {

	memcpy_P(&playGameVars.world, &world, sizeof(world));
	memset(&playGameVars, 0, 12);
	playGameVars.numberOfRecasts = 3;

}
