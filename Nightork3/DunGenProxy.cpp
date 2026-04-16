// Copyright (C) 2011-2026 by Maximilian Hönig

#include <iostream>
#include <ObjectPlacementCommon.h>
#include "DunGenProxy.h"

Nightork::CDunGenProxy::SVoxelSpace::SVoxelSpace(DunGen::CDunGen* dungeonGenerator_)
	: Content(dungeonGenerator_->GetVoxelSpace())
{
}

Nightork::CDunGenProxy::CDunGenProxy(irr::IrrlichtDevice* irrDevice_)
	: SceneManager(irrDevice_->getSceneManager())
	, FileSystem(irrDevice_->getFileSystem())
	, DungeonGenerator(new DunGen::CDunGen(irrDevice_))
	, VoxelSpace(NULL)
	, IntroText()
{
}

Nightork::CDunGenProxy::~CDunGenProxy()
{
	delete VoxelSpace;
	delete DungeonGenerator;
}

void Nightork::CDunGenProxy::ParseIntroExtro( const irr::io::path& filename_ )
{
	std::cout << "[DunGenProxy:] Parsing intro of " << filename_.c_str();
	IntroText.clear();
	ExtroText.clear();

	// get read access to file
	irr::io::IReadFile* file = FileSystem->createAndOpenFile(filename_);

	if (!file)
	{
		std::cout << " failed." << std::endl;
		return;
	}

	irr::io::IXMLReader* xmlReader = FileSystem->createXMLReader(file);

	// process file content
	while(xmlReader->read())
	{
		if (xmlReader->getNodeType() == irr::io::EXN_ELEMENT &&  irr::core::stringw("Description") == xmlReader->getNodeName())
		{
			IntroText = xmlReader->getAttributeValue(L"Intro");
			ExtroText = xmlReader->getAttributeValue(L"Extro");
		}
	}

	// close file
	xmlReader->drop();
	file->drop();

	std::cout << " succeeded." << std::endl;
}


void Nightork::CDunGenProxy::LoadLevel(const irr::io::path& filename_)
{
	// create dungeon
	DungeonGenerator->StartUp();
	DungeonGenerator->SetPrintToConsole(true);
	DungeonGenerator->ReadDungeonFromFile(filename_);
}

bool Nightork::CDunGenProxy::AssembleLevel()
{
	DungeonGenerator->AddDungeon(SceneManager->getRootSceneNode(),SceneManager);

	VoxelSpace = new SVoxelSpace(DungeonGenerator);
	TuneVoxelSpace();

	return IsValid();
}

void Nightork::CDunGenProxy::UnloadLevel()
{
	delete VoxelSpace;
	VoxelSpace = NULL;
	DungeonGenerator->ShutDown();
}

bool Nightork::CDunGenProxy::IsValid() const
{
	if (!SceneManager->getSceneNodeFromName(
		"PlayerShip",SceneManager->getRootSceneNode()))
		return false;
	if (!SceneManager->getSceneNodeFromName(
		"Cave",SceneManager->getRootSceneNode()))
		return false;
	return true;
}

unsigned int Nightork::CDunGenProxy::GetMonsterHitpoints() const
{
	return DungeonGenerator->GetMonsterHitpoints();
}

unsigned int Nightork::CDunGenProxy::GetMonsterFireRate() const
{
	return DungeonGenerator->GetMonsterFireTime();
}

unsigned int Nightork::CDunGenProxy::GetBossMonsterFireRate() const
{
	return DungeonGenerator->GetBossMonsterFireTime();
}

unsigned int Nightork::CDunGenProxy::GetBossMonsterHitpoints() const
{
	return DungeonGenerator->GetBossMonsterHitpoints();
}

unsigned int Nightork::CDunGenProxy::GetMonsterDamage() const
{
	return DungeonGenerator->GetMonsterDamage();
}

std::wstring Nightork::CDunGenProxy::GetDescriptionIntro() const
{
	return IntroText;
}

std::wstring Nightork::CDunGenProxy::GetDescriptionExtro() const
{
	return ExtroText;
}

bool Nightork::CDunGenProxy::IsBoundaryVoxel(unsigned int x_,unsigned int y_,unsigned int z_, unsigned char bitmask) const
{
	// a border voxel has to be a free voxel
	if (0 != (bitmask & VoxelSpace->Content[x_][y_][z_]))
		return false;

	// if it has a 6-connected not free voxel, it is a boundary voxel
	return 0 != (
		  (bitmask & VoxelSpace->Content[x_ - 1][y_][z_])
		+ (bitmask & VoxelSpace->Content[x_ + 1][y_][z_])
		+ (bitmask & VoxelSpace->Content[x_][y_ - 1][z_])
		+ (bitmask & VoxelSpace->Content[x_][y_ + 1][z_])
		+ (bitmask & VoxelSpace->Content[x_][y_][z_ - 1])
		+ (bitmask & VoxelSpace->Content[x_][y_][z_ + 1])
		);
}

