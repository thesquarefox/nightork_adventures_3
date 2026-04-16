// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef PLAYER_H
#define PLAYER_H

#include <irrlicht.h>
#include <time.h>
#include "Color.h"

namespace Nightork
{

class CCollisionManager;
class CEventReceiver;
class CGameplayStats;
class CPilot;
class CPowerUpManager;
class CProjectileManager;
class CSoundEffects;

class CPlayer
{
public:
	/// Constructor.
	CPlayer(irr::IrrlichtDevice* irrDevice_, CSoundEffects* soundEffects_, CEventReceiver* eventReceiver_,
		CCollisionManager* collisionManager_, CPowerUpManager* powerupManager_, CProjectileManager* projectileManager_,
		CGameplayStats* gameplayStats_,	const CPilot* pilot_);

	/// Destructor.
	~CPlayer();

	/// Updates the player ship and camera.
	void Update(clock_t actTime_);

	// Sets the 3D position and the monster, the crosshair is aiming to
	void SetAimingPosition(const irr::core::vector3df& pos_, CMonster* aimedMonster_);

	/// Returns the current hit points.
	unsigned int GetHpCurrent() const;

	/// Returns the maximum hit points.
	unsigned int GetHpMaximum() const;

	/// Returns the current after burner charge.
	unsigned int GetAfterburnerCurrent() const;

	/// Returns the maximum after burner charge.
	unsigned int GetAfterburnerMaximum() const;

	/// Returns the current energy charge.
	unsigned int GetEnergyCurrent() const;

	/// Returns the maximum energy charge.
	unsigned int GetEnergyMaximum() const;

	/// Is the power up currently active?
	bool IsPowerUpActive() const;

	/// Return the start tick of the power up.
	clock_t GetPowerUpStartTick() const;

	/// Drops life of the monster.
	void DropLife(unsigned int hp_, bool projectile_);

	/// Get the world position where the projectiles start.
	irr::core::vector3df GetAbsoluteShootPosition() const;

	/// Get the absolute world position of the player ship.
	irr::core::vector3df GetAbsoluteShipPosition() const;

	/// Returns the player camera.
	irr::scene::ICameraSceneNode* GetCamera() const;

private:
	CSoundEffects* SoundEffects;			///< The sound effects.
	CEventReceiver* EventReceiver;			///< The event receiver.
	CCollisionManager* CollisionManager;	///< The collision manager.
	CPowerUpManager* PowerUpManager;		///< The powerup manager.
	CProjectileManager* ProjectileManager;	///< The projectile manager.
	CGameplayStats* GameplayStats;			///< The gameplay statistics.
	const CPilot* Pilot;					///< The access to the pilot settings.

	irr::scene::ICameraSceneNode* Camera;	///< The camera behind the player ship.
	irr::scene::IMeshSceneNode* PlayerShip;	///< The player ship scene node.

	irr::core::vector3df Front;				///< Local coordinate system of player ship: front vector.
	irr::core::vector3df Left;				///< Local coordinate system of player ship: left vector.
	irr::core::vector3df Up;				///< Local coordinate system of player ship: up vector.

	irr::core::vector3df BoundingEllipsoid;	///< The bounding ellipsoid of the player ship.

	irr::core::vector3df AimingPosition;	///< The 3D position, the crosshair is aiming to.
	CMonster* AimedMonster;					///< The monster that is in aim lock.

	irr::scene::IParticleEmitter* PSEngine[2];///< The particle system emitters for the engine.
	irr::scene::IParticleEmitter* PSAfterburner[2];///< The particle system emitters for the afterburner.
	irr::scene::IParticleEmitter* PSDestruction;///< The particle system emitters for player explosion.

	clock_t LastTime;						///< The time of the previous frame.
	clock_t LastAfterburnerTick;			///< The time of the last afterburner tick.
	clock_t LastEnergyTick;					///< The time of the last energy tick.
	clock_t LastHpTick;						///< The time of the last hp tick.
	clock_t LastShotTick;					///< The time of the last shot.
	clock_t PowerUpStartTick;				///< The time the power up was activated.
	bool PowerUpActive;						///< Is the power up active currently?
	bool AfterburnerOnlineLastFrame;		///< Was the afterburner activated last frame?

	irr::core::vector3df RespawnPos;		///< The respawn position.
	irr::core::vector3df RespawnFront;		///< Local coordinate system of player ship: respawn front vector.
	irr::core::vector3df RespawnLeft;		///< Local coordinate system of player ship: respawn left vector.
	irr::core::vector3df RespawnUp;			///< Local coordinate system of player ship: respawn up vector.
	float Scale;							///< The current scale factor.
	float BaseScale;						///< The basic scale factor.
	unsigned int HpCurrent;					///< The current hit points.
	unsigned int AfterburnerCurrent;		///< The current afterburner charge.
	unsigned int EnergyCurrent;				///< The current energy.

	irr::core::vector3df CameraCurrentPosition; ///< The current position of the camera.
	irr::core::vector3df CameraCurrentTarget;   ///< The current target of the camera.
	irr::core::vector3df CameraCurrentUp;		///< The current up of the camera.
	float CurrentSpeed;							///< The current speed of the player.
	float CurrentRollSpeed;						///< The current roll speed of the player.
	float CurrentDeltaYaw;						///< The current yaw delta of the player.
	float CurrentDeltaPitch;					///< The current pitch delta of the player.
};

}

#endif
