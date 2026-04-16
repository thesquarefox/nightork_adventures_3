// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef MONSTERMANAGER_H
#define MONSTERMANAGER_H

#include <irrlicht.h>
#include <vector>

namespace Nightork
{

class CCollisionManager;
class CDunGenProxy;
class CMonster;
class CPowerUpManager;
class CProjectileManager;
class CSoundEffects;

class CMonsterManager
{
public:
	/// Constructor.
	CMonsterManager(irr::IrrlichtDevice* irrDevice_, CSoundEffects* soundEffects_, CCollisionManager* collisionManager_,
		CPowerUpManager* powerupManager_, CProjectileManager* projectileManager_, CDunGenProxy* dunGen_);

	/// Destructor.
	~CMonsterManager();

	/// Updates all monsters.
	void Update(clock_t actTime_);

	/// Removes destroyed monsters.
	void Cleanup();

	/// Returns the monster for the node. NULL if node isn't a monster.
	CMonster* GetMonster(irr::scene::ISceneNode* node_);

	/// Returns the Monsters.
	const std::vector<CMonster*>* GetMonsters() const;

	/// Drops life of a monster.
	void DropLife(irr::scene::ISceneNode* node_, unsigned int color1_, unsigned int color2_, bool overload_, bool powerUpped_);

	/// Monster has collided with player ship object.
	void Collision(irr::scene::ISceneNode* node_);

	/// Returns the smallest distance of the player ship to a monster.
	float GetNearestDistancePlayerShipToMonster() const;

	/// Returns the current number of monsters.
	unsigned int GetNumberMonsters() const;

	/// Returns the currently active critical destructions.
	const std::vector<std::pair<irr::core::vector3df, clock_t> >* GetCriticalDestructions() const;

private:
	CSoundEffects* SoundEffects;			///< The sound effects.
	CCollisionManager* CollisionManager;	///< The collision manager.
	CPowerUpManager* PowerUpManager;		///< The powerup manager.
	CProjectileManager* ProjectileManager;	///< The projectile manager.
	irr::scene::ISceneNode* PlayerShip;		///< The player ship scene node.

	std::vector<CMonster*> Monsters;        ///< The active monster ships.
	std::vector<std::pair<irr::core::vector3df, clock_t> > CriticalDestructions; ///< The position of monster ships with critical destructions.
};
}

#endif
