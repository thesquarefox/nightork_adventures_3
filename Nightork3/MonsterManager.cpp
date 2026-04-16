// Copyright (C) 2011-2026 by Maximilian Hönig

#include <algorithm>
#include <iostream>
#include "SoundEffects.h"
#include "CollisionManager.h"
#include "Constants.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "PowerUpManager.h"
#include "ProjectilManager.h"

Nightork::CMonsterManager::CMonsterManager(irr::IrrlichtDevice* irrDevice_, CSoundEffects* soundEffects_, CCollisionManager* collisionManager_,
	CPowerUpManager* powerupManager_, CProjectileManager* projectileManager_, CDunGenProxy* dunGen_)
	: SoundEffects(soundEffects_)
	, CollisionManager(collisionManager_)
	, PowerUpManager(powerupManager_)
	, ProjectileManager(projectileManager_)
{
	// search for player ship
	PlayerShip = irrDevice_->getSceneManager()->getSceneNodeFromName("PlayerShip",irrDevice_->getSceneManager()->getRootSceneNode());

	std::vector<irr::core::vector3df> monsterPositions;
	std::vector<irr::core::vector3df> bossPositions;

	// create monsters and boss monsters
	irr::core::list<irr::scene::ISceneNode*> const & childNode = irrDevice_->getSceneManager()->getSceneNodeFromName("DungeonRoot")->getChildren();
	for (irr::core::list<irr::scene::ISceneNode*>::ConstIterator child(childNode.begin()); child != childNode.end(); ++child)
	{
		if (!strcmp("Ship",(*child)->getName()) || !strcmp("BossShip",(*child)->getName()))
		{
			CMonster* monster = new CMonster(static_cast<irr::scene::IMeshSceneNode*>(*child), irrDevice_, soundEffects_, projectileManager_, dunGen_);
			Monsters.push_back(monster);

			if (!strcmp("Ship", (*child)->getName()))
				monsterPositions.push_back((*child)->getAbsolutePosition());
			else
				bossPositions.push_back((*child)->getAbsolutePosition());
		}
	}

	for (unsigned int i = 0; i < Monsters.size(); ++i)
		Monsters[i]->InitializePatroling(monsterPositions, bossPositions, PlayerShip->getAbsolutePosition());

	// pass objects for collision
	CollisionManager->SetMonsters(&Monsters);

	std::cout << "[MonsterManager:] created monsters = " << Monsters.size() << std::endl;
}

Nightork::CMonsterManager::~CMonsterManager()
{
	for (unsigned int i=0; i<Monsters.size(); ++i)
		delete Monsters[i];
}

void Nightork::CMonsterManager::Update(clock_t actTime_)
{
	const irr::core::vector3df playerShipPosition = PlayerShip->getAbsolutePosition();

	// update all monsters
	for (unsigned int i = 0; i < Monsters.size(); ++i)
	{
		Monsters[i]->Update(playerShipPosition, actTime_);

		// normally destroyed non-boss monsters receive special handling
		if (Monsters[i]->IsDestroyed() && !Monsters[i]->IsDestroyedByOverload()
			&& Monsters[i]->GetMonsterType() < EMonsterType::BOSS_01)
		{
			// only power ups cause critical destrcutions
			if (Monsters[i]->IsDestroyedByPowerUpped())
			{
				std::vector<std::pair<CMonster*, float> > nearestMonsters;
				const irr::core::vector3df pos = Monsters[i]->GetMonsterNode()->getAbsolutePosition();

				for (unsigned int j = 0; j < Monsters.size(); ++j)
					if (Monsters[j]->GetMonsterType() < EMonsterType::BOSS_01 && !Monsters[j]->IsDestroyed())
						nearestMonsters.push_back(
							std::make_pair(
								Monsters[j],
								(Monsters[j]->GetMonsterNode()->getAbsolutePosition() - pos).getLengthSQ())
						);

				std::sort(nearestMonsters.begin(), nearestMonsters.end(),
					[](const std::pair<CMonster*, float>& a, const std::pair<CMonster*, float>& b) {
						return a.second < b.second;
					});

				for (unsigned int j = 0; j < nearestMonsters.size() && j < Constants::CriticalDestruction::NumberOfParts; ++j)
					ProjectileManager->CreateProjectilePlayer(
						pos,
						(nearestMonsters[j].first->GetMonsterNode()->getAbsolutePosition() - pos).normalize(),
						nearestMonsters[j].first,
						EColor::OVERLOAD,
						false
					);

				SoundEffects->PlayMonsterCriticalDestruction(irrklang::vec3df(pos.X, pos.Y, pos.Z));

				CriticalDestructions.push_back(std::make_pair(pos, actTime_));
			}
			else
			{
				// if not destroyed by power up, then create one for new usage
				PowerUpManager->CreatePowerUp(Monsters[i]);
			}
		}
	}

	// invalidate critical destructions
	for (unsigned int i = 0; i < CriticalDestructions.size(); ++i)
		if (actTime_ - CriticalDestructions[i].second > Constants::CriticalDestruction::DurationOfText)
			CriticalDestructions[i].second = 0;
}

