// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include <irrlicht.h>
#include <time.h>
#include <vector>
#include "Color.h"
#include "MonsterTypes.h"

namespace Nightork
{

class CCollisionManager;
class CMonsterManager;
class CPlayer;

class CProjectileManager
{
public:
	struct SProjectilePlayer
	{
		irr::scene::IMeshSceneNode* Projectile;				///< The projectile.
		irr::scene::IParticleSystemSceneNode* Particle;		///< The particle system projectile.
		irr::core::vector3df Direction;						///< The direction for the projectile.
		float Speed;										///< Speed of the projectile.
		CMonster* Monster;									///< The target monster, can be null.
		EColor::Enum Color;									///< The color of the projectile.
		bool PowerUpped;									///< Overloaded through power up.
		int Lifetime;										///< The lifetime of the projectile.
	};

	struct SProjectileMonster
	{
		irr::scene::IBillboardSceneNode* Projectile;		///< The projectile.
		irr::core::vector3df Direction;						///< The direction for the projectile.
		int Lifetime;										///< The lifetime of the projectile.
	};

public:
	/// Constructor.
	CProjectileManager(irr::IrrlichtDevice* irrDevice_, CCollisionManager* collisionManager_, CDunGenProxy* dunGen_);

	/// Destructor.
	~CProjectileManager();

	/// Initialize.
	void Initialize(CMonsterManager* monsterManager_, CPlayer* playerShip_);

	/// Updates all projectiles.
	void Update(clock_t actTime_);

	/// Cleanup projectile references.
	void Cleanup();

	/// Creates a projectile for the player object.
	void CreateProjectilePlayer(const irr::core::vector3df& pos_, const irr::core::vector3df& dir_, CMonster* monster_, EColor::Enum color_, bool powerUpped_);

	/// Creates a projectile for a monster object.
	void CreateProjectileMonster(const irr::core::vector3df& pos_, const irr::core::vector3df& dir_, EMonsterType::Enum type_);

private:
	irr::scene::ISceneManager* SceneManager;			///< The scene manager.
	irr::video::IVideoDriver* VideoDriver;				///< The video driver.
	CCollisionManager* CollisionManager;				///< The collision manager.
	CDunGenProxy* DunGenProxy;							///< Access to the game settings.
	CMonsterManager* MonsterManager;					///< The monster manager.
	CPlayer* PlayerShip;								///< The player ship.

	std::vector<SProjectilePlayer> ProjectilesPlayer;	///< The projectiles for the player object.
	std::vector<SProjectileMonster> ProjectilesMonster;	///< The projectiles for the monster objects.
	irr::scene::ISceneNodeAnimator* ImplosionMonsterProjectile;	///< Implosion animator for monster projectiles.
	clock_t LastUpdateTick;								///< The time of the last update tick.
};

}

#endif
