// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#ifndef OBJECTPLACEMENT_H
#define OBJECTPLACEMENT_H

#include "interface/ObjectPlacementCommon.h"
#include <irrlicht.h>
#include <vector>

/// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	/// object placement class
	///
	/// class for placing object in the cave
	class CObjectPlacement
	{
	public:
		/// constructor
		CObjectPlacement();
		
		/// destructor
		~CObjectPlacement();

		/// delete all normal objects
		void DeleteObjects();

		/// delete all bosses and the portal
		void DeleteStaticObjects();

		/// adds a single objects
		void AddObject(const SPlacedObject& object_);

		/// get the placed objects
		std::vector<SPlacedObject> GetPlacedObjects() const;

		/// sets a boss object
		void SetBossObject(EBossObject::Enum type_, const SBossObject& object_);

		/// get boss object
		SBossObject GetBossObject(EBossObject::Enum type_) const;

		/// sets the portal object
		void SetPortalObject(const SPortalObject& object_);

		/// get portal object
		SPortalObject GetPortalObject() const;

	private:
		/// the placed objects
		std::vector<SPlacedObject> PlacedObjects;

		/// the boss objects
		SBossObject BossObjects[EBossObject::NUMBER_OF_OBJECTS];

		/// the portal object
		SPortalObject PortalObject;
	};

} // END NAMESPACE DunGen

#endif