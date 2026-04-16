// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#ifndef DUNGEONGENERATOR_H
#define DUNGEONGENERATOR_H

#include "interface/MaterialDunGen.h"
#include <irrlicht.h>
#include <string>
#include <vector>

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	class CLSystem;
	class CObjectPlacement;
	class CRandomGenerator;
	class CRoomPattern;
	class CMaterialProvider;
	class CMeshCave;
	class CTimer;
	class CVoxelCave;

	struct SRoomInstance;

	class CDungeonGenerator
	{
	public:
		/// Constructor.
		CDungeonGenerator(irr::IrrlichtDevice* irrDevice_, CMaterialProvider* materialProvider_);

		/// Destructor.
		~CDungeonGenerator();

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// generators
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		/// Creates the cave from the current parameters.
		void GenerateCave();

		/// Assembles the dungeon and adds it under the specified node in the specified scene manager.
		void AddDungeon(irr::scene::ISceneNode* parentNode_, irr::scene::ISceneManager* sceneManager_);

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// parameters
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		// Get subgenerator (to configure it).
		/// Returns the random generator.
		CRandomGenerator* GetRandomGenerator();
		/// Returns the L-system generator.
		CLSystem* GetLSystem();
		/// Returns the voxel cave generator
		CVoxelCave* GetVoxelCave();
		/// Returns the mesh cave generator.
		CMeshCave* GetMeshCave();
		/// Returns the object placement.
		CObjectPlacement* GetObjectPlacement();

		/// Adds an erosion step for the voxel cave.
		void VoxelCaveAddErosionStep(double likelihood_);
		/// Deletes all erosion steps for the voxel cave.
		void VoxelCaveDeleteErosionSteps();
		/// Gets the erosion steps for the voxel cave.
		std::vector<double> VoxelCaveGetErosionSteps() const;

		/// Sets the filtering option for the voxel cave.
		void VoxelCaveSetFiltering(bool filtering_);
		/// Gets the filtering option for the voxel cave.
		bool VoxelCaveGetFiltering() const;

		/// Sets the backface culling option for the cave material.
		void MaterialSetCaveBackfaceCulling(bool backFaceCulling_);
		/// Gets the backface culling option for the cave material.
		bool MaterialGetCaveBackfaceCulling() const;

		/// Sets the anti aliasing option for the cave material.
		void MaterialSetCaveAntiAliasing(bool antiAliasing_);
		/// Gets the anti aliasing option for the cave material.
		bool MaterialGetCaveAntiAliasing() const;

		/// Sets the color for the cave single color material.
		void MaterialSetCaveColor(const irr::video::SColorf& color_);
		/// Gets the color for the cave single color material.
		irr::video::SColorf MaterialGetCaveColor() const;

		/// Sets a custom material for the cave.
		void MaterialSetCaveMaterial(EMaterialType::Enum materialCave_, CMaterialDunGen* material_);
		/// Gets a custom material for the cave.
		EMaterialType::Enum MaterialGetCaveMaterial() const;

		/// Sets the hit points for ship objects (not player).
		void SetMonsterHitpoints(unsigned int val_);

		/// Returns the hit points for ship objects (not player).
		unsigned int GetMonsterHitpoints() const;

		/// Sets the hit points for boss ship objects (not player).
		void SetBossMonsterHitpoints(unsigned int val_);

		/// Returns the hit points for boss ship objects (not player).
		unsigned int GetBossMonsterHitpoints() const;

		/// Sets the base fire time for ship objects (not player).
		void SetMonsterFireTime(unsigned int val_);

		/// Returns the base fire time for ship objects (not player).
		unsigned int GetMonsterFireTime() const;

		/// Sets the base fire time for boss ship objects (not player).
		void SetBossMonsterFireTime(unsigned int val_);

		/// Returns the base fire time for boss ship objects (not player).
		unsigned int GetBossMonsterFireTime() const;

		/// Returns the damage for all ship objects (not player).
		unsigned int GetMonsterDamage() const;

		/// Sets the damage for all ship objects (not player).
		void SetMonsterDamage(unsigned int val_);

		/// Sets the intro description text.
		void SetDescriptionIntro(const std::wstring& val_);

		/// Returns he intro description text.
		std::wstring GetDescriptionIntro() const;

		/// Sets the extro description text.
		void SetDescriptionExtro(const std::wstring& val_);

		/// Returns the extro description text.
		std::wstring GetDescriptionExtro() const;

		/// Sets if status reports shall be printed to the console.
		void SetPrintToConsole(bool enabled_);

	private:
		CRandomGenerator* RandomGenerator;								///< the random generator
		CLSystem* LSystem;												///< the L-system generator
		CVoxelCave* VoxelCave;											///< the voxel cave generator
		CMeshCave* MeshCave;											///< the mesh cave generator
		CObjectPlacement* ObjectPlacement;								///< the object placement
		CTimer* Timer;													///< the timer

		std::vector<double>	ErosionSteps;								///< The erosion steps for the voxelcave
		bool FilteringEnabled;											///< Shall the voxel cave be filtered?

		CMaterialProvider* MaterialProvider;							///< the material provider
		EMaterialType::Enum MaterialCave;								///< the material used for the cave
		bool MaterialCaveBackfaceCulling;								///< Shall backface culling for the cave be enabled?
		bool MaterialCaveAntiAliasing;									///< Shall antialiasing for the cave be enabled?

		unsigned int MonsterHitpoints;									///< The hit points for ship objects (not player).
		unsigned int BossMonsterHitpoints;								///< The hit points for boss ship objects (not player).
		unsigned int MonsterFireTime;									///< The base fire time for ship objects (not player).
		unsigned int BossMonsterFireTime;								///< The base fire time for boss ship objects (not player).
		unsigned int MonsterDamage;										///< The damage of monsters ship objects (not player).
		std::wstring DescriptionIntro;									///< The intro description text.
		std::wstring DescriptionExtro;									///< The extro description text.

		irr::IrrlichtDevice* IrrDevice;									///< The irrlicht device.

		bool PrintToConsole;											///< Print status reports to console?
	};

} // END NAMESPACE DunGen

#endif