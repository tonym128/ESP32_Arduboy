#include "Defines.h"
#include "Game.h"
#include "FixedMath.h"
#include "Draw.h"
#include "Map.h"
#include "Projectile.h"
#include "Particle.h"
#include "MapGenerator.h"
#include "Platform.h"
#include "Entity.h"
#include "Enemy.h"
#include "Menu.h"
#include "Font.h"

Player Game::players[2];
const char* Game::displayMessage = nullptr;
uint8_t Game::displayMessageTime = 0;
Game::State Game::state = Game::State::Menu;
uint8_t Game::floor = 1;
uint8_t Game::globalTickFrame = 0;
Stats Game::stats;
Menu Game::menu;
uint8_t Game::localPlayerId = 0;
char Game::localNetworkToken;

void Game::Init()
{
	state = State::EstablishingNetwork;
	localNetworkToken = PlatformNet::GenerateRandomNetworkToken();

	menu.Init();
	ParticleSystemManager::Init();
	ProjectileManager::Init();
	EnemyManager::Init();
}

void Game::StartGame()
{
	globalTickFrame = 0;
	SeedRandom(0);
	floor = 1;
	stats.Reset();
	players[0].Init();
	players[1].Init();
	SwitchState(State::EnteringLevel);
}

void Game::SwitchState(State newState)
{
	if(state != newState)
	{
		state = newState;
		menu.ResetTimer();
	}
}

void Game::ShowMessage(const char* message)
{
	constexpr uint8_t messageDisplayTime = 90;

	displayMessage = message;
	displayMessageTime = messageDisplayTime;
}

void Game::NextLevel()
{
	if (floor == 10)
	{
		GameOver();
	}
	else
	{
		floor++;
		SwitchState(State::EnteringLevel);
	}
}

void Game::Respawn()
{

}

void Game::StartLevel()
{
	ParticleSystemManager::Init();
	ProjectileManager::Init();
	EnemyManager::Init();
	MapGenerator::Generate();
	EnemyManager::SpawnEnemies();

	players[0].NextLevel();
	players[1].NextLevel();

	Platform::ExpectLoadDelay();
	SwitchState(State::InGame);
}

void Game::Draw()
{
	switch(state)
	{
		case State::EstablishingNetwork:
		case State::SendSyncMessage:
		case State::RecvSyncMessage:
			menu.DrawEstablishingNetwork();
			break;
		case State::Menu:
			//menu.Draw();
			break;
		case State::EnteringLevel:
			menu.DrawEnteringLevel();
			break;
		case State::InGame:
		{
			Player& player = GetLocalPlayer();
			Renderer::camera.x = player.x;
			Renderer::camera.y = player.y;
			Renderer::camera.angle = player.angle;

			Renderer::Render();
		}
			break;
		case State::GameOver:
			//menu.DrawGameOver();
			break;
		case State::FadeOut:
			menu.FadeOut();
			break;
	}
}

bool Game::TickInGame()
{
	static bool waitingForRead = false;
	static uint8_t localInput = 0;

	if (!PlatformNet::IsAvailableForWrite())
	{
		return false;
	}

	if (!waitingForRead)
	{
		localInput = Platform::GetInput();

		PlatformNet::Write(localInput);
	}

	uint8_t remoteInput;

	if (PlatformNet::IsAvailable())
	{
		remoteInput = PlatformNet::Read();
		waitingForRead = false;
	}
	else
	{
		waitingForRead = true;
		return false;
	}
	

	if (displayMessageTime > 0)
	{
		displayMessageTime--;
		if (displayMessageTime == 0)
			displayMessage = nullptr;
	}

	ProjectileManager::Update();
	ParticleSystemManager::Update();
	EnemyManager::Update();

	for (int n = 0; n < 2; n++)
	{
		Player& player = players[n];
		player.Tick(localPlayerId == n ? localInput : remoteInput);

		if (player.hp == 0)
		{
			GameOver();
			break;
		}

		if (Map::GetCell(player.x / CELL_SIZE, player.y / CELL_SIZE) == CellType::Exit)
		{
			NextLevel();
			break;
		}
	}
	
	return true;
}

bool Game::Tick()
{
	bool success = true;

	switch(state)
	{
		case State::EstablishingNetwork:
			ConnectToNetwork();
			break;
		case State::SendSyncMessage:
		case State::RecvSyncMessage:
			SyncNetwork();
			break;
		case State::InGame:
		case State::FadeOut:
			success = TickInGame();
			break;
		case State::EnteringLevel:
			menu.TickEnteringLevel();
			break;
		case State::Menu:
			//menu.Tick();
			break;
		case State::GameOver:
			//menu.TickGameOver();
			break;
	}

	if (success)
	{
		globalTickFrame++;
	}
	return success;
}

void Game::GameOver()
{
	SwitchState(State::FadeOut);
}

void Game::SyncNetwork()
{
	constexpr uint8_t syncCode = 0;

	if (state == State::SendSyncMessage && PlatformNet::IsAvailableForWrite())
	{
		PlatformNet::Write(syncCode);
		state = State::RecvSyncMessage;
	}

	if (state == State::RecvSyncMessage && PlatformNet::IsAvailable())
	{
		if (PlatformNet::Read() == syncCode)
		{
			StartGame();
		}
	}
}

void Game::ConnectToNetwork()
{
	if (PlatformNet::IsAvailableForWrite())
	{
		static uint8_t pingTimer = 0;
   
		if (pingTimer == 0)
		{
			PlatformNet::Write(localNetworkToken);
			pingTimer = 30;
		}
		else pingTimer--;
	}
 
	if (PlatformNet::IsAvailable())
	{
		char remoteToken = PlatformNet::Read();

		// Flush input buffer
		while (PlatformNet::IsAvailable())
		{
			if (PlatformNet::Peek() == remoteToken)
			{
				PlatformNet::Read();
			}
		}

		if (remoteToken == localNetworkToken)
		{
			// Token clash: change up token and try again
			localNetworkToken = PlatformNet::GenerateRandomNetworkToken();
		}
		else
		{
			// Sync network
			localPlayerId = localNetworkToken < remoteToken ? 0 : 1;
			SwitchState(State::SendSyncMessage);
		}
	}
}


Player& Game::GetLocalPlayer()
{
	return players[localPlayerId]; 
}

Player& Game::GetRemotePlayer()
{
	return players[!localPlayerId];
}

void Stats::Reset()
{
	killedBy = EnemyType::None;
	chestsOpened = 0;
	coinsCollected = 0;
	crownsCollected = 0;
	scrollsCollected = 0;

	for (uint8_t& killCounter : enemyKills)
	{
		killCounter = 0;
	}
}
