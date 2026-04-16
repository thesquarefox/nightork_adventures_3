// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef POWERUPMANAGER_H
#define POWERUPMANAGER_H

#include <irrlicht.h>
#include <vector>

class CMonster;
class CPlayer;

namespace Nightork
{
class CPowerUpManager
{
public:
	struct SPowerUp
	{
		irr::scene::IMeshSceneNode* BaseNode;
		irr::scene::ISceneNode* MorphSceneNode;
		int Lifetime;
	};

public:
	/// Constructor.
	CPowerUpManager(irr::IrrlichtDevice* irrDevice_);

	/// Destructor.
	~CPowerUpManager();

	/// Updates all power ups.
	void Update(clock_t actTime_);

	/// Create new power up.
	void CreatePowerUp(const CMonster* monster_);

	/// Checks if power up is consumed by player.
	bool CheckForPowerUpPickup(const CPlayer* player_);

private:
	irr::scene::ISceneManager* SceneManager;///< The scene manager.
	irr::video::IVideoDriver* VideoDriver;	///< The video driver.
	std::vector<SPowerUp> PowerUps;			///< The active powerups.
	clock_t LastUpdateTick;					///< The time of the last update tick.
};
}

#endif
