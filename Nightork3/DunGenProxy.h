// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef DUNGENPROXY_H
#define DUNGENPROXY_H

#include <string>
#include <DunGen.h>

namespace Nightork
{

class CDunGenProxy
{
private:
	/// Helper struct to access the voxel space.
	struct SVoxelSpace
	{
		/// Constructor.
		SVoxelSpace(DunGen::CDunGen* dungeonGenerator_);
		/// Reference to the voxel space.
		unsigned char (&Content)[DunGen::SVoxelSpace::DimX][DunGen::SVoxelSpace::DimY][DunGen::SVoxelSpace::DimZ];
	};

public:
	/// Constructor.
	CDunGenProxy(irr::IrrlichtDevice* irrDevice_);

	/// Destructor.
	~CDunGenProxy();

	/// Parse intro.
	void ParseIntroExtro(const irr::io::path& filename_);

	/// Loads a level.
	void LoadLevel(const irr::io::path& filename_);

	/// Assembles the current level to a dungeon.
	bool AssembleLevel();

	/// Unloads the current level.
	void UnloadLevel();

	/// Is the current scene a valid dungeon?
	bool IsValid() const;

	/// Returns the monster hitpoints.
	unsigned int GetMonsterHitpoints() const;

	/// Returns the boss monster hitpoints.
	unsigned int GetBossMonsterHitpoints() const;

	/// Returns the monster fire rate.
	unsigned int GetMonsterFireRate() const;

	/// Returns the boss monster fire rate.
	unsigned int GetBossMonsterFireRate() const;

	/// Returns the monster damage.
	unsigned int GetMonsterDamage() const;

	/// Returns the intro description.
	std::wstring GetDescriptionIntro() const;

	/// Returns the extro description.
	std::wstring GetDescriptionExtro() const;

	/// Test if a voxel in the ray is 0.
	bool TestForVoxelCollision(const irr::core::line3df& ray_, unsigned char bitmask);

private:
	/// Is the voxel a boundary voxel?
	bool IsBoundaryVoxel(unsigned int x_,unsigned int y_,unsigned int z_, unsigned char bitmask) const;

	/// Extends the cave border of the voxel space by 1.
	void TuneVoxelSpace();

	/// Test if a voxel on the line is 0.
	bool Bresenham3dX(int xs_,int ys_,int zs_,int xe_,int ye_,int ze_, unsigned char bitmask) const;

	/// Test if a voxel on the line is 0.
	bool Bresenham3dY(int xs_,int ys_,int zs_,int xe_,int ye_,int ze_, unsigned char bitmask) const;

	/// Test if a voxel on the line is 0.
	bool Bresenham3dZ(int xs_,int ys_,int zs_,int xe_,int ye_,int ze_, unsigned char bitmask) const;

private:
	irr::scene::ISceneManager* SceneManager;	///< The scene manager.
	irr::io::IFileSystem* FileSystem;			///< The file system.
	DunGen::CDunGen* DungeonGenerator;			///< The dungeon generator.

	SVoxelSpace* VoxelSpace;					///< The voxel space.
	std::wstring IntroText;						///< The intro text.
	std::wstring ExtroText;						///< The extro text.
};

}

#endif
