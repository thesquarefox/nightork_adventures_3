// Copyright (C) 2011-2026 by Maximilian Hönig

#include <ObjectPlacementCommon.h>
#include "Constants.h"
#include "DunGenProxy.h"
#include "Monster.h"
#include "ProjectilManager.h"
#include "SoundEffects.h"

Nightork::CMonster::CMonster(irr::scene::IMeshSceneNode* monster_, irr::IrrlichtDevice* irrDevice_, CSoundEffects* soundEffects_, CProjectileManager* projectileManager_, CDunGenProxy* dunGen_)
	: SoundEffects(soundEffects_)
	, SceneManager(irrDevice_->getSceneManager())
	, VideoDriver(irrDevice_->getVideoDriver())
	, ProjectileManager(projectileManager_)
	, DunGenProxy(dunGen_)
	, Monster(monster_)
	, OverloadDeath(false)
	, PowerUpDeath(false)
	, PatrolCenter(monster_->getAbsolutePosition())
	, PatrolRadius(0.0f)
	, PatrolTarget(monster_->getAbsolutePosition())
	, CurrentSpeed(0.0f)
	, CurrentMaxSpeed(0.0f)
	, Scale(1.0f)
	, BaseScale(Monster->getScale().X)	// equal scale for all axis
	, Particle(NULL)
	, LastExplosionUpdateTick(0)
	, LastFireTick(0)
	, LastTime(0)
{
	// create triangle selector
	irr::scene::ITriangleSelector* ts = SceneManager->createTriangleSelector(Monster->getMesh(), Monster);
	Monster->setTriangleSelector(ts);
	ts->drop();

	// determine monster type and hitpoints
	if (!strcmp("Ship",Monster->getName()))
	{
		const unsigned int hitpoints = dunGen_->GetMonsterHitpoints();

		HpMaximum[EShield::PRIMARY] = hitpoints;
		HpMaximum[EShield::SECONDARY] = hitpoints;
		HpMaximum[EShield::TERTIARY] = hitpoints;

		switch (Monster->getID())
		{
		case 1:
			MonsterType = EMonsterType::SHIP_RED;
			HpColor[EShield::PRIMARY] = EColor::MAGIC;
			HpColor[EShield::SECONDARY] = EColor::ANTIMAGIC;
			HpColor[EShield::TERTIARY] = EColor::ANTIMAGIC;
			break;
		case 2:
			MonsterType = EMonsterType::SHIP_GREEN;
			HpColor[EShield::PRIMARY] = EColor::MAGIC;
			HpColor[EShield::SECONDARY] = EColor::ANTIMAGIC;
			HpColor[EShield::TERTIARY] = EColor::MAGIC;
			break;
		case 3:
			MonsterType = EMonsterType::SHIP_BLUE;
			HpColor[EShield::PRIMARY] = EColor::MAGIC;
			HpColor[EShield::SECONDARY] = EColor::MAGIC;
			HpColor[EShield::TERTIARY] = EColor::ANTIMAGIC;
			break;
		case 4:
			MonsterType = EMonsterType::SHIP_YELLOW;
			HpColor[EShield::PRIMARY] = EColor::ANTIMAGIC;
			HpColor[EShield::SECONDARY] = EColor::ANTIMAGIC;
			HpColor[EShield::TERTIARY] = EColor::MAGIC;
			break;
		case 5:
			MonsterType = EMonsterType::SHIP_TEAL;
			HpColor[EShield::PRIMARY] = EColor::ANTIMAGIC;
			HpColor[EShield::SECONDARY] = EColor::MAGIC;
			HpColor[EShield::TERTIARY] = EColor::MAGIC;
			break;
		default:
			MonsterType = EMonsterType::SHIP_PURPLE;
			HpColor[EShield::PRIMARY] = EColor::ANTIMAGIC;
			HpColor[EShield::SECONDARY] = EColor::MAGIC;
			HpColor[EShield::TERTIARY] = EColor::ANTIMAGIC;
			break;
		}
	}
	else
	{
		const unsigned int hitpoints = dunGen_->GetBossMonsterHitpoints();

		HpMaximum[EShield::PRIMARY] = hitpoints;
		HpMaximum[EShield::SECONDARY] = hitpoints;
		HpMaximum[EShield::TERTIARY] = hitpoints;

		switch (Monster->getID())
		{
		case 1:
			MonsterType = EMonsterType::BOSS_01;
			HpColor[EShield::PRIMARY] = EColor::MAGIC;
			HpColor[EShield::SECONDARY] = EColor::ANTIMAGIC;
			HpColor[EShield::TERTIARY] = EColor::MAGIC;
			break;
		case 2:
			MonsterType = EMonsterType::BOSS_02;
			HpColor[EShield::PRIMARY] = EColor::ANTIMAGIC;
			HpColor[EShield::SECONDARY] = EColor::MAGIC;
			HpColor[EShield::TERTIARY] = EColor::ANTIMAGIC;
			break;
		case 3:
			MonsterType = EMonsterType::BOSS_03;
			HpColor[EShield::PRIMARY] = EColor::MAGIC;
			HpColor[EShield::SECONDARY] = EColor::MAGIC;
			HpColor[EShield::TERTIARY] = EColor::ANTIMAGIC;
			break;
		case 4:
			MonsterType = EMonsterType::BOSS_04;
			HpColor[EShield::PRIMARY] = EColor::ANTIMAGIC;
			HpColor[EShield::SECONDARY] = EColor::ANTIMAGIC;
			HpColor[EShield::TERTIARY] = EColor::MAGIC;
			break;
		case 5:
			MonsterType = EMonsterType::BOSS_05;
			HpColor[EShield::PRIMARY] = EColor::MAGIC;
			HpColor[EShield::SECONDARY] = EColor::ANTIMAGIC;
			HpColor[EShield::TERTIARY] = EColor::ANTIMAGIC;
			break;
		default:
			MonsterType = EMonsterType::BOSS_06;
			HpColor[EShield::PRIMARY] = EColor::ANTIMAGIC;
			HpColor[EShield::SECONDARY] = EColor::MAGIC;
			HpColor[EShield::TERTIARY] = EColor::MAGIC;
			break;
		}
	}

	HpCurrent[EShield::PRIMARY] = HpMaximum[EShield::PRIMARY];
	HpCurrent[EShield::SECONDARY] = HpMaximum[EShield::SECONDARY];
	HpCurrent[EShield::TERTIARY] = HpMaximum[EShield::TERTIARY];
}

Nightork::CMonster::~CMonster()
{
}

void Nightork::CMonster::InitializePatroling(const std::vector<irr::core::vector3df>& monsterPositions_, const std::vector<irr::core::vector3df>& bossPositions_, irr::core::vector3df playerPosition_)
{
	PatrolRadius = (PatrolCenter - playerPosition_).getLength() - DunGen::CollisionRadius::Portal - DunGen::CollisionRadius::NormalShip;

	for (const auto& pos : monsterPositions_)
	{
		float dist = (pos - PatrolCenter).getLength();
		if (dist > 0.01f)
		{
			float radius = 0.5f * dist -  DunGen::CollisionRadius::NormalShip;
			if (radius < PatrolRadius)
				PatrolRadius = radius;
		}
	}

	for (const auto& pos : bossPositions_)
	{
		float radius = (pos - PatrolCenter).getLength() - DunGen::CollisionRadius::BossShip - DunGen::CollisionRadius::NormalShip;
		if (radius < PatrolRadius)
			PatrolRadius = radius;
	}
}

namespace
{
	float randomFromMinToMax(float value_)
	{
		return (static_cast<float>(rand()) / RAND_MAX) * 2.0f * value_ - value_;
	}
}

void Nightork::CMonster::Update(const irr::core::vector3df& playerShipPosition_, clock_t actTime_)
{
	const unsigned int timePassed = actTime_ - LastTime;
	LastTime = actTime_;

	// normal monsters on the move
	if (EMonsterType::BOSS_01 > MonsterType)
	{
		const irr::core::vector3df currentPos = Monster->getAbsolutePosition();

		const irr::core::vector3df movementDirection = PatrolTarget - currentPos;
		const float distanceToTarget = movementDirection.getLength();

		const float targetSpeed = (distanceToTarget < PatrolRadius * 0.2f) ? Constants::Monster::MinMovementSpeed : CurrentMaxSpeed;

		if (CurrentSpeed < targetSpeed)
		{
			CurrentSpeed += Constants::Monster::Acceleration * timePassed;
			if (CurrentSpeed > targetSpeed)
				CurrentSpeed = targetSpeed;
		}
		else
		{
			CurrentSpeed -= Constants::Monster::Acceleration * timePassed;
			if (CurrentSpeed < targetSpeed)
				CurrentSpeed = targetSpeed;
		}

		const float moveStep = CurrentSpeed * timePassed;

		const irr::core::vector3df movementVector = movementDirection / distanceToTarget;
		irr::core::vector3df newPos = currentPos + movementVector * moveStep;

		const bool caveCollision = DunGenProxy->TestForVoxelCollision(irr::core::line3df(newPos, currentPos), 3);
		
		if (distanceToTarget < moveStep || caveCollision) {

			newPos = caveCollision ? currentPos : PatrolTarget;
			CurrentSpeed = 0.0f;
			CurrentMaxSpeed = Constants::Monster::MaxMovementSpeed + randomFromMinToMax(Constants::Monster::RandomOffsetMovementSpeed);

			if (true || PatrolTarget == PatrolCenter)
			{
				irr::core::vector3df offset(
					randomFromMinToMax(PatrolRadius),
					randomFromMinToMax(PatrolRadius),
					randomFromMinToMax(PatrolRadius)
				);
				offset.setLength(PatrolRadius);
				PatrolTarget = PatrolCenter + offset;
			}
			else
			{
				PatrolTarget = PatrolCenter;
			}
		}

		Monster->setPosition(newPos);
	}

	// rotate towards player ship
	const irr::core::vector3df toTarget(playerShipPosition_ - Monster->getAbsolutePosition());
	Monster->setRotation(toTarget.getHorizontalAngle());

	// fire projectile?
	const unsigned int fireTimePassed = actTime_-LastFireTick;
	if (Constants::Projectile::Monster::Range*Constants::Projectile::Monster::Range >= toTarget.getLengthSQ()
		&& LastFireTick < actTime_ && (
		(EMonsterType::BOSS_01>MonsterType && DunGenProxy->GetMonsterFireRate() <= fireTimePassed)
		|| (EMonsterType::BOSS_01<=MonsterType && DunGenProxy->GetBossMonsterFireRate() <= fireTimePassed)
		))
	{
		LastFireTick = actTime_ + rand()%(Constants::Monster::FireTimeAddMax-Constants::Monster::FireTimeAddMin) + Constants::Monster::FireTimeAddMin;

		irr::core::vector3df projPosition;
		Monster->getAbsoluteTransformation().transformVect(projPosition,Constants::Monster::Projectile::Position[static_cast<unsigned int>(MonsterType)]);
		irr::core::vector3df toTargetProj = playerShipPosition_ - projPosition;
		toTargetProj.normalize();
		ProjectileManager->CreateProjectileMonster(projPosition, toTargetProj, MonsterType);

		if (EMonsterType::BOSS_01<=MonsterType)
		{
			irr::core::vector3df tmp = Constants::Monster::Projectile::Position[MonsterType];
			tmp.X = -tmp.X;
			Monster->getAbsoluteTransformation().transformVect(projPosition,tmp);
			toTargetProj = playerShipPosition_ - projPosition;
			toTargetProj.normalize();
			ProjectileManager->CreateProjectileMonster(projPosition, toTargetProj, MonsterType);
		}
	}

	// in destruction?
	if (Particle)
	{
		const int ticked = actTime_-LastExplosionUpdateTick;
		LastExplosionUpdateTick = actTime_;
		Scale -= ticked*Constants::Monster::ScaleDownExplodePerMs;

		if (Constants::Monster::Particle::DeactivationScale >= Scale)
		{
			Particle->getEmitter()->setMinParticlesPerSecond(0);
			Particle->getEmitter()->setMaxParticlesPerSecond(0);
		}

		// only rescale when in destruction, not when special attacking
		if (0==HpCurrent[EShield::TERTIARY])
		{
			const float resScale = Scale*BaseScale;
			Monster->setScale(irr::core::vector3df(resScale,resScale,resScale));
		}
		// set special effects trigger to off again?
		else if (0.0f >= Scale)
		{
			Scale = 1.0f;
			Particle->remove();
			Particle = NULL;
		}
	}
}

