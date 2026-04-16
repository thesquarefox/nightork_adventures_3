// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <irrlicht.h>
#include <cmath>
#include "MonsterTypes.h"

namespace Nightork
{

namespace Constants
{
	namespace Level
	{
		struct EArena
		{
			enum Enum
			{
				Sphere,
				Hive,
				Cave,
				Number
			};
		};
	}
	
	namespace Player
	{
		/// Totem type.
		struct ETotemType
		{
			enum Enum
			{
				IceBear = 0,
				SnowTiger,
				FrostRaven,
				Number
			};
		};

		namespace Afterburner
		{
			static const unsigned int TickDecharge = 100;
			static const unsigned int TickCharge[ETotemType::Number] = {50, 50, 40};
			static const unsigned int ActivationMinimum = 10;
			static const unsigned int Maximum[ETotemType::Number] = {100, 100, 150};
		};

		namespace EngineParticle // engine particles
		{
			static const float Size = 0.25f;
			static const float DirectionFactor = -0.0003f;
			static const int Angle = 20;
			static const unsigned int AgeMin = 1000;
			static const unsigned int AgeMax = 1000;
			static const unsigned int ParticlesPerSec = 10;
			static irr::video::SColor Color = irr::video::SColor(255, 0, 255, 255);
			static irr::core::vector3df Position = irr::core::vector3df(40.0f, 0.0f, -155.0f);
		};

		namespace AfterburnerParticle // afterburner particles
		{
			static const float Size = 0.5f;
			static const float DirectionFactor = -0.0003f;
			static const int Angle = 20;
			static const unsigned int AgeMin = 1000;
			static const unsigned int AgeMax = 1000;
			static const unsigned int ParticlesPerSec = 10;
			static irr::video::SColor Color = irr::video::SColor(255,0,255,255);
			static irr::core::vector3df Position = irr::core::vector3df(40.0f,0.0f,-155.0f);
		};

		namespace Particle // explosion particles
		{
			static const float Size = 5.0f;
			static const int Angle = 360;
			static const unsigned int Age = 1000;
			static const unsigned int ParticlesPerSec = 20;
			static const irr::video::SColor Color = irr::video::SColor(255,0,255,255);
			static const irr::core::vector3df Direction = irr::core::vector3df(0.01f,0.0f,0.0f);
		}

		namespace Projectile
		{
			static const unsigned int Decharge = 10;		///< Decharging per shot.
			static const unsigned int TickCharge[ETotemType::Number] = {50, 40, 50};	///< Recharging 1 unit per X ms.
			static const unsigned int Maximum[ETotemType::Number] = {100, 150, 100};	///< Maximum charge.
			static const unsigned int ShotDelay = 200;		///< Delay for shots in ms.
			static const irr::core::vector3df Position = irr::core::vector3df(26.0f, -8.0f, 226.0f);
		}

		namespace Camera
		{
			static const float ViewPointFront = 10.4f;
			static const float PositionFront = -2.6f;
			static const float PositionUp = 0.85f;

			static const float NearValue = 0.5f;
			static const float FarValue = 1000.0f;

			static const double CameraStiffness = 1000.0f; // Federkonstante (je höher, desto „härter“ folgt die Kamera)
			static const double CameraDamping = 2.0f * std::sqrt(CameraStiffness);  // Dämpfung (je höher, desto weniger Nachschwingen)
			static const double CameraSpeedPos = 0.022f;
			static const double CameraTargetSpeed = 0.005f;
			static const double CameraUpSpeed = 0.02f;
		}
		
		namespace Movement
		{
			static const float SpeedForwardAfterburner = 0.03f;
			static const float SpeedForward = 0.015f;
			static const float SpeedBackward = -0.015f;

			static const float Accelaration = 0.00002f;
			static const float Deceleration = 0.00002f;

			static const float SpeedRoll = 0.002f;
			static const float AccelarationRoll = 0.000002f;
			static const float AccelarationYawPitch = 0.000003f;
		}

		static const unsigned int HitpointMaximum[ETotemType::Number] = {200, 150, 150};
		static const unsigned int TickHpRegen[ETotemType::Number] = {1000, 1250, 1250};
		static const unsigned int TickHpRegenPortal = 20;

		static const float ScaleDownExplodePerMs = 0.0005f;
		
		static const float LightRadius = 1000.0f;
	}