void Nightork::CDunGenProxy::TuneVoxelSpace()
{
	// reverse voxelspace: 0 is free voxel, other is filled voxel
	for (unsigned int i = 1; i <= DunGen::SVoxelSpace::DimX - 1; ++i)
		for (unsigned int j = 1; j <= DunGen::SVoxelSpace::DimY - 1; ++j)
			for (unsigned int k = 1; k <= DunGen::SVoxelSpace::DimZ - 1; ++k)
				VoxelSpace->Content[i][j][k] ^= 1;

	// step 1: mark voxels to extend
	for (unsigned int i=1; i<=DunGen::SVoxelSpace::DimX-1; ++i)
		for (unsigned int j=1; j<=DunGen::SVoxelSpace::DimY-1; ++j)
			for (unsigned int k=1; k<=DunGen::SVoxelSpace::DimZ-1; ++k)
				if (IsBoundaryVoxel(i, j, k, 3))
					VoxelSpace->Content[i][j][k] = 4;

	// step 2: extend marked voxels
	for (unsigned int i=1; i<=DunGen::SVoxelSpace::DimX-1; ++i)
		for (unsigned int j=1; j<=DunGen::SVoxelSpace::DimY-1; ++j)
			for (unsigned int k=1; k<=DunGen::SVoxelSpace::DimZ-1; ++k)
				if (4 == VoxelSpace->Content[i][j][k])
					VoxelSpace->Content[i][j][k] = 1;

	for (unsigned int c = 0; c < DunGen::CollisionRadius::NormalShip-2; ++c)
	{
		// step 1: mark voxels to extend
		for (unsigned int i = 1; i <= DunGen::SVoxelSpace::DimX - 1; ++i)
			for (unsigned int j = 1; j <= DunGen::SVoxelSpace::DimY - 1; ++j)
				for (unsigned int k = 1; k <= DunGen::SVoxelSpace::DimZ - 1; ++k)
					if (IsBoundaryVoxel(i, j, k, 3))
						VoxelSpace->Content[i][j][k] = 4;

		// step 2: extend marked voxels
		for (unsigned int i = 1; i <= DunGen::SVoxelSpace::DimX - 1; ++i)
			for (unsigned int j = 1; j <= DunGen::SVoxelSpace::DimY - 1; ++j)
				for (unsigned int k = 1; k <= DunGen::SVoxelSpace::DimZ - 1; ++k)
					if (4 == VoxelSpace->Content[i][j][k])
						VoxelSpace->Content[i][j][k] = 2;
	}
}

bool Nightork::CDunGenProxy::TestForVoxelCollision(const irr::core::line3df& ray_, unsigned char bitmask)
{
	// test collision in voxel space
	const irr::core::vector3di start = irr::core::vector3di(static_cast<int>(ray_.start.X),static_cast<int>(ray_.start.Y),static_cast<int>(ray_.start.Z));
	irr::core::vector3di end = irr::core::vector3di(static_cast<int>(ray_.end.X),static_cast<int>(ray_.end.Y),static_cast<int>(ray_.end.Z));

	if (0 > start.X || 0 > start.Y || 0 > start.Z
		|| 0 > end.X || 0 > end.Y || 0 > end.Z
		|| DunGen::SVoxelSpace::DimX-1 < start.X || DunGen::SVoxelSpace::DimY-1 < start.Y || DunGen::SVoxelSpace::DimZ-1 < start.Z
		|| DunGen::SVoxelSpace::DimX-1 < end.X || DunGen::SVoxelSpace::DimY-1 < end.Y || DunGen::SVoxelSpace::DimZ-1 < end.Z)
		return true;

	// compute axis lengths
	int dx=abs (end.X-start.X);
	int dy=abs (end.Y-start.Y);
	int dz=abs (end.Z-start.Z);

	if (4 > dx && 4 > dy && 4 > dz)
	{
		// fast test
		int incX = start.X < end.X ? 1 : -1;
		int incY = start.Y < end.Y ? 1 : -1;
		int incZ = start.Z < end.Z ? 1 : -1;

		for (int x = start.X; x != end.X + incX; x+=incX)
			for (int y = start.Y; y != end.Y + incY; y+=incY)
				for (int z = start.Z; z != end.Z + incZ; z+=incZ)
					if (0 != (bitmask & VoxelSpace->Content[x][y][z]))
						return true;

		return false;
	}

	// slow test
	if ((dx>dy)&&(dx>dz)) return Bresenham3dX(start.X,start.Y,start.Z,end.X,end.Y,start.Z, bitmask);// longest axis: X
	else if (dy>dz) return Bresenham3dY(start.X,start.Y,start.Z,end.X,end.Y,start.Z, bitmask);		// longest axis: Y
	else return Bresenham3dZ(start.X,start.Y,start.Z,end.X,end.Y,start.Z, bitmask);					// longest axis: Z
}

bool Nightork::CDunGenProxy::Bresenham3dX(int xs_,int ys_,int zs_,int xe_,int ye_,int ze_, unsigned char bitmask) const
{	// input: endpoints
	
	// compute directions
	int xinc = (xe_<xs_) ? -1 : 1;
	int yinc = (ye_<ys_) ? -1 : 1;
	int zinc = (ze_<zs_) ? -1 : 1;

	// compute dx,dy,dz
	int dx=abs (xe_-xs_);
	int dy=abs (ye_-ys_);
	int dz=abs (ze_-zs_);

	// compute initial decision parameters
	int ey=2*dy-dx; int ez=2*dz-dx;
	// compute constants for Y
	int twoDy=2*dy; int twoDyDx=2*(dy-dx);
	// compute constants for Z
	int twoDz=2*dz; int twoDzDx=2*(dz-dx);
	// inialize position
	int x=xs_; int y=ys_; int z=zs_;
	
	// loop to draw the line
	for (int i=0; i<=dx; ++i)
	{
		// test voxel
		if (0 != (bitmask & VoxelSpace->Content[x][y][z]))
		{
			return true;
		}

		// start of computing the next position
		x+=xinc; 
		if (ey>=0) {
			y+=yinc; ey+=twoDyDx;
		}
		else
			ey+=twoDy;
		
		if (ez>=0) {
			z+=zinc; ez+=twoDzDx;
		}
		else
			ez+=twoDz;
		// end of computing the next position
	}

	return false;
}

bool Nightork::CDunGenProxy::Bresenham3dY(int xs_,int ys_,int zs_,int xe_,int ye_,int ze_, unsigned char bitmask) const
{	// input: endpoints

	// compute directions
	int xinc = (xe_<xs_) ? -1 : 1;
	int yinc = (ye_<ys_) ? -1 : 1;
	int zinc = (ze_<zs_) ? -1 : 1;

	// compute dx,dy,dz
	int dx=abs (xe_-xs_);
	int dy=abs (ye_-ys_);
	int dz=abs (ze_-zs_);

	// compute initial decision parameters
	int ex=2*dx-dy; int ez=2*dz-dy;
	// compute constants for X
	int twoDx=2*dx; int twoDxDy=2*(dx-dy);
	// compute constants for Z
	int twoDz=2*dz; int twoDzDy=2*(dz-dy);
	// inialize position
	int x=xs_; int y=ys_; int z=zs_;
	
	// loop to draw the line
	for (int i=0; i<=dy; ++i)
	{
		// test voxel
		if (0 != (bitmask & VoxelSpace->Content[x][y][z]))
		{
			return true;
		}

		// start of computing the next position
		y+=yinc; 
		if (ex>=0) {
			x+=xinc; ex+=twoDxDy;
		}
		else
			ex+=twoDx;
		
		if (ez>=0) {
			z+=zinc; ez+=twoDzDy;
		}
		else
			ez+=twoDz;
		// end of computing the next position
	}

	return false;
}

bool Nightork::CDunGenProxy::Bresenham3dZ(int xs_,int ys_,int zs_,int xe_,int ye_,int ze_, unsigned char bitmask) const
{	// input: endpoints

	// compute directions
	int xinc = (xe_<xs_) ? -1 : 1;
	int yinc = (ye_<ys_) ? -1 : 1;
	int zinc = (ze_<zs_) ? -1 : 1;

	// compute dx,dy,dz
	int dx=abs (xe_-xs_);
	int dy=abs (ye_-ys_);
	int dz=abs (ze_-zs_);

	// compute initial decision parameters
	int ey=2*dy-dz; int ex=2*dx-dz;
	// compute constants for Y
	int twoDy=2*dy; int twoDyDz=2*(dy-dz);
	// compute constants for Z
	int twoDx=2*dx; int twoDxDz=2*(dx-dz);
	// inialize position
	int x=xs_; int y=ys_; int z=zs_;
	
	// loop to draw the line
	for (int i=0; i<=dz; ++i)
	{
		// test voxel
		if (0 != (bitmask & VoxelSpace->Content[x][y][z]))
		{
			return true;
		}

		/// start of computing the next position
		z+=zinc; 
		if (ey>=0) {
			y+=yinc; ey+=twoDyDz;
		}
		else
			ey+=twoDy;
		
		if (ex>=0) {
			x+=xinc; ex+=twoDxDz;
		}
		else
			ex+=twoDx;
		// end of computing the next position
	}

	return false;
}