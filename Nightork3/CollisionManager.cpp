// Copyright (C) 2011-2026 by Maximilian Hönig

#include <iostream>
#include <string>
#include <time.h>
#include "CollisionManager.h"
#include "DunGenProxy.h"
#include "Monster.h"
#include "MonsterManager.h"

Nightork::CCollisionManager::CCollisionManager(irr::IrrlichtDevice* irrDevice_, CDunGenProxy* dunGen_)
	: SceneManager(irrDevice_->getSceneManager())
	, SceneCollisionManager(SceneManager->getSceneCollisionManager())
	, DunGenProxy(dunGen_)
	, Monsters(NULL)
	, MonsterManager(NULL)
{
	// create cave collision selector
	irr::scene::IMeshSceneNode* caveNode = static_cast<irr::scene::IMeshSceneNode*>(irrDevice_->getSceneManager()->getSceneNodeFromName("Cave"));
	CaveSelector = irrDevice_->getSceneManager()->createOctreeTriangleSelector(caveNode->getMesh(),caveNode);
	caveNode->setTriangleSelector(CaveSelector);
	CaveSelector->drop();

	// create portal collision selector
	irr::scene::IMeshSceneNode* portalNode = static_cast<irr::scene::IMeshSceneNode*>(irrDevice_->getSceneManager()->getSceneNodeFromName("Portal"));
	PortalSelector = irrDevice_->getSceneManager()->createOctreeTriangleSelector(portalNode->getMesh(),portalNode);
	portalNode->setTriangleSelector(PortalSelector);
	PortalSelector->drop();

	// create player ship collision selector
	irr::scene::IMeshSceneNode* playerShipNode = static_cast<irr::scene::IMeshSceneNode*>(irrDevice_->getSceneManager()->getSceneNodeFromName("PlayerShip"));
	PlayerShipSelector = irrDevice_->getSceneManager()->createOctreeTriangleSelector(playerShipNode->getMesh(),playerShipNode);
	playerShipNode->setTriangleSelector(PlayerShipSelector);
	PlayerShipSelector->drop();

	// create monster collision selector
	MonsterSelector = irrDevice_->getSceneManager()->createMetaTriangleSelector();
}

Nightork::CCollisionManager::~CCollisionManager()
{
	MonsterSelector->drop();
}

void Nightork::CCollisionManager::Initialize(CMonsterManager* monsterManager_ )
{
	MonsterManager = monsterManager_;
}

irr::scene::ISceneNode* Nightork::CCollisionManager::FindNearestMonsterNode(const irr::core::vector3df& position_) const
{
	irr::scene::ISceneNode* monsterNode = NULL;
	float currentDistSQ = FLT_MAX;
	for (unsigned int i=0; i<Monsters->size(); ++i)
	{
		irr::scene::ISceneNode* node = (*Monsters)[i]->GetMonsterNode();
		const irr::core::vector3df diff = node->getAbsolutePosition() - position_;
		const float distSQ = diff.getLengthSQ();
		if (distSQ < currentDistSQ)
		{
			monsterNode = node;
			currentDistSQ = distSQ;
		}
	}

	return monsterNode;
}

void Nightork::CCollisionManager::SetMonsters(std::vector<CMonster*>* monsters_)
{
	MonsterSelector->removeAllTriangleSelectors();
	Monsters = monsters_;

	// add monster triangle selectors
	for (unsigned int i=0; i<monsters_->size(); ++i)
	{
		irr::scene::ITriangleSelector* ts = (*monsters_)[i]->GetMonsterNode()->getTriangleSelector();
		MonsterSelector->addTriangleSelector(ts);
	}
}


irr::core::vector3df Nightork::CCollisionManager::GetPlayerPosition(const irr::core::vector3df &ellipsoidPosition_,
	const irr::core::vector3df& ellipsoidRadius_, const irr::core::vector3df& ellipsoidDirectionAndSpeed_ )
{
	irr::core::triangle3df tmpTri;
	irr::core::vector3df tmpVec;
	irr::scene::ISceneNode* outNode = NULL;
	bool tmpBool = false;

	return SceneCollisionManager->getCollisionResultPosition(
		CaveSelector,
		ellipsoidPosition_,
		ellipsoidRadius_,
		ellipsoidDirectionAndSpeed_,
		tmpTri,
		tmpVec,
		tmpBool,
		outNode
		);
}

bool Nightork::CCollisionManager::PlayerMonsterCollision(const irr::core::vector3df &ellipsoidPosition_,
	const irr::core::vector3df& ellipsoidRadius_, const irr::core::vector3df& ellipsoidDirectionAndSpeed_)
{
	if (!Monsters)	// initialized already?
		return false;

	irr::core::triangle3df tmpTri;
	irr::core::vector3df tmpVec;
	irr::scene::ISceneNode* outNode = NULL;
	bool tmpBool = false;

	// test for collision
	irr::core::vector3df position = SceneCollisionManager->getCollisionResultPosition(
		MonsterSelector,
		ellipsoidPosition_,
		ellipsoidRadius_,
		ellipsoidDirectionAndSpeed_, // values of zero do not result in a collision
		tmpTri,
		tmpVec,
		tmpBool,
		outNode,
		0.0f
		);

	if (outNode)
	{
		// find real outNode (outNode is not correct currently because of a bug in Irrlicht)
		outNode = FindNearestMonsterNode(position);
		MonsterManager->Collision(outNode);
		std::cout << "[CollisionManager:] Proximity alert: Player <-> " << outNode->getName() << " Type " << outNode->getID() << std::endl;
		return true;
	}

	return false;
}

bool Nightork::CCollisionManager::GetCollisionPointWithCave(const irr::core::line3df& ray_, irr::core::vector3df& outCollisionPoint_)
{
	if (DunGenProxy->TestForVoxelCollision(ray_, 1))
	{
		// test collision in mesh space
		irr::core::triangle3df tmpTri;
		irr::scene::ISceneNode* tmpNode;
		return SceneCollisionManager->getCollisionPoint(ray_,CaveSelector,outCollisionPoint_,tmpTri,tmpNode);
	}
	return false;
}

bool Nightork::CCollisionManager::GetCollisionPointWithPlayer(const irr::core::line3df& ray_, irr::core::vector3df& outCollisionPoint_)
{
	irr::core::triangle3df tmpTri;
	irr::scene::ISceneNode* tmpNode;
	return SceneCollisionManager->getCollisionPoint(ray_,PlayerShipSelector,outCollisionPoint_,tmpTri,tmpNode);
}

bool Nightork::CCollisionManager::GetCollisionPointWithPortal(const irr::core::line3df& ray_, irr::core::vector3df& outCollisionPoint_)
{
	irr::core::triangle3df tmpTri;
	irr::scene::ISceneNode* tmpNode;
	return SceneCollisionManager->getCollisionPoint(ray_,PortalSelector,outCollisionPoint_,tmpTri,tmpNode);
}

bool Nightork::CCollisionManager::GetCollisionPointWithMonster(const irr::core::line3df& ray_, irr::core::vector3df& outCollisionPoint_, irr::scene::ISceneNode*& outNode_)
{
	irr::core::triangle3df tmpTri;
	bool result = SceneCollisionManager->getCollisionPoint(ray_,MonsterSelector,outCollisionPoint_,tmpTri,outNode_);
	if (outNode_)
		outNode_ = FindNearestMonsterNode(outCollisionPoint_);
	return result;
}