	namespace Monster
	{
		static const float MinMovementSpeed = 0.002f;
		static const float MaxMovementSpeed = 0.015f;
		static const float RandomOffsetMovementSpeed = 0.003f;
		static const float Acceleration = 0.00002f;

		static const float ScaleDownExplodePerMs = 0.0005f;

		static const unsigned int FireTimeAddMin = 50;
		static const unsigned int FireTimeAddMax = 1000;

		namespace Projectile
		{
			static const irr::core::vector3df Position[EMonsterType::NUMBER_OF_MONSTERTYPES] =
			{
				irr::core::vector3df(0.0f, 1.5f, 7.0f),		// SHIP_RED
				irr::core::vector3df(0.0f, -0.73f, 2.5f),	// SHIP_GREEN
				irr::core::vector3df(0.0f, 0.0f, -4.2f),	// SHIP_BLUE
				irr::core::vector3df(0.0f, 0.0f, -7.0f),	// SHIP_YELLOW
				irr::core::vector3df(0.0f, -4.5f, 12.0f),	// SHIP_TEAL
				irr::core::vector3df(0.0f, -1.1f, 11.0f),	// SHIP_PURPLE

				irr::core::vector3df(43.3f, 6.4f, 14.6f),	// BOSS_01
				irr::core::vector3df(11.0f, -10.3f, 35.7f),	// BOSS_02
				irr::core::vector3df(31.2f, -0.6f, 16.3f),	// BOSS_03
				irr::core::vector3df(20.4f, -4.0f, 11.7f),	// BOSS_04
				irr::core::vector3df(4.9f, -0.9f, 29.0f),	// BOSS_05
				irr::core::vector3df(11.4f, -8.0f, 24.5f)	// BOSS_06
			};
		}

		namespace Particle // explosion particles
		{
			static const float Size = 5.0f;
			static const float DeactivationScale = 0.5f;
			static const int Angle = 360;
			static const unsigned int Age = 1000;
			static const unsigned int ParticlesPerSec = 20;
			static const irr::core::vector3df Direction = irr::core::vector3df(0.01f,0.0f,0.0f);
		}
	}

	namespace Hud
	{
		static const float AimingOffset = 2.0f;
		static const unsigned int SizeX = 250;
		static const unsigned int SizeY = 20;
		static const unsigned int BorderText = 20;
		static const unsigned int BorderBackground = 2;

		static const unsigned int PosHpBarY = BorderText-BorderBackground;
		static const unsigned int SizeHpBarY = 15+BorderBackground;
	}

	namespace Projectile
	{
		namespace Player
		{
			static const float Size = 0.5f;
			static const float Speed = 0.06f;
			static const int StopLifetime = static_cast<int>(150.0f/Speed); // = range/speed
			static const unsigned int Damage = 10;

			namespace Particle // explosion particles
			{
				static const float Size = 5.0f;
				static const float DirectionFactor = -0.005f;
				static const int Angle = 360;
				static const unsigned int Age = 1000;
				static const unsigned int ParticlesPerSec = 10;
				static const int ActivationLifetime = StopLifetime;
				static const int DeactivationLifetime = ActivationLifetime+500;
			}

			static const int DestroyLifetime = Particle::DeactivationLifetime+static_cast<int>(Particle::Age);
		}

		namespace Monster
		{
			static const float Size = 1.0f;
			static const float Speed = 0.02f;
			static const float Range = 160.0f;
			static const unsigned int ImplosionStepTime = 50;
			static const unsigned int ImplosionStepNumber = 7;
			static const int StopLifetime = static_cast<int>(Range/Speed);
			static const int DestroyLifetime = StopLifetime+static_cast<int>(ImplosionStepNumber*ImplosionStepTime);
		}
	}

	namespace PowerUp
	{
		static const unsigned int Size = 2;
		static const unsigned int PickUpRadius = 7;
		static const unsigned int Lifetime = 20000;
		static const unsigned int DestructionTime = 1000;
		static const unsigned int DestroyLifetime = Lifetime + DestructionTime;

		static const unsigned int Duration = 20000;
		static const unsigned int DurationOfText = 2000;
	}

	namespace CriticalDestruction
	{
		static const unsigned int NumberOfParts = 4;
		static const unsigned int DurationOfText = 2000;
	}
}

}

#endif
