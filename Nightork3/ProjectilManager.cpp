// Copyright (C) 2011-2026 by Maximilian Hönig

#include <algorithm>
#include <iostream>
#include "CollisionManager.h"
#include "Constants.h"
#include "DunGenProxy.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "Player.h"
#include "ProjectilManager.h"

Nightork::CProjectileManager::CProjectileManager(irr::IrrlichtDevice* irrDevice_, CCollisionManager* collisionManager_, CDunGenProxy* dunGen_)
	: SceneManager(irrDevice_->getSceneManager())
	, VideoDriver(irrDevice_->getVideoDriver())
	, CollisionManager(collisionManager_)
	, DunGenProxy(dunGen_)
	, LastUpdateTick(0)
	, MonsterManager(NULL)
	, PlayerShip(NULL)
{
	// load textures for monster projectile implosion
	irr::core::array<irr::video::ITexture*> textures;
	for (unsigned int i=Constants::Projectile::Monster::ImplosionStepNumber; 0 < i; --i)
	{
		irr::core::stringc tmp;
		tmp = "data/ProjectileExploding";
		tmp += i;
		tmp += ".png";
		irr::video::ITexture* t = VideoDriver->getTexture(tmp.c_str());
		textures.push_back(t);
	}
	// create texture animator
	ImplosionMonsterProjectile = SceneManager->createTextureAnimator(textures, Constants::Projectile::Monster::ImplosionStepTime);
}

Nightork::CProjectileManager::~CProjectileManager()
{
}

void Nightork::CProjectileManager::Initialize(CMonsterManager* monsterManager_, CPlayer* playerShip_)
{
	MonsterManager = monsterManager_;
	PlayerShip = playerShip_;
}

namespace
{
	bool ExpirePP(const Nightork::CProjectileManager::SProjectilePlayer& proj_)
	{
		if (Nightork::Constants::Projectile::Player::DestroyLifetime <= proj_.Lifetime)
		{
			proj_.Projectile->remove();
			proj_.Particle->remove();
			return true;
		}
		return false;
	}

