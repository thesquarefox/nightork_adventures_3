// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef MONSTER_H
#define MONSTER_H

#include <irrlicht.h>
#include <time.h>
#include <vector>
#include "Color.h"
#include "MonsterTypes.h"
#include "Shield.h"

namespace Nightork
{

class CDunGenProxy;
class CProjectileManager;
class CSoundEffects;

class CMonster
{
public:
	/// Constructor.
	CMonster(irr::scene::IMeshSceneNode* monster_, irr::IrrlichtDevice* irrDevice_, CSoundEffects* soundEffects_, CProjectileManager* projectileManager_, CDunGenProxy* dunGen_);

	/// Destructor.
	~CMonster();

	/// Initalize the patroling values.
	void InitializePatroling(const std::vector<irr::core::vector3df>& monsterPositions_, const std::vector<irr::core::vector3df>& bossPositions_, irr::core::vector3df playerPosition_);

	/// Updates the monster.
	void Update(const irr::core::vector3df& playerShipPosition_, clock_t time);

	/// Is the monster destroyed?
	bool IsDestroyed() const;

	/// Is the monster destroyed by overload?
	bool IsDestroyedByOverload() const;

	/// Is the monster destroyed by power upped?
	bool IsDestroyedByPowerUpped() const;

	/// Returns the monster node.
	irr::scene::IMeshSceneNode* GetMonsterNode() const;

	/// Returns the type of the monster.
	EMonsterType::Enum GetMonsterType() const;

	/// Returns the current hit points.
	unsigned int GetHitpointsCurrent(EShield::Enum color_) const;

	/// Returns the maximum hit points.
	unsigned int GetHitpointsMaximum(EShield::Enum color_) const;

	/// Returns the shield color
	EColor::Enum GetHitpointsColor(EShield::Enum color_) const;

	/// Drops life of the monster.
	void DropLife(unsigned int color1_, unsigned int color2_, bool overload_, bool powerUpped_);

	/// Monster has collided with player ship object.
	void Collision();

private:
	void DropLifeByShield(unsigned int color1_, unsigned int color2_, EShield::Enum shield_);

private:
	CSoundEffects* SoundEffects;			///< The sound effects.
	irr::scene::ISceneManager* SceneManager;///< The scene manager.
	irr::video::IVideoDriver* VideoDriver;	///< The video driver.
	CProjectileManager* ProjectileManager;	///< The projectile manager.
	CDunGenProxy* DunGenProxy;				///< The access to the dungen settings.

	irr::scene::IMeshSceneNode* Monster;	///< The monster scene node.
	EMonsterType::Enum MonsterType;			///< The type of the monster.

	unsigned int HpCurrent[EShield::NUMBER];///< The current hit points.
	unsigned int HpMaximum[EShield::NUMBER];///< The maximum hit points.
	EColor::Enum HpColor[EShield::NUMBER];	///< The hp color.
	bool OverloadDeath;						///< Death by overload?
	bool PowerUpDeath;						///< Death by power up?

	irr::core::vector3df PatrolCenter;		///< The center of the patroling sphere.
	float PatrolRadius;						///< The radius of the patroling sphere.
	irr::core::vector3df PatrolTarget;		///< Target for patroling.
	float CurrentSpeed;						///< The current speed of the monster.
	float CurrentMaxSpeed;					///< The current maximum speed of the monster.

	float Scale;							///< The current scale factor.
	float BaseScale;						///< The basic scale factor.
	irr::scene::IParticleSystemSceneNode* Particle; ///< Particles for destruction.
	clock_t LastExplosionUpdateTick;		///< The time of the last explosion update tick.
	clock_t LastFireTick;					///< The time where the last projectile was fired.
	clock_t LastTime;						///< The time of the previous frame.
};
}

#endif
