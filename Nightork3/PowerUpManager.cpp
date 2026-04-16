// Copyright (C) 2011-2026 by Maximilian Hönig

#include <algorithm>
#include <ObjectPlacementCommon.h>
#include "Constants.h"
#include "Monster.h"
#include "Player.h"
#include "PowerUpManager.h"

Nightork::CPowerUpManager::CPowerUpManager(irr::IrrlichtDevice* irrDevice_)
	: SceneManager(irrDevice_->getSceneManager())
	, VideoDriver(irrDevice_->getVideoDriver())
	, PowerUps()
	, LastUpdateTick(0)
{
}

Nightork::CPowerUpManager::~CPowerUpManager()
{
}

namespace
{
	bool ExpirePU(const Nightork::CPowerUpManager::SPowerUp& pu_)
	{
		if (Nightork::Constants::PowerUp::DestroyLifetime <= pu_.Lifetime)
		{
			pu_.MorphSceneNode->remove();
			pu_.BaseNode->remove();
			return true;
		}
		return false;
	}

}

void Nightork::CPowerUpManager::Update(clock_t actTime_)
{
	const int ticked = actTime_ - LastUpdateTick;
	LastUpdateTick = actTime_;

	for (unsigned int i = 0; i < PowerUps.size(); ++i)
	{
		PowerUps[i].Lifetime += ticked;
		if (PowerUps[i].Lifetime >= Constants::PowerUp::Lifetime)
		{
			if (PowerUps[i].Lifetime >= Constants::PowerUp::DestroyLifetime)
				PowerUps[i].Lifetime = Constants::PowerUp::DestroyLifetime;

			const float scale = 1.0f - static_cast<float>(PowerUps[i].Lifetime - Constants::PowerUp::Lifetime) / static_cast<float>(Constants::PowerUp::DestructionTime);
			PowerUps[i].MorphSceneNode->setScale(irr::core::vector3df(scale, scale, scale));
		}
	}

	// delete all expired power ups
	PowerUps.erase(std::remove_if(std::begin(PowerUps), std::end(PowerUps), ExpirePU), std::end(PowerUps));
}

void Nightork::CPowerUpManager::CreatePowerUp(const CMonster* monster_)
{
	SPowerUp newPowerUp;
	newPowerUp.Lifetime = 0;

	newPowerUp.BaseNode = SceneManager->addCubeSceneNode(
		Constants::PowerUp::Size,
		0, -1,
		monster_->GetMonsterNode()->getPosition()
	);

	irr::video::ITexture* powerUpTexture = VideoDriver->getTexture("data/power_up.png");
	newPowerUp.BaseNode->setName("PowerUp");
	newPowerUp.BaseNode->setMaterialTexture(0, powerUpTexture);
	newPowerUp.BaseNode->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
	newPowerUp.BaseNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	newPowerUp.BaseNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
	newPowerUp.BaseNode->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
	newPowerUp.BaseNode->setVisible(false);

	newPowerUp.MorphSceneNode = SceneManager->addWaterSurfaceSceneNode(
		newPowerUp.BaseNode->getMesh(),			// mesh
		0.3f,									// wave height
		500.0f,									// wave speed
		0.3f,									// wave length
		0,										// parent node
		-1,										// node id
		newPowerUp.BaseNode->getPosition(),		// position
		newPowerUp.BaseNode->getRotation(),		// rotation
		newPowerUp.BaseNode->getScale()			// scale
	);
	newPowerUp.MorphSceneNode->setName("PowerUp");

	PowerUps.push_back(newPowerUp);
}

bool Nightork::CPowerUpManager::CheckForPowerUpPickup(const CPlayer* player_)
{
	const float distanceSquared = Constants::PowerUp::PickUpRadius * Constants::PowerUp::PickUpRadius;

	for (unsigned int i = 0; i < PowerUps.size(); ++i)
	{
		if (PowerUps[i].Lifetime < Constants::PowerUp::Lifetime
			&& (PowerUps[i].BaseNode->getAbsolutePosition() - player_->GetAbsoluteShipPosition())
			.getLengthSQ() < distanceSquared)
		{
			PowerUps[i].Lifetime = Constants::PowerUp::Lifetime;
			return true;
		}
	}
	return false;
}