	bool ExpireMP(const Nightork::CProjectileManager::SProjectileMonster& proj_)
	{
		if (Nightork::Constants::Projectile::Monster::DestroyLifetime <= proj_.Lifetime)
		{
			proj_.Projectile->remove();
			return true;
		}
		return false;
	}
}
void Nightork::CProjectileManager::Update(clock_t actTime_)
{
	const int ticked = actTime_ - LastUpdateTick;

	// update all player projectiles
	for (unsigned int i=0; i<ProjectilesPlayer.size(); ++i)
	{
		ProjectilesPlayer[i].Lifetime += ticked;
		// movement
		if (Constants::Projectile::Player::StopLifetime > ProjectilesPlayer[i].Lifetime)
		{
			const irr::core::vector3df oldPos = ProjectilesPlayer[i].Projectile->getAbsolutePosition();
			const float distance = ProjectilesPlayer[i].Speed * ticked;

			// target search
			bool targetReached = false;
			if (ProjectilesPlayer[i].Monster)
			{
				irr::core::vector3df monsterDir = ProjectilesPlayer[i].Monster->GetMonsterNode()->getAbsolutePosition() - oldPos;
				if (monsterDir.getLengthSQ() <= distance)
					targetReached = true;
				ProjectilesPlayer[i].Direction = monsterDir.normalize();
			}
			
			irr::core::vector3df newPos = ProjectilesPlayer[i].Projectile->getPosition() + ProjectilesPlayer[i].Direction * distance;
			irr::core::vector3df collPos;
			irr::core::line3df ray(oldPos,newPos);
			irr::scene::ISceneNode* monsterNode = NULL;

			// if the projectile collides with the cave, it stops at the collision point
			if (CollisionManager->GetCollisionPointWithCave(ray,collPos))
			{
				ProjectilesPlayer[i].Lifetime = Constants::Projectile::Player::StopLifetime;
				newPos = collPos;
			}
			else if (CollisionManager->GetCollisionPointWithPortal(ray,collPos))
			{
				ProjectilesPlayer[i].Lifetime = Constants::Projectile::Player::StopLifetime;
				newPos = collPos;
			}
			else if (targetReached)
			{
				switch (ProjectilesPlayer[i].Color)
				{
				case EColor::MAGIC:
					MonsterManager->DropLife(ProjectilesPlayer[i].Monster->GetMonsterNode(), Constants::Projectile::Player::Damage, 0, false, ProjectilesPlayer[i].PowerUpped);
					break;
				case EColor::ANTIMAGIC:
					MonsterManager->DropLife(ProjectilesPlayer[i].Monster->GetMonsterNode(), 0, Constants::Projectile::Player::Damage, false, ProjectilesPlayer[i].PowerUpped);
					break;
				case EColor::OVERLOAD:
					MonsterManager->DropLife(ProjectilesPlayer[i].Monster->GetMonsterNode(), 0, 0, true, false);
					break;
				}

				ProjectilesPlayer[i].Lifetime = Constants::Projectile::Player::StopLifetime;
				newPos = ProjectilesPlayer[i].Monster->GetMonsterNode()->getAbsolutePosition();
			}
			// if the projectile collides with a monster, it stops at the collision point
			else if (CollisionManager->GetCollisionPointWithMonster(ray,collPos,monsterNode))
			{
				switch (ProjectilesPlayer[i].Color)
				{
				case EColor::MAGIC:
					MonsterManager->DropLife(monsterNode, Constants::Projectile::Player::Damage, 0, false, ProjectilesPlayer[i].PowerUpped);
					break;
				case EColor::ANTIMAGIC:
					MonsterManager->DropLife(monsterNode, 0, Constants::Projectile::Player::Damage, false, ProjectilesPlayer[i].PowerUpped);
					break;
				case EColor::OVERLOAD:
					MonsterManager->DropLife(monsterNode, 0, 0, true, false);
					break;
				}
				
				ProjectilesPlayer[i].Lifetime = Constants::Projectile::Player::StopLifetime;
				newPos = collPos;
			}
			
			ProjectilesPlayer[i].Projectile->setPosition(newPos);
			ProjectilesPlayer[i].Particle->setPosition(newPos);
		}

		if (Constants::Projectile::Player::StopLifetime <= ProjectilesPlayer[i].Lifetime)
		{
			ProjectilesPlayer[i].Projectile->setVisible(false);
		}

		// particle effect
		if (Constants::Projectile::Player::Particle::DeactivationLifetime <= ProjectilesPlayer[i].Lifetime)
		{
			ProjectilesPlayer[i].Particle->getEmitter()->setMinParticlesPerSecond(0);
			ProjectilesPlayer[i].Particle->getEmitter()->setMaxParticlesPerSecond(0);
		}
		else if (Constants::Projectile::Player::Particle::ActivationLifetime <= ProjectilesPlayer[i].Lifetime)
		{
			ProjectilesPlayer[i].Particle->getEmitter()->setMinParticlesPerSecond(Constants::Projectile::Player::Particle::ParticlesPerSec);
			ProjectilesPlayer[i].Particle->getEmitter()->setMaxParticlesPerSecond(Constants::Projectile::Player::Particle::ParticlesPerSec);
		}
	}

	// delete all expired projectiles
	ProjectilesPlayer.erase(std::remove_if(std::begin(ProjectilesPlayer),std::end(ProjectilesPlayer), ExpirePP), std::end(ProjectilesPlayer));

	// update all monster projectiles
	for (unsigned int i=0; i<ProjectilesMonster.size(); ++i)
	{
		ProjectilesMonster[i].Lifetime += ticked;
		// movement
		if (Constants::Projectile::Monster::StopLifetime > ProjectilesMonster[i].Lifetime)
		{
			const irr::core::vector3df oldPos = ProjectilesMonster[i].Projectile->getAbsolutePosition();
			irr::core::vector3df newPos = ProjectilesMonster[i].Projectile->getPosition()
				+ ProjectilesMonster[i].Direction * (Constants::Projectile::Monster::Speed * ticked);
			irr::core::vector3df collPos;
			irr::core::line3df ray(oldPos,newPos);

			// if the projectile collides with the cave or portal, it stops at the collision point
			if (CollisionManager->GetCollisionPointWithCave(ray,collPos))
			{
				ProjectilesMonster[i].Lifetime = Constants::Projectile::Monster::StopLifetime;
				newPos = collPos;
			}
			else if (CollisionManager->GetCollisionPointWithPortal(ray,collPos))
			{
				ProjectilesMonster[i].Lifetime = Constants::Projectile::Monster::StopLifetime;
				newPos = collPos;
			}
			else if (CollisionManager->GetCollisionPointWithPlayer(ray,collPos))
			{
				PlayerShip->DropLife(DunGenProxy->GetMonsterDamage(),true);
				ProjectilesMonster[i].Lifetime = Constants::Projectile::Monster::StopLifetime;
				newPos = collPos;
			}

			ProjectilesMonster[i].Projectile->setPosition(newPos);
		}

		if (Constants::Projectile::Monster::StopLifetime <= ProjectilesMonster[i].Lifetime)
		{
			ProjectilesMonster[i].Projectile->addAnimator(ImplosionMonsterProjectile);
		}
	}

	// delete all expired projectiles
	ProjectilesMonster.erase(std::remove_if(std::begin(ProjectilesMonster),std::end(ProjectilesMonster), ExpireMP), std::end(ProjectilesMonster));

	LastUpdateTick = actTime_;
}

void Nightork::CProjectileManager::Cleanup()
{
	for (unsigned int i = 0; i < ProjectilesPlayer.size(); ++i)
		if (ProjectilesPlayer[i].Monster && ProjectilesPlayer[i].Monster->IsDestroyed())
			ProjectilesPlayer[i].Monster = NULL;
}

