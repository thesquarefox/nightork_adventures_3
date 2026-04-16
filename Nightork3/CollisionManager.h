// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <irrlicht.h>
#include <vector>

namespace Nightork
{

class CDunGenProxy;
class CMonster;
class CMonsterManager;

class CCollisionManager
{
public:
	/// Constructor.
	CCollisionManager(irr::IrrlichtDevice* irrDevice_, CDunGenProxy* dunGen_);

	/// Destructor.
	~CCollisionManager();

	/// Initialize.
	void Initialize(CMonsterManager* monsterManager_);

	/// Sets the monsters as objects for collision.
	void SetMonsters(std::vector<CMonster*>* monsters_);

	/// Returns the new position of the player object.
	irr::core::vector3df GetPlayerPosition(const irr::core::vector3df &ellipsoidPosition_,
		const irr::core::vector3df& ellipsoidRadius_,
		const irr::core::vector3df& ellipsoidDirectionAndSpeed_);

	/// Computes the collision of the player ship with monsters. Returns true if a collision occurred.
	bool PlayerMonsterCollision(const irr::core::vector3df &ellipsoidPosition_,
		const irr::core::vector3df& ellipsoidRadius_,
		const irr::core::vector3df& ellipsoidDirectionAndSpeed_);

	/// Computes the collision point with the cave. Returns true on collision.
	bool GetCollisionPointWithCave(const irr::core::line3df& ray_, irr::core::vector3df& outCollisionPoint_);

	/// Computes the collision point with the portal. Returns true on collision.
	bool GetCollisionPointWithPortal(const irr::core::line3df& ray_, irr::core::vector3df& outCollisionPoint_);

	/// Computes the collision point with the player ship. Returns true on collision.
	bool GetCollisionPointWithPlayer(const irr::core::line3df& ray_, irr::core::vector3df& outCollisionPoint_);

	/// Computes the collision point with the monsters. Returns true on collision.
	bool GetCollisionPointWithMonster(const irr::core::line3df& ray_, irr::core::vector3df& outCollisionPoint_, irr::scene::ISceneNode*& outNode_);

private:
	/// Finds the nearest monster node to the given position.
	irr::scene::ISceneNode* FindNearestMonsterNode(const irr::core::vector3df& position_) const;

private:
	irr::scene::ISceneManager* SceneManager;											///< The scene manager.
	irr::scene::ISceneCollisionManager* SceneCollisionManager;							///< The scene collision manager.
	irr::scene::ITriangleSelector* CaveSelector;										///< The cave triangle selector.
	irr::scene::ITriangleSelector* PortalSelector;										///< The portal triangle selector.
	irr::scene::ITriangleSelector* PlayerShipSelector;									///< The player ship triangle selector.
	irr::scene::IMetaTriangleSelector* MonsterSelector;									///< The monster triangle selector.
	CDunGenProxy* DunGenProxy;															///< The proxy to the dungeon generator.

	CMonsterManager* MonsterManager;													///< The monster manager.

	std::vector<CMonster*>* Monsters;													///< The monsters.
};

}

#endif
