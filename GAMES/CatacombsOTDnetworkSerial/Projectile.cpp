#include "Defines.h"
#include "Projectile.h"
#include "Map.h"
#include "FixedMath.h"
#include "Particle.h"
#include "Enemy.h"
#include "Generated/SpriteTypes.h"
#include "Platform.h"
#include "Sounds.h"

Projectile ProjectileManager::projectiles[ProjectileManager::MAX_PROJECTILES];

Projectile* ProjectileManager::FireProjectile(Entity* owner, int16_t x, int16_t y, uint8_t angle)
{
	for (Projectile& p : projectiles)
	{
		if(p.life == 0)
		{
			if (owner == &Game::players[0])
				p.ownerId = Projectile::player1OwnerId;
			else if (owner == &Game::players[1])
				p.ownerId = Projectile::player2OwnerId;
			else
			{
				for (uint8_t n = 0; n < EnemyManager::maxEnemies; n++)
				{
					if (&EnemyManager::enemies[n] == owner)
					{
						p.ownerId = n;
						break;
					}
				}
			}

			p.life = 255;
			p.x = x;
			p.y = y;
			p.angle = angle;
			return &p;
		}
	}

	return nullptr;
}

Entity* Projectile::GetOwner() const
{
	if (ownerId == player1OwnerId)
		return &Game::players[0];
	else if (ownerId == player2OwnerId)
		return &Game::players[1];
	return &EnemyManager::enemies[ownerId];
}

void ProjectileManager::Update()
{
	for (Projectile& p : projectiles)
	{
		if(p.life > 0)
		{
			p.life--;

			int16_t deltaX = FixedCos(p.angle) / 4;
			int16_t deltaY = FixedSin(p.angle) / 4;

			p.x += deltaX;
			p.y += deltaY;

			bool hitAnything = false;

			Entity* owner = p.GetOwner();

			if (Map::IsBlockedAtWorldPosition(p.x, p.y))
			{
				uint8_t cellX = p.x / CELL_SIZE;
				uint8_t cellY = p.y / CELL_SIZE;

				if (Map::GetCellSafe(cellX, cellY) == CellType::Urn)
				{
					Map::SetCell(cellX, cellY, CellType::Empty);
					ParticleSystemManager::CreateExplosion(cellX * CELL_SIZE + CELL_SIZE / 2, cellY * CELL_SIZE + CELL_SIZE / 2, true);

					switch ((Random() % 5))
					{
					case 0:
						EnemyManager::Spawn(EnemyType::Spider, cellX * CELL_SIZE + CELL_SIZE / 2, cellY * CELL_SIZE + CELL_SIZE / 2);
						break;
					case 1:
						Map::SetCell(cellX, cellY, CellType::Potion);
						break;
					case 2:
						Map::SetCell(cellX, cellY, CellType::Coins);
						break;
					}
					Platform::PlaySound(Sounds::Kill);
				}
				else
				{
					Platform::PlaySound(Sounds::Hit);
				}

				hitAnything = true;
			}
			else
			{
				uint8_t damageAmount = 0;
				EnemyType ownerType;

				if (owner == &Game::players[0] || owner == &Game::players[1])
				{
					ownerType = EnemyType::Player;
					damageAmount = Player::attackStrength;

					Enemy* overlappingEnemy = EnemyManager::GetOverlappingEnemy(p.x, p.y);
					if (overlappingEnemy)
					{
						overlappingEnemy->Damage(owner, damageAmount);
						hitAnything = true;
					}
				}
				else
				{
					const EnemyArchetype* enemyArchetype = ((Enemy*)owner)->GetArchetype();
					ownerType = ((Enemy*)owner)->GetType();
					if (enemyArchetype)
					{
						damageAmount = enemyArchetype->GetAttackStrength();
					}
				}

				if (!hitAnything)
				{
					for (int n = 0; n < 2; n++)
					{
						Player& player = Game::players[n];
						if (owner != &player && player.IsOverlappingPoint(p.x, p.y))
						{
							player.Damage(damageAmount);
							if (player.hp == 0)
							{
								Game::stats.killedBy = ownerType;
							}
							hitAnything = true;
						}
					}
				}
			}

			if (hitAnything)
			{
				ParticleSystemManager::CreateExplosion(p.x - deltaX, p.y - deltaY);
				p.life = 0;
			}
		}
	}	
}

void ProjectileManager::Init()
{
	for (Projectile& p : projectiles)
	{
		p.life = 0;
	}
}

void ProjectileManager::Draw()
{
	for(Projectile& p : projectiles)
	{
		if (p.life > 0)
		{
			Renderer::DrawObject(p.ownerId == Projectile::GetLocalPlayerOwnerId() ? projectileSpriteData : enemyProjectileSpriteData, p.x, p.y, 32, AnchorType::BelowCenter);
		}
	}
}

uint8_t Projectile::GetLocalPlayerOwnerId()
{
	return Game::localPlayerId ? player2OwnerId : player1OwnerId;
}