namespace
{
	bool MonsterCleanup(Nightork::CMonster* const monster_)
	{
		const bool result = monster_->IsDestroyed();
		if (result)
			delete monster_;
		return result;
	}

	bool CriticalDestructionCleanup(const std::pair<irr::core::vector3df, clock_t>& criticalDestruction_)
	{
		return criticalDestruction_.second == 0;
	}
}

void Nightork::CMonsterManager::Cleanup()
{
	// delete monsters, if necessary
	const unsigned int numMonsters = static_cast<unsigned int>(Monsters.size());

	Monsters.erase(std::remove_if(std::begin(Monsters), std::end(Monsters), MonsterCleanup), std::end(Monsters));
	if (Monsters.size() != numMonsters)
		CollisionManager->SetMonsters(&Monsters);

	CriticalDestructions.erase(std::remove_if(std::begin(CriticalDestructions), std::end(CriticalDestructions), CriticalDestructionCleanup), std::end(CriticalDestructions));
}

Nightork::CMonster* Nightork::CMonsterManager::GetMonster(irr::scene::ISceneNode* node_)
{
	for (unsigned int i=0; i<Monsters.size(); ++i)
		if (Monsters[i]->GetMonsterNode() == node_ && !Monsters[i]->IsDestroyed())
			return Monsters[i];
	return NULL;
}

const std::vector<Nightork::CMonster*>* Nightork::CMonsterManager::GetMonsters() const
{
	return &Monsters;
}

void Nightork::CMonsterManager::DropLife(irr::scene::ISceneNode* node_, unsigned int color1_, unsigned int color2_, bool overload_, bool powerUpped_)
{
	for (unsigned int i=0; i<Monsters.size(); ++i)
		if (Monsters[i]->GetMonsterNode() == node_)
		{
			Monsters[i]->DropLife(color1_, color2_, overload_, powerUpped_);
			return;
		}
}

void Nightork::CMonsterManager::Collision(irr::scene::ISceneNode* node_)
{
	for (unsigned int i=0; i<Monsters.size(); ++i)
		if (Monsters[i]->GetMonsterNode() == node_)
		{
			Monsters[i]->Collision();
			return;
		}
}

float Nightork::CMonsterManager::GetNearestDistancePlayerShipToMonster() const
{
	float currentDistanceSQ = 9999.0f*9999.0f;
	const irr::core::vector3df playerShipPosition = PlayerShip->getAbsolutePosition();
	for (unsigned int i=0; i<Monsters.size(); ++i)
	{
		const float distanceSQ = (Monsters[i]->GetMonsterNode()->getAbsolutePosition()-PlayerShip->getAbsolutePosition()).getLengthSQ();
		if (distanceSQ < currentDistanceSQ)
			currentDistanceSQ = distanceSQ;
	}
	return sqrt(currentDistanceSQ);
}

unsigned int Nightork::CMonsterManager::GetNumberMonsters() const
{
	return static_cast<unsigned int>(Monsters.size());
}

const std::vector<std::pair<irr::core::vector3df, clock_t> >* Nightork::CMonsterManager::GetCriticalDestructions() const
{
	return &CriticalDestructions;
}