void Nightork::CProjectileManager::CreateProjectilePlayer(const irr::core::vector3df& pos_, const irr::core::vector3df& dir_, CMonster* monster_, EColor::Enum color_, bool powerUpped_)
{
	SProjectilePlayer newProjectile;
	newProjectile.Color = color_;
	newProjectile.Lifetime = 0;
	newProjectile.Direction = dir_;
	newProjectile.Monster = monster_;
	newProjectile.PowerUpped = powerUpped_;

	// add mesh
	const float size = color_ != EColor::OVERLOAD ? Constants::Projectile::Player::Size : Constants::Projectile::Player::Size * 5.0f;
	newProjectile.Projectile = SceneManager->addSphereSceneNode(size,8,0,-1,pos_);

	newProjectile.Projectile->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	newProjectile.Projectile->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
	newProjectile.Projectile->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
	newProjectile.Projectile->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);

	irr::video::SColor color;
	switch (color_)
	{

	case EColor::MAGIC:
		newProjectile.Projectile->setMaterialTexture(0, VideoDriver->getTexture("data/projectile_green.png"));
		color = irr::video::SColor(255,50,255,50);
		newProjectile.Speed = Constants::Projectile::Player::Speed;
		break;
	case EColor::ANTIMAGIC:
		newProjectile.Projectile->setMaterialTexture(0, VideoDriver->getTexture("data/projectile_orange.png"));
		color = irr::video::SColor(255, 255, 128, 50);
		newProjectile.Speed = Constants::Projectile::Player::Speed;
		break;
	case EColor::OVERLOAD:
		newProjectile.Projectile->setMaterialTexture(0, VideoDriver->getTexture("data/projectile_white.png"));
		color = irr::video::SColor(255, 255, 255, 255);
		newProjectile.Speed = Constants::Projectile::Player::Speed * 0.5f;
		break;
	}
	
	// add particle system
	newProjectile.Particle = SceneManager->addParticleSystemSceneNode(
		false, 0, -1, pos_);

	irr::scene::IParticleEmitter* emitter = newProjectile.Particle->createPointEmitter(
		dir_*Constants::Projectile::Player::Particle::DirectionFactor,	// direction
		0,0,									// emit rate
		color,									// darkest color
		color,									// brightest color
		Constants::Projectile::Player::Particle::Age,	// min age,
		Constants::Projectile::Player::Particle::Age,	// max age,
		Constants::Projectile::Player::Particle::Angle,	// angle
		irr::core::dimension2df(Constants::Projectile::Player::Particle::Size,Constants::Projectile::Player::Particle::Size),// min size
		irr::core::dimension2df(Constants::Projectile::Player::Particle::Size,Constants::Projectile::Player::Particle::Size) // max size
		);
	newProjectile.Particle->setEmitter(emitter);
	newProjectile.Particle->setParticlesAreGlobal(false);
	emitter->drop();

	irr::scene::IParticleAffector* paf = newProjectile.Particle->createFadeOutParticleAffector();
	newProjectile.Particle->addAffector(paf);
	paf->drop();

	newProjectile.Particle->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	newProjectile.Particle->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
	newProjectile.Particle->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
	newProjectile.Particle->setMaterialTexture(0, VideoDriver->getTexture("data/particle_white.png"));
	newProjectile.Particle->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);

	ProjectilesPlayer.push_back(newProjectile);
}

void Nightork::CProjectileManager::CreateProjectileMonster(const irr::core::vector3df& pos_, const irr::core::vector3df& dir_, EMonsterType::Enum type_)
{
	SProjectileMonster newProjectile;
	newProjectile.Lifetime = 0;
	newProjectile.Direction = dir_;

	irr::video::SColor color;
	switch (type_)
	{
	case EMonsterType::SHIP_RED:
		color = irr::video::SColor(255,255,50,50);
		break;
	case EMonsterType::SHIP_GREEN:
		color = irr::video::SColor(255,50,255,50);
		break;
	case EMonsterType::SHIP_BLUE:
		color = irr::video::SColor(255,50,50,255);
		break;
	case EMonsterType::SHIP_YELLOW:
		color = irr::video::SColor(255,255,255,50);
		break;
	case EMonsterType::SHIP_TEAL:
		color = irr::video::SColor(255,50,255,255);
		break;
	case EMonsterType::SHIP_PURPLE:
		color = irr::video::SColor(255,255,50,255);
		break;
	default:
		color = irr::video::SColor(255,255,255,255);
		break;
	}

	// add mesh
	newProjectile.Projectile = SceneManager->addBillboardSceneNode(
		0,
		irr::core::dimension2df(Constants::Projectile::Monster::Size,Constants::Projectile::Monster::Size),
		pos_,
		-1,
		color,
		color
		);

	newProjectile.Projectile->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	newProjectile.Projectile->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
	newProjectile.Projectile->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
	newProjectile.Projectile->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
	newProjectile.Projectile->setMaterialTexture(0, VideoDriver->getTexture("data/ProjectileExploding1.png"));

	ProjectilesMonster.push_back(newProjectile);
}
