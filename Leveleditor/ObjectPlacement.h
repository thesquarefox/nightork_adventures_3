// Copyright (C) 2011-2014 by Maximilian Hönig

#ifndef OBJECTPLACEMENT_H
#define OBJECTPLACEMENT_H

#include <DunGen.h>
#include <irrlicht.h>
#include <vector>

namespace Editor
{
	/// object placement class
	///
	/// class for placing object in the cave
	class CObjectPlacement
	{
	public:
		/// Constructor.
		CObjectPlacement(DunGen::CDunGen* dungeonGenerator_);
		
		/// Destructor.
		~CObjectPlacement();

		/// Place the objects.
		void PlaceObjects();

		private:
		/// Try to place an object.
		bool CheckForPlaceability(unsigned int x_,unsigned int y_,unsigned int z_);

		/// Mark voxel space as placed.
		void MarkAsPlaced(unsigned int x_, unsigned int y_, unsigned int z_, unsigned int radius_);

	public:
		/// The minimum distance from the center of an object to the center of an other object is 2*CollisionRadiusCave + AdditionalDistanceObjects.
		unsigned int AdditionalDistanceToShip;

		/// The minimum distance from the center of an object to the center of a boss is 2*CollisionRadiusCave + AdditionalDistanceToBoss.
		unsigned int AdditionalDistanceToBoss;

		/// The minimum distance from the center of an object to the center of the portal is 2*CollisionRadiusCave + AdditionalDistanceToPortal.
		unsigned int AdditionalDistanceToPortal;

		/// Probability factors for objects.
		unsigned int ObjectFactors[DunGen::EObject::NUMBER_OF_OBJECTS];

	private:
		/// The dungeon generator.
		DunGen::CDunGen* DungeonGenerator;

		/// Reference to voxel space.
		unsigned char (&VoxelSpace)[DunGen::SVoxelSpace::DimX][DunGen::SVoxelSpace::DimY][DunGen::SVoxelSpace::DimZ];
	};
}

#endif