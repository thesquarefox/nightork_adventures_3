// Copyright (C) 2011-2026 by Maximilian Hönig

#include <DunGen.h>
#include <cmath>
#include <limits>
#include "CollisionManager.h"
#include "Constants.h"
#include "EventReceiver.h"
#include "GameplayStats.h"
#include "Pilot.h"
#include "Player.h"
#include "PowerUpManager.h"
#include "ProjectilManager.h"
#include "SoundEffects.h"

namespace 
{
	float clamp(float value, float min, float max)
	{
		return std::max(std::min(value, max), min);
	}
}

Nightork::CPlayer::CPlayer(irr::IrrlichtDevice* irrDevice_, CSoundEffects* soundEffects_,
	CEventReceiver* eventReceiver_, CCollisionManager* collisionManager_, CPowerUpManager* powerupManager_,
	CProjectileManager* projectileManager_, CGameplayStats* gameplayStats_, const CPilot* pilot_)
	: SoundEffects(soundEffects_)
	, EventReceiver(eventReceiver_)
	, CollisionManager(collisionManager_)
	, PowerUpManager(powerupManager_)
	, ProjectileManager(projectileManager_)
	, GameplayStats(gameplayStats_)
	, Pilot(pilot_)
	, AimingPosition(0.0, 0.0f, 0.0f)
	, AimedMonster(NULL)
	, LastTime(0)
	, LastAfterburnerTick(0)
	, LastEnergyTick(0)
	, LastHpTick(0)
	, LastShotTick(0)
	, PowerUpStartTick(0)
	, Scale(1.0f)
	, PowerUpActive(false)
	, AfterburnerOnlineLastFrame(false)
	, HpCurrent(Constants::Player::HitpointMaximum[Pilot->GetTotemType()])
	, AfterburnerCurrent(Constants::Player::Afterburner::Maximum[Pilot->GetTotemType()])
	, EnergyCurrent(Constants::Player::Projectile::Maximum[Pilot->GetTotemType()])
	, CameraCurrentPosition(irr::core::vector3df(0.0f, 0.0f, 0.0f))
	, CameraCurrentTarget(irr::core::vector3df(0.0f, 0.0f, 0.0f))
	, CameraCurrentUp(irr::core::vector3df(0.0f, 1.0f, 0.0f))
	, CurrentSpeed(0.0f)
	, CurrentRollSpeed(0.0f)
	, CurrentDeltaYaw(0.0f)
	, CurrentDeltaPitch(0.0f)
{
	// create camera
	Camera = irrDevice_->getSceneManager()->addCameraSceneNode();
	Camera->setNearValue(Constants::Player::Camera::NearValue);
	Camera->setFarValue(Constants::Player::Camera::FarValue);
	Camera->setPosition(irr::core::vector3df(0,0,0));
	Camera->setTarget(irr::core::vector3df(99999.f, 99999.f, 99999.f));

	// create light
	irr::scene::ILightSceneNode* light = irrDevice_->getSceneManager()->addLightSceneNode(Camera);
	irr::video::SLight lightdata;
	lightdata.AmbientColor = irr::video::SColorf(0.0f,0.0f,0.0f);
	lightdata.DiffuseColor = irr::video::SColorf(1.0f,1.0f,1.0f);
	lightdata.SpecularColor = irr::video::SColorf(0.0f,0.0f,0.0f);
	lightdata.Radius = Constants::Player::LightRadius;
	lightdata.CastShadows = false;
	light->setLightData(lightdata);

	// search for player ship
	PlayerShip = static_cast<irr::scene::IMeshSceneNode*>(irrDevice_->getSceneManager()->getSceneNodeFromName("PlayerShip"));
	RespawnPos = PlayerShip->getAbsolutePosition();
	BaseScale = PlayerShip->getScale().X; // equal scale for all axis

	// computing the bounding ellipsoid
	irr::core::aabbox3df box = PlayerShip->getMesh()->getBoundingBox();
	irr::core::vector3df extend = 0.5f * box.getExtent(); // half extend = some sort of radius
	irr::core::vector3df center = box.getCenter();
	extend += irr::core::vector3df(abs(center.X),abs(center.Y),abs(center.Z)); // add center offset
	irr::core::vector3df scale = PlayerShip->getScale();
	float maxExtend = std::max(extend.X*scale.X,std::max(extend.Y*scale.Y,extend.Z*scale.Z)); // normally we would need to multiply with sqrt(3), but the player ship is not cube formed - more cylindrical
	BoundingEllipsoid = irr::core::vector3df(maxExtend,maxExtend,maxExtend);

	// set local coordinate system
	irr::core::matrix4 m = PlayerShip->getAbsoluteTransformation();
	Front = irr::core::vector3df(m[8], m[9], m[10]);
	//Left = irr::core::vector3df(m[0], m[1], m[2]);
	Up = irr::core::vector3df(m[4], m[5], m[6]);
	Front.normalize();
	Left = Front.crossProduct(Up);
	Left.normalize();
	Up = Left.crossProduct(Front);
	Up.normalize();

	RespawnFront = Front;
	RespawnLeft = Left;
	RespawnUp = Up;

	// add particle effects
	for (unsigned int i=0; i<2; ++i)
	{
		// engine
		irr::core::vector3df posEngine = Constants::Player::AfterburnerParticle::Position;
		posEngine.X = (0==i) ? -posEngine.X : posEngine.X;
		irr::scene::IParticleSystemSceneNode* psEngine = irrDevice_->getSceneManager()->addParticleSystemSceneNode(
			false, PlayerShip, -1,
			posEngine,
			irr::core::vector3df(0.0f,0.0f,0.0f),
			irr::core::vector3df(1.0f,1.0f,1.0f));

		PSEngine[i] = psEngine->createPointEmitter(
			Front,											// direction
			0,0,											// emit rate
			Constants::Player::EngineParticle::Color,	// darkest color
			Constants::Player::EngineParticle::Color,	// brightest color
			Constants::Player::EngineParticle::AgeMin,	// min age,
			Constants::Player::EngineParticle::AgeMax,	// max age,
			Constants::Player::EngineParticle::Angle,	// angle
			irr::core::dimension2df(Constants::Player::EngineParticle::Size,Constants::Player::EngineParticle::Size),// min size
			irr::core::dimension2df(Constants::Player::EngineParticle::Size,Constants::Player::EngineParticle::Size) // max size
			);
		psEngine->setEmitter(PSEngine[i]);
		psEngine->setParticlesAreGlobal(false);
		PSEngine[i]->drop();

		irr::scene::IParticleAffector* pafEngine = psEngine->createFadeOutParticleAffector();
		psEngine->addAffector(pafEngine);
		pafEngine->drop();

		psEngine->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		psEngine->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
		psEngine->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
		psEngine->setMaterialTexture(0, irrDevice_->getVideoDriver()->getTexture("data/particle_white.png"));
		psEngine->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);

		// afterburner
		irr::core::vector3df posAfterburner = Constants::Player::AfterburnerParticle::Position;
		posAfterburner.X = (0 == i) ? -posAfterburner.X : posAfterburner.X;
		irr::scene::IParticleSystemSceneNode* psAfterburner = irrDevice_->getSceneManager()->addParticleSystemSceneNode(
			false, PlayerShip, -1,
			posAfterburner,
			irr::core::vector3df(0.0f, 0.0f, 0.0f),
			irr::core::vector3df(1.0f, 1.0f, 1.0f));

		PSAfterburner[i] = psAfterburner->createPointEmitter(
			Front,											// direction
			0, 0,											// emit rate
			Constants::Player::AfterburnerParticle::Color,	// darkest color
			Constants::Player::AfterburnerParticle::Color,	// brightest color
			Constants::Player::AfterburnerParticle::AgeMin,	// min age,
			Constants::Player::AfterburnerParticle::AgeMax,	// max age,
			Constants::Player::AfterburnerParticle::Angle,	// angle
			irr::core::dimension2df(Constants::Player::AfterburnerParticle::Size, Constants::Player::AfterburnerParticle::Size),// min size
			irr::core::dimension2df(Constants::Player::AfterburnerParticle::Size, Constants::Player::AfterburnerParticle::Size) // max size
		);
		psAfterburner->setEmitter(PSAfterburner[i]);
		psAfterburner->setParticlesAreGlobal(false);
		PSAfterburner[i]->drop();

		irr::scene::IParticleAffector* pafAfterburner = psAfterburner->createFadeOutParticleAffector();
		psAfterburner->addAffector(pafAfterburner);
		pafAfterburner->drop();

		psAfterburner->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		psAfterburner->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
		psAfterburner->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
		psAfterburner->setMaterialTexture(0, irrDevice_->getVideoDriver()->getTexture("data/particle_white.png"));
		psAfterburner->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
	}

	// add destruction particle effect
	irr::scene::IParticleSystemSceneNode* ps = irrDevice_->getSceneManager()->addParticleSystemSceneNode(
		false, PlayerShip, -1,
		irr::core::vector3df(0.0f,0.0f,0.0f),
		irr::core::vector3df(0.0f,0.0f,0.0f),
		irr::core::vector3df(1.0f,1.0f,1.0f));

	PSDestruction = ps->createPointEmitter(
		Constants::Player::Particle::Direction,	// direction
		0,0,									// emit rate
		Constants::Player::Particle::Color,		// darkest color
		Constants::Player::Particle::Color,		// brightest color
		Constants::Player::Particle::Age,		// min age
		Constants::Player::Particle::Age,		// max age
		Constants::Player::Particle::Angle,		// angle
		irr::core::dimension2df(Constants::Player::Particle::Size,Constants::Player::Particle::Size),// min size
		irr::core::dimension2df(Constants::Player::Particle::Size,Constants::Player::Particle::Size) // max size
		);
	ps->setEmitter(PSDestruction);
	ps->setParticlesAreGlobal(true);
	PSDestruction->drop();

	irr::scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf);
	paf->drop();

	ps->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	ps->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
	ps->setMaterialTexture(0, irrDevice_->getVideoDriver()->getTexture("data/particle_white.png"));
	ps->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
}

Nightork::CPlayer::~CPlayer()
{
}

void Nightork::CPlayer::Update(clock_t actTime_)
{
	const Constants::Player::ETotemType::Enum totemType = Pilot->GetTotemType();

	unsigned int timePassed = actTime_ - LastTime;
	LastTime = actTime_;

	// in destruction?
	if (0 == HpCurrent)
	{
		Scale -= timePassed*Constants::Player::ScaleDownExplodePerMs;

		if (0.0f >= Scale)
		{
			Scale = 0.0f;
			PSDestruction->setMinParticlesPerSecond(0);
			PSDestruction->setMaxParticlesPerSecond(0);

			PlayerShip->setPosition(RespawnPos);
			Front = RespawnFront;
			Left = RespawnLeft;
			Up = RespawnUp;

			HpCurrent = Constants::Player::HitpointMaximum[totemType];
			AfterburnerCurrent = Constants::Player::Afterburner::Maximum[totemType];
			EnergyCurrent = Constants::Player::Projectile::Maximum[totemType];
			PowerUpActive = false;

			CurrentSpeed = 0.0f;
			CurrentRollSpeed = 0.0f;
			CurrentDeltaYaw = 0.0f;
			CurrentDeltaPitch = 0.0f;

			// delete weapon maneuvers that occurred during respawn
			EventReceiver->IsManeuver(CPilot::SControls::Weapon1);
			EventReceiver->IsManeuver(CPilot::SControls::Weapon2);

			// reset follow camera
			CameraCurrentPosition = irr::core::vector3df(0.0f, 0.0f, 0.0f);
		}

		const float resScale = Scale*BaseScale;
		PlayerShip->setScale(irr::core::vector3df(resScale,resScale,resScale));

		return;
	}
	// in respawn?
	else if (1.0f > Scale)
	{
		Scale += timePassed*Constants::Player::ScaleDownExplodePerMs;
		if (1.0f < Scale)
			Scale = 1.0f;
		const float resScale = Scale*BaseScale;
		PlayerShip->setScale(irr::core::vector3df(resScale,resScale,resScale));
	}

	// update hp
	if ((PlayerShip->getAbsolutePosition() - RespawnPos).getLengthSQ() <= DunGen::CollisionRadius::Portal*DunGen::CollisionRadius::Portal)
	{
		while (Constants::Player::TickHpRegenPortal < actTime_ - LastHpTick)
		{
			LastHpTick += Constants::Player::TickHpRegenPortal;
			if (Constants::Player::HitpointMaximum[totemType] > HpCurrent)
				++HpCurrent;
		}
	}
	else
	{
		while (Constants::Player::TickHpRegen[totemType] < static_cast<unsigned int>(actTime_ - LastHpTick))
		{
			LastHpTick += Constants::Player::TickHpRegen[totemType];
			if (Constants::Player::HitpointMaximum[totemType] > HpCurrent)
				++HpCurrent;
		}
	}

	// compute angles
	irr::core::vector3df angles = irr::core::vector3df(0.0f,0.0f,0.0f);

	// yaw and pitch smoothly
	const float mouseSens = Pilot->GetMouseSensitivity();
	const bool mouseInvertY = Pilot->GetMouseInvertY();

	const irr::core::position2df mouse = EventReceiver->GetRelativeMousePosition();
	const float targetDeltaYaw = -mouse.X * irr::core::HALF_PI * mouseSens;
	const float targetDeltaPitch = (mouseInvertY ? 1.0f : -1.0f) * mouse.Y * irr::core::HALF_PI * mouseSens;

	const float yawPitchValue = Constants::Player::Movement::AccelarationYawPitch * timePassed * timePassed;

	// yaw (left/right)
	if (CurrentDeltaYaw < targetDeltaYaw)
	{
		CurrentDeltaYaw += yawPitchValue;
		if (CurrentDeltaYaw > targetDeltaYaw)
			CurrentDeltaYaw = targetDeltaYaw;
	}
	else if (CurrentDeltaYaw > targetDeltaYaw)
	{
		CurrentDeltaYaw -= yawPitchValue;
		if (CurrentDeltaYaw < targetDeltaYaw)
			CurrentDeltaYaw = targetDeltaYaw;
	}

	// pitch (up/down)
	if (CurrentDeltaPitch < targetDeltaPitch)
	{
		CurrentDeltaPitch += yawPitchValue;
		if (CurrentDeltaPitch > targetDeltaPitch)
			CurrentDeltaPitch = targetDeltaPitch;
	}
	else if (CurrentDeltaPitch > targetDeltaPitch)
	{
		CurrentDeltaPitch -= yawPitchValue;
		if (CurrentDeltaPitch < targetDeltaPitch)
			CurrentDeltaPitch = targetDeltaPitch;
	}

	// set angle changes
	angles.Y += CurrentDeltaYaw;
	angles.X += CurrentDeltaPitch;

	const float accelerationRoll = Constants::Player::Movement::AccelarationRoll * timePassed;

	if (EventReceiver->IsManeuver(CPilot::SControls::Left))
	{
		if (CurrentRollSpeed > -Constants::Player::Movement::SpeedRoll)
		{
			CurrentRollSpeed -= accelerationRoll;
		}
		else
		{
			CurrentRollSpeed = -Constants::Player::Movement::SpeedRoll;
		}
	}
	else if (EventReceiver->IsManeuver(CPilot::SControls::Right))
	{
		if (CurrentRollSpeed < Constants::Player::Movement::SpeedRoll)
		{
			CurrentRollSpeed += accelerationRoll;
		}
		else
		{
			CurrentRollSpeed = Constants::Player::Movement::SpeedRoll;
		}
	}
	else
	{
		if (CurrentRollSpeed >= accelerationRoll)
		{
			CurrentRollSpeed -= accelerationRoll;
		}
		else if (CurrentRollSpeed <= -accelerationRoll)
		{
			CurrentRollSpeed += accelerationRoll;
		}
		else {
			CurrentRollSpeed = 0.0f;
		}
	}

	angles.Z += CurrentRollSpeed * timePassed;

	if (0.0f != angles.Y)
	{
		// compute sinus and cosinus values for the given angle
		const float cosAngleYaw = cos(angles.Y);	
		const float sinAngleYaw = sin(angles.Y);

		// compute new coordinate system vectors
		// normalize for numerical stability
		Front = cosAngleYaw * Front + sinAngleYaw * Left;
		Front.normalize();
		Left = Front.crossProduct(Up);
		Left.normalize();
	}
	if (0.0f != angles.X)
	{
		// compute sinus and cosinus values for the given angle
		const float cosAnglePitch = cos(angles.X);
		const float sinAnglePitch = sin(angles.X);

		// compute new coordinate system vectors
		// normalize for numerical stability
		Front = cosAnglePitch * Front + sinAnglePitch * Up;
		Front.normalize();	
		Up = Left.crossProduct(Front);
		Up.normalize();
	}
	if (0.0f != angles.Z)
	{
		// compute sinus and cosinus values for the given angle
		const float cosAngleRoll = cos(angles.Z);
		const float sinAngleRoll = sin(angles.Z);

		// compute new coordinate system vectors
		// normalize for numerical stability
		Left = cosAngleRoll * Left + sinAngleRoll * Up;
		Left.normalize();
		Up = Left.crossProduct(Front);
		Up.normalize();
	}

	if (irr::core::vector3df(0.0f,0.0f,0.0f) != angles || 1.0f > Scale)
	{
		// compute final angles
		angles.Z = atan2(-Left.Y,-Left.X)*irr::core::RADTODEG;
		const float tmp = atan2(Up.Z,Front.Z);
		angles.X = tmp*irr::core::RADTODEG;
		angles.Y = atan2(Left.Z,Front.Z/cos(tmp))*irr::core::RADTODEG;

		// set rotation
		PlayerShip->setRotation(angles);
	}

	// movement preparation with smooth acceleration/deceleration
	const float acceleration = Constants::Player::Movement::Accelaration * timePassed;
	const float deceleration = Constants::Player::Movement::Deceleration * timePassed;

	unsigned int particlePerSecEngine = 0;
	unsigned int particlePerSecAfterburner = 0;
	bool afterburnerActivated = false;

	if (EventReceiver->IsManeuver(CPilot::SControls::Forward))
	{
		if (EventReceiver->IsManeuver(CPilot::SControls::Afterburner)
			&& ((AfterburnerOnlineLastFrame && 0 < AfterburnerCurrent)
				|| (Constants::Player::Afterburner::ActivationMinimum < AfterburnerCurrent)))
		{
			particlePerSecAfterburner = Constants::Player::AfterburnerParticle::ParticlesPerSec;
			afterburnerActivated = true;
			if (CurrentSpeed < Constants::Player::Movement::SpeedForwardAfterburner - acceleration)
			{			
				CurrentSpeed += acceleration;
			}
			else
			{
				CurrentSpeed = Constants::Player::Movement::SpeedForwardAfterburner;
			}
		}
		else
		{
			particlePerSecEngine = Constants::Player::EngineParticle::ParticlesPerSec;
			if (CurrentSpeed < Constants::Player::Movement::SpeedForward - acceleration)
			{
				CurrentSpeed += acceleration;
			}
			else if (CurrentSpeed > Constants::Player::Movement::SpeedForward + deceleration)
			{
				CurrentSpeed -= deceleration;
			}
			else
			{
				CurrentSpeed = Constants::Player::Movement::SpeedForward;
			}
		}
	}
	else if (EventReceiver->IsManeuver(CPilot::SControls::Backward))
	{
		if (CurrentSpeed > Constants::Player::Movement::SpeedBackward + deceleration)
		{
			CurrentSpeed -= deceleration;
		}
		else
		{
			CurrentSpeed = Constants::Player::Movement::SpeedBackward;
		}
	}
	else
	{
		if (CurrentSpeed >= deceleration)
		{
			CurrentSpeed -= deceleration;
		}
		else if (CurrentSpeed <= -acceleration)
		{
			CurrentSpeed += acceleration;
		}
		else {
			CurrentSpeed = 0.0f;
		}
	}

	const float wayPassed = CurrentSpeed * timePassed;

	// sound
	SoundEffects->PlayPlayerAfterburner(afterburnerActivated);
	SoundEffects->PlayPlayerEngine(!afterburnerActivated && std::abs(CurrentSpeed) > 0.0f);

	// movement only if speed is not zero
	if (std::abs(CurrentSpeed) > 0.0f)
	{
		// monster collision
		if (CollisionManager->PlayerMonsterCollision(
			PlayerShip->getAbsolutePosition(),
			BoundingEllipsoid,
			Front * wayPassed))
			DropLife(std::numeric_limits<unsigned int>::max(), false);

		// movement
		irr::core::vector3df position = CollisionManager->GetPlayerPosition(
			PlayerShip->getAbsolutePosition(),
			BoundingEllipsoid,
			Front * wayPassed);

		PlayerShip->setPosition(position);

		// update absolute position
		PlayerShip->updateAbsolutePosition();
	}

	// set camera
	const irr::core::vector3df desiredCameraPos = PlayerShip->getAbsolutePosition()
		+ Constants::Player::Camera::PositionFront * Front
		+ Constants::Player::Camera::PositionUp * Up;
	const irr::core::vector3df desiredCameraTarget = PlayerShip->getAbsolutePosition()
		+ Constants::Player::Camera::ViewPointFront * Front;

	// Initialisierung beim ersten Aufruf
	if (CameraCurrentPosition == irr::core::vector3df(0.0f, 0.0f, 0.0f)) {
		CameraCurrentPosition = desiredCameraPos;
		CameraCurrentTarget = desiredCameraTarget;
		CameraCurrentUp = Up;
	}

	const double lerpPos = std::exp(-Constants::Player::Camera::CameraSpeedPos * timePassed);
	const double lerpTarget = std::exp(-Constants::Player::Camera::CameraTargetSpeed * timePassed);
	const double lerpUp = std::exp(-Constants::Player::Camera::CameraUpSpeed * timePassed);

	CameraCurrentPosition = CameraCurrentPosition.getInterpolated(desiredCameraPos, lerpPos);

	CameraCurrentTarget = CameraCurrentTarget.getInterpolated(desiredCameraTarget, lerpTarget);
	CameraCurrentUp = CameraCurrentUp.getInterpolated(Up, lerpUp);

	Camera->setPosition(CameraCurrentPosition);
	Camera->updateAbsolutePosition();
	Camera->setTarget(CameraCurrentTarget);
	Camera->setUpVector(CameraCurrentUp);

	// update engine and afterburner particle emitters
	for (unsigned int i=0; i<2; ++i)
	{
		PSEngine[i]->setDirection(Front * Constants::Player::EngineParticle::DirectionFactor);
		PSEngine[i]->setMinParticlesPerSecond(particlePerSecEngine);
		PSEngine[i]->setMaxParticlesPerSecond(particlePerSecEngine);

		PSAfterburner[i]->setDirection(Front*Constants::Player::AfterburnerParticle::DirectionFactor);
		PSAfterburner[i]->setMinParticlesPerSecond(particlePerSecAfterburner);
		PSAfterburner[i]->setMaxParticlesPerSecond(particlePerSecAfterburner);
	}

	// update afterburner charge
	if (afterburnerActivated)
	{
		if (!AfterburnerOnlineLastFrame)
		{
			AfterburnerOnlineLastFrame = true;
			if (0 < AfterburnerCurrent )
				--AfterburnerCurrent;
		}
		while (Constants::Player::Afterburner::TickDecharge < actTime_ - LastAfterburnerTick)
		{
			LastAfterburnerTick += Constants::Player::Afterburner::TickDecharge;
			if (0 < AfterburnerCurrent)
				--AfterburnerCurrent;
		}
	}
	else
	{
		AfterburnerOnlineLastFrame = false;
		while (Constants::Player::Afterburner::TickCharge[totemType] < static_cast<unsigned int>(actTime_ - LastAfterburnerTick))
		{
			LastAfterburnerTick += Constants::Player::Afterburner::TickCharge[totemType];
			if (Constants::Player::Afterburner::Maximum[totemType] > AfterburnerCurrent)
				++AfterburnerCurrent;
		}
	}

	// check for power ups
	if (PowerUpManager->CheckForPowerUpPickup(this))
	{
		SoundEffects->PlayPowerUpOn();
		PowerUpActive = true;
		PowerUpStartTick = actTime_;
	}
	else if (PowerUpActive)
	{
		if (actTime_ - PowerUpStartTick >= Constants::PowerUp::Duration)
		{
			SoundEffects->PlayPowerUpOff();
			PowerUpActive = false;
		}
	}

	// update energy and shoot projectiles
	while (Constants::Player::Projectile::TickCharge[totemType] < static_cast<unsigned int>(actTime_ - LastEnergyTick))
	{
		LastEnergyTick += Constants::Player::Projectile::TickCharge[totemType];
		if (Constants::Player::Projectile::Maximum[totemType] > EnergyCurrent)
			++EnergyCurrent;
	}

	if ((EventReceiver->IsManeuver(CPilot::SControls::Weapon1) || EventReceiver->IsManeuver(CPilot::SControls::Weapon2))
		&& Constants::Player::Projectile::Decharge <= EnergyCurrent
		&& Constants::Player::Projectile::ShotDelay <= actTime_ - LastShotTick)
	{
		const EColor::Enum energyColor = EventReceiver->IsManeuver(CPilot::SControls::Weapon1) ? EColor::MAGIC : EColor::ANTIMAGIC;

		irr::core::vector3df projPosition;
		PlayerShip->getAbsoluteTransformation().transformVect(projPosition,Constants::Player::Projectile::Position);
		ProjectileManager->CreateProjectilePlayer(projPosition,
			(AimingPosition-projPosition).normalize(), AimedMonster, energyColor, PowerUpActive);

		irr::core::vector3df tmp = Constants::Player::Projectile::Position;
		tmp.X = -tmp.X;
		PlayerShip->getAbsoluteTransformation().transformVect(projPosition,tmp);
		ProjectileManager->CreateProjectilePlayer(projPosition,
			(AimingPosition-projPosition).normalize(), AimedMonster, energyColor, PowerUpActive);
		
		EnergyCurrent -= Constants::Player::Projectile::Decharge;
		LastEnergyTick = actTime_;
		LastShotTick = actTime_;

		SoundEffects->PlayPlayerFire();
	}

	const irr::core::vector3df position = Camera->getPosition();
	SoundEffects->SetListenerPosition(irrklang::vec3df(position.X,position.Y,position.Z),
		irrklang::vec3df(Front.X,Front.Y,Front.Z), irrklang::vec3df(Up.X,Up.Y,Up.Z));
}

void Nightork::CPlayer::SetAimingPosition(const irr::core::vector3df& pos_, CMonster* aimedMonster_)
{
	AimingPosition = pos_;
	AimedMonster = aimedMonster_;
}

unsigned int Nightork::CPlayer::GetHpCurrent() const
{
	return HpCurrent;
}

unsigned int Nightork::CPlayer::GetHpMaximum() const
{
	return Constants::Player::HitpointMaximum[Pilot->GetTotemType()];
}

unsigned int Nightork::CPlayer::GetAfterburnerCurrent() const
{
	return AfterburnerCurrent;
}

unsigned int Nightork::CPlayer::GetAfterburnerMaximum() const
{
	return Constants::Player::Afterburner::Maximum[Pilot->GetTotemType()];
}

unsigned int Nightork::CPlayer::GetEnergyCurrent() const
{
	return EnergyCurrent;
}

unsigned int Nightork::CPlayer::GetEnergyMaximum() const
{
	return Constants::Player::Projectile::Maximum[Pilot->GetTotemType()];
}

bool Nightork::CPlayer::IsPowerUpActive() const
{
	return PowerUpActive;
}

clock_t Nightork::CPlayer::GetPowerUpStartTick() const
{
	return PowerUpStartTick;
}

void Nightork::CPlayer::DropLife(unsigned int hp_, bool projectile_)
{
	if (projectile_)
		SoundEffects->PlayPlayerHit();

	// dead?
	if (0 == HpCurrent)
		return;

	HpCurrent = HpCurrent<hp_ ? 0 : HpCurrent-hp_;

	// alive?
	if (0<HpCurrent)
		return;

	GameplayStats->CountDeath();
	SoundEffects->PlayPlayerExplosion();
	SoundEffects->PlayPlayerEngine(false);
	SoundEffects->PlayPlayerAfterburner(false);

	PSDestruction->setMinParticlesPerSecond(Constants::Player::Particle::ParticlesPerSec);
	PSDestruction->setMaxParticlesPerSecond(Constants::Player::Particle::ParticlesPerSec);
}

irr::core::vector3df Nightork::CPlayer::GetAbsoluteShootPosition() const
{
	irr::core::vector3df position = Constants::Player::Projectile::Position;
	position.X = 0.0f; // is in the middle between both shooting points -X and X
	PlayerShip->getAbsoluteTransformation().transformVect(position);
	return position;
}

irr::core::vector3df Nightork::CPlayer::GetAbsoluteShipPosition() const
{
	return PlayerShip->getAbsolutePosition(); 
}

irr::scene::ICameraSceneNode* Nightork::CPlayer::GetCamera() const
{
	return Camera;
}
