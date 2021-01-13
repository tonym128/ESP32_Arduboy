#pragma once

#include <stdint.h>
#include "Defines.h"
#include "Player.h"
#include "Enemy.h"
#include "Menu.h"

class Entity;

struct Stats
{
	EnemyType killedBy;
	uint8_t enemyKills[(int)EnemyType::NumEnemyTypes];
	uint8_t chestsOpened;
	uint8_t crownsCollected;
	uint8_t scrollsCollected;
	uint8_t coinsCollected;

	void Reset();
};

class Game
{
public:
	static uint8_t globalTickFrame;

	enum class State : uint8_t
	{
		EstablishingNetwork,
		SendSyncMessage,
		RecvSyncMessage,
		Menu,
		EnteringLevel,
		InGame,
		GameOver,
		FadeOut
	};

	static void Init();
	static bool Tick();
	static void Draw();

	static void StartGame();
	static void StartLevel();
	static void NextLevel();
	static void GameOver();
	static void Respawn();
	
	static void SwitchState(State newState);

	static void ShowMessage(const char* message);

	static Player& GetLocalPlayer();
	static Player& GetRemotePlayer();

	static Player players[2];
	static uint8_t localPlayerId;

	static const char* displayMessage;
	static uint8_t displayMessageTime;
	static uint8_t floor;

	static Stats stats;

private:
	static bool TickInGame();

	static void ConnectToNetwork();
	static void SyncNetwork();
		
	static Menu menu;
	static State state;
	static char localNetworkToken;
};
