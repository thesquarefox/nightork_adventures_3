// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#ifndef OBJECTPLACEMENTCOMMON_H
#define OBJECTPLACEMENTCOMMON_H

#include <irrlicht.h>

namespace DunGen
{
	/// The type of the object
	struct EObject
	{
		enum Enum
		{
			SHIP_RED = 0,
			SHIP_GREEN = 1,
			SHIP_BLUE = 2,
			SHIP_YELLOW = 3,
			SHIP_TEAL = 4,
			SHIP_PURPLE = 5,
			NO_SHIP = 6,
			NUMBER_OF_OBJECTS = 7
		};
	};

	/// The type of the boss object
	struct EBossObject
	{
		enum Enum
		{
			BOSS_01 = 0,
			BOSS_02 = 1,
			BOSS_03 = 2,
			BOSS_04 = 3,
			BOSS_05 = 4,
			BOSS_06 = 5,
			NUMBER_OF_OBJECTS = 6
		};
	};

	/// Parameters for an object.
	struct SPlacedObject
	{
		/// The object type.
		EObject::Enum Type;

		/// The position.
		irr::core::vector3d<double> Position;
	};

	/// Parameters for an boss object.
	struct SBossObject
	{
		/// The position.
		irr::core::vector3d<double> Position;

		/// Is this placement valid?
		bool IsPlaced;
	};

	/// Parameters for the portal object.
	struct SPortalObject
	{
		/// The position.
		irr::core::vector3d<double> Position;

		/// The rotation.
		irr::core::vector3d<double> Rotation;

		/// Is this placement valid?
		bool IsPlaced;
	};

	namespace CollisionRadius
	{
		/// the minimum distance from the center of an object to the cave stone voxels
		static const unsigned int NormalShip = 5;

		/// the minimum distance from the center of a boss ship to normal ships
		static const unsigned int BossShip= 14;

		/// the minimum distance from the center of the portal to normal ships
		static const unsigned int Portal = 15;
	}
}

#endif