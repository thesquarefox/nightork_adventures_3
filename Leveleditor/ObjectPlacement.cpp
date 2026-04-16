// Copyright (C) 2011-2014 by Maximilian Hönig

#include "ObjectPlacement.h"
#include <iostream>
#include <xutility>
#include <vector>

namespace
{
	/// convert double to integer with rounding up and down
	inline int d2i(double d_)
	{
		return static_cast<int>(d_<0?d_-0.5:d_+0.5);
	}
}

Editor::CObjectPlacement::CObjectPlacement(DunGen::CDunGen* dungeonGenerator_)
	: AdditionalDistanceToShip(20)
	, AdditionalDistanceToBoss(20)
	, AdditionalDistanceToPortal(20)
	, DungeonGenerator(dungeonGenerator_)
	, VoxelSpace(dungeonGenerator_->GetVoxelSpace())
{
	for (unsigned int i=0; i<DunGen::EObject::NO_SHIP; ++i)
		ObjectFactors[i] = 10;
	ObjectFactors[DunGen::EObject::NO_SHIP] = 0;
}

Editor::CObjectPlacement::~CObjectPlacement()
{
}

void Editor::CObjectPlacement::PlaceObjects()
{
	// delete old objects:
	DungeonGenerator->ObjectsDelete();

	// setup the random parameters:
	std::vector<DunGen::EObject::Enum> objectFactors;
	for (unsigned int i=0; i<DunGen::EObject::NUMBER_OF_OBJECTS; ++i)
		for (unsigned int j=0; j<ObjectFactors[i]; ++j)
			objectFactors.push_back(static_cast<DunGen::EObject::Enum>(i));

	std::cout << "Objects are placed..." << std::endl;

	// place static objects:
	for (unsigned int i=0; i<DunGen::EBossObject::NUMBER_OF_OBJECTS; ++i)
	{
		DunGen::SBossObject boss = DungeonGenerator->ObjectsGetBossObject(static_cast<DunGen::EBossObject::Enum>(i));
		if (boss.IsPlaced)
			MarkAsPlaced(d2i(boss.Position.X),d2i(boss.Position.Y), d2i(boss.Position.Z),
			DunGen::CollisionRadius::BossShip + AdditionalDistanceToBoss);
	}

	{
		DunGen::SPortalObject portal = DungeonGenerator->ObjectsGetPortalObject();
		if (portal.IsPlaced)
		MarkAsPlaced(d2i(portal.Position.X),d2i(portal.Position.Y), d2i(portal.Position.Z),
			DunGen::CollisionRadius::Portal + AdditionalDistanceToPortal);
	}

	// place random objects:
	for (unsigned int i=DunGen::CollisionRadius::NormalShip; i<DunGen::SVoxelSpace::DimX - DunGen::CollisionRadius::NormalShip; ++i)
	{
		std::cout << ".";
		for (unsigned int j=DunGen::CollisionRadius::NormalShip; j<DunGen::SVoxelSpace::DimY - DunGen::CollisionRadius::NormalShip; ++j)
			for (unsigned int k=DunGen::CollisionRadius::NormalShip; k<DunGen::SVoxelSpace::DimZ - DunGen::CollisionRadius::NormalShip; ++k)
				if (1 == VoxelSpace[i][j][k])
					if (CheckForPlaceability(i,j,k))
					{
						MarkAsPlaced(i,j,k,DunGen::CollisionRadius::NormalShip + AdditionalDistanceToShip);

						DunGen::SPlacedObject newObject;
						newObject.Type = objectFactors[rand()%objectFactors.size()];

						if (DunGen::EObject::NO_SHIP != newObject.Type)
						{
							newObject.Position.X = i + 0.5;
							newObject.Position.Y = j + 0.5;
							newObject.Position.Z = k + 0.5;
							DungeonGenerator->ObjectsAddObject(newObject);
						}
					}
	}

	// restore marking:
	for (unsigned int i=0; i<DunGen::SVoxelSpace::DimX; ++i)
		for (unsigned int j=0; j<DunGen::SVoxelSpace::DimY; ++j)
			for (unsigned int k=0; k<DunGen::SVoxelSpace::DimZ; ++k)
				if (2 == VoxelSpace[i][j][k])
					VoxelSpace[i][j][k] = 1;

	std::cout << std::endl << "#objects placed = " << DungeonGenerator->ObjectsGetObjects().size() << std::endl;
}

bool Editor::CObjectPlacement::CheckForPlaceability(unsigned int x_,unsigned int y_,unsigned int z_)
{
	// check if placement possible:
	const int radius = static_cast<int>(DunGen::CollisionRadius::NormalShip);
	const int radiusSquared = radius*radius;
	int xSquared;
	int xSquaredPlusYSquared;

	// walk over the bounding cube of the sphere
	for (int i = -radius; i<=radius; ++i)
	{
		// compute x^2
		xSquared = i*i;
		for (int j = -radius; j<=radius; ++j)
		{
			// compute x^2 + y^2
			xSquaredPlusYSquared = xSquared + j*j;
			for (int k = -radius; k<=radius; ++k)
				// test, if x^2 + y^2 + z^2 no larger than radius^2
				// -> belongs to sphere -> test voxel
				if ((xSquaredPlusYSquared+k*k) <= radiusSquared)
					if (1 != VoxelSpace[x_+i][y_+j][z_+k])
						return false;
		}
	}
	return true;
}

void Editor::CObjectPlacement::MarkAsPlaced(unsigned int x_, unsigned int y_, unsigned int z_, unsigned int radius_)
{
	const int i1 = std::max(0,static_cast<int>(x_)-static_cast<int>(radius_));
	const int i2 = static_cast<int>(std::min(DunGen::SVoxelSpace::DimX-1,x_+radius_));
	const int j1 = std::max(0,static_cast<int>(y_)-static_cast<int>(radius_));
	const int j2 = static_cast<int>(std::min(DunGen::SVoxelSpace::DimY-1,y_+radius_));
	const int k1 = std::max(0,static_cast<int>(z_)-static_cast<int>(radius_));
	const int k2 = static_cast<int>(std::min(DunGen::SVoxelSpace::DimZ-1,z_+radius_));
	const int radiusSquared = radius_*radius_;

	int xSquared;
	int xSquaredPlusYSquared;

	// walk over the bounding cube of the sphere
	for (int i = i1; i<=i2; ++i)
	{
		// compute x^2
		xSquared = (x_-i)*(x_-i);
		for (int j = j1; j<=j2; ++j)
		{
			// compute x^2 + y^2
			xSquaredPlusYSquared = xSquared + (static_cast<int>(y_)-j)*(static_cast<int>(y_)-j);
			for (int k = k1; k<=k2; ++k)
				// test, if x^2 + y^2 + z^2 no larger than radius^2
				// -> belongs to sphere -> mark voxel
				if ((xSquaredPlusYSquared+(static_cast<int>(z_)-k)*(static_cast<int>(z_)-k)) <= radiusSquared)
					if (1 == VoxelSpace[i][j][k])
						VoxelSpace[i][j][k] = 2;
		}
	}
}