bool Nightork::CMonster::IsDestroyed() const
{
	return (0.0f >= Scale) && (0 == HpCurrent[EShield::TERTIARY]);
}

bool Nightork::CMonster::IsDestroyedByOverload() const
{
	return OverloadDeath;
}

bool Nightork::CMonster::IsDestroyedByPowerUpped() const
{
	return PowerUpDeath;
}

irr::scene::IMeshSceneNode* Nightork::CMonster::GetMonsterNode() const
{
	return Monster;
}

Nightork::EMonsterType::Enum Nightork::CMonster::GetMonsterType() const
{
	return MonsterType;
}

unsigned int Nightork::CMonster::GetHitpointsCurrent(EShield::Enum shield_) const
{
	return HpCurrent[shield_];
}

unsigned int Nightork::CMonster::GetHitpointsMaximum(EShield::Enum shield_) const
{
	return HpMaximum[shield_];
}

Nightork::EColor::Enum Nightork::CMonster::GetHitpointsColor(EShield::Enum shield_) const
{
	return HpColor[shield_];
}

void Nightork::CMonster::DropLife(unsigned int color1_, unsigned int color2_, bool overload_, bool powerUpped_)
{
	if (Particle)	// invulnerable on destruction and special attacks
		return;

	if (overload_ && MonsterType < EMonsterType::BOSS_01)
	{
		for (int shield = 0; shield < EShield::NUMBER; ++shield)
			HpCurrent[shield] = 0;
		OverloadDeath = true;
	}
	else
	{
		for (int shield = 0; shield < EShield::NUMBER; ++shield)
			if (HpCurrent[shield] > 0) {
				DropLifeByShield(color1_, color2_, static_cast<EShield::Enum>(shield));
				break;
			}
	}

	PowerUpDeath = powerUpped_;

	// alive?
	if (0 == HpCurrent[EShield::TERTIARY])
		Collision();
}

void Nightork::CMonster::Collision()
{
	if (Particle)
		return;

	const irr::core::vector3df pos = Monster->getAbsolutePosition();
	SoundEffects->PlayMonsterExplosion(irrklang::vec3df(pos.X,pos.Y,pos.Z));

	// normal ships are destroyed on collision, boss monsters use special attack
	if (MonsterType<EMonsterType::BOSS_01)
	{
		HpCurrent[EShield::PRIMARY] = 0;
		HpCurrent[EShield::SECONDARY] = 0;
		HpCurrent[EShield::TERTIARY] = 0;
	}

	// add particle system
	Particle = SceneManager->addParticleSystemSceneNode(false, Monster);
	
	irr::video::SColor color;
	switch (MonsterType)
	{
	case EMonsterType::SHIP_RED:
		color = irr::video::SColor(255,255,0,0);
		break;
	case EMonsterType::SHIP_GREEN:
		color = irr::video::SColor(255,0,255,0);
		break;
	case EMonsterType::SHIP_BLUE:
		color = irr::video::SColor(255,0,0,255);
		break;
	case EMonsterType::SHIP_YELLOW:
		color = irr::video::SColor(255,255,255,0);
		break;
	case EMonsterType::SHIP_TEAL:
		color = irr::video::SColor(255,0,255,255);
		break;
	case EMonsterType::SHIP_PURPLE:
		color = irr::video::SColor(255,255,0,255);
		break;
	default:
		color = irr::video::SColor(255,255,255,255);
		break;
	}

	irr::scene::IParticleEmitter* emitter = Particle->createPointEmitter(
		Constants::Monster::Particle::Direction,		// direction
		Constants::Monster::Particle::ParticlesPerSec,	// emit rate
		Constants::Monster::Particle::ParticlesPerSec,	// emit rate
		color,											// darkest color
		color,											// brightest color
		Constants::Monster::Particle::Age,				// min age
		Constants::Monster::Particle::Age,				// max age
		Constants::Monster::Particle::Angle,			// angle
		irr::core::dimension2df(Constants::Monster::Particle::Size,Constants::Monster::Particle::Size),// min size
		irr::core::dimension2df(Constants::Monster::Particle::Size,Constants::Monster::Particle::Size) // max size
		);
	Particle->setEmitter(emitter);
	Particle->setParticlesAreGlobal(false);
	emitter->drop();

	irr::scene::IParticleAffector* paf = Particle->createFadeOutParticleAffector();
	Particle->addAffector(paf);
	paf->drop();

	Particle->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	Particle->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
	Particle->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
	Particle->setMaterialTexture(0, VideoDriver->getTexture("data/particle_white.png"));
	Particle->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);

	LastExplosionUpdateTick = LastTime;
}

void Nightork::CMonster::DropLifeByShield(unsigned int color1_, unsigned int color2_, EShield::Enum shield_)
{
	if (HpColor[shield_] == EColor::MAGIC)
		HpCurrent[shield_] = HpCurrent[shield_] < color1_ ? 0 : HpCurrent[shield_] - color1_;
	else
		HpCurrent[shield_] = HpCurrent[shield_] < color2_ ? 0 : HpCurrent[shield_] - color2_;
}
