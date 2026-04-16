// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#ifndef DUNGEN_H
#define DUNGEN_H

#include "LSystemCommon.h"
#include "MaterialDunGen.h"
#include "MeshCaveCommon.h"
#include "ObjectPlacementCommon.h"
#include "VoxelCaveCommon.h"
#include <irrlicht.h>
#include <string>
#include <vector>

#define DllExport __declspec(dllexport)

/// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	class CDunGenXMLReader;
	class CDunGenXMLWriter;
	class CDungeonGenerator;
	class CMaterialProvider;

	/// Interfaceclass for the DunGen-DungeonGenerator.
	class DllExport CDunGen
	{
	public:
		/// Constructor.
		/// \param irrDevice The Irrlicht device.
		CDunGen(irr::IrrlichtDevice* irrDevice);

		/// Destructor.
		~CDunGen();

		/// Start up the Dungeongenerator.
		void StartUp();

		/// Shut down the Dungeongenerator. Deletes all unnecessary objects.
		void ShutDown();

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// generators
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		/// Creates the cave from the current parameters.
		void GenerateCave();

		/// Reads the dungeon from a file. The dungeon creation steps will be done accordingly to the XML description in this file.
		/// \param filename The filename of the file.
		/// \return Returns true, if the file exists and can be processed, false otherwise.
		bool ReadDungeonFromFile(const irr::io::path& filename);

		/// Writes the dungeon parameters to a file.
		/// \param filename The filename of the file.
		/// \return Returns true, if the file exists and can be processed, false otherwise.
		bool WriteDungeonToFile(const irr::io::path& filename);

		/// Assembles the dungeon and adds it under the specified node in the specified scene manager.
		/// \param parentNode The parent node for the dungeon.
		/// \param sceneManager The scene manager parentNode belongs to.
		void AddDungeon(irr::scene::ISceneNode* parentNode, irr::scene::ISceneManager* sceneManager);

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// parameters
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		/// Set the parameter 'seed' for the random generator: linear congruential generator, X[i+1] = (a*X[i]+c) mod m).
		/// \param seed The random seed.
		void RandomGeneratorSetSeed(unsigned int seed);

		/// Get the parameter 'seed' for the random generator: linear congruential generator, X[i+1] = (a*X[i]+c) mod m).
		/// \return The random seed.
		unsigned int RandomGeneratorGetSeed() const;

		// Set the parameter 'a' for the random generator: linear congruential generator, X[i+1] = (a*X[i]+c) mod m).
		/// \param a The parameter a.
		void RandomGeneratorSetA(unsigned int a);

		/// Get the parameter 'a' for the random generator: linear congruential generator, X[i+1] = (a*X[i]+c) mod m).
		/// \return The parameter a.
		unsigned int RandomGeneratorGetA() const;

		// Set the parameter 'c' for the random generator: linear congruential generator, X[i+1] = (a*X[i]+c) mod m).
		/// \param c The parameter c.
		void RandomGeneratorSetC(unsigned int c);

		/// Get the parameter c for the random generator: linear congruential generator, X[i+1] = (a*X[i]+c) mod m).
		/// \return The parameter c.
		unsigned int RandomGeneratorGetC() const;

		// Set the parameter 'm' for the random generator: linear congruential generator, X[i+1] = (a*X[i]+c) mod m).
		/// \param m The parameter m.
		void RandomGeneratorSetM(unsigned int m);

		/// Get the parameter 'm' for the random generator: linear congruential generator, X[i+1] = (a*X[i]+c) mod m).
		/// \return The parameter m.
		unsigned int RandomGeneratorGetM() const;

		// L-system parameter functions:

		/// Sets the maximum iteration for the L-system
		/// \param iteration The maximum iteration.
		void LSystemSetIteration(unsigned int iteration);

		/// Gets the maximum iteration for the L-system
		/// \return The iteration of the L-system.
		unsigned int LSystemGetIteration() const;

		/// Sets a parameter for the L-system.
		/// \param parameter The parameter.
		/// \param value The value to be set.
		void LSystemSetParameter(ELSystemParameter::Enum parameter, double value);

		// Gets a parameter for the L-system.
		/// \param parameter The parameter.
		/// \return The value of the parameter
		double LSystemGetParameter(ELSystemParameter::Enum parameter) const;

		/// Sets the start symbols for the L-system
		/// \param start The start symbols.
		void LSystemSetStart(const std::string& start);

		/// Gets the start symbols for the L-system
		/// \return The start symbols.
		std::string LSystemGetStart() const;

		/// Adds a substitution rule for the L-system.
		/// \param rule The substitution rule.
		void LSystemAddRule(const SSubstitutionRule& rule);

		/// Deletes all substitution rules for the L-system.
		void LSystemDeleteRules();

		/// Gets the the substitution rules of the L-system
		/// \return The substitution rules of the L-system
		std::vector<SSubstitutionRule> LSystemGetRules() const;

		// Voxel cave parameter functions:

		/// Sets the border for the voxel cave.
		/// \param border The border of 0-voxels, which will be preserved. Will be capped to at least SVoxelSpace::MinBorder
		void VoxelCaveSetBorder(unsigned int border);

		/// Gets the border of the voxel cave.
		/// \return The border of 0-voxels, which will be preserved. Will be capped to at least SVoxelSpace::MinBorder
		unsigned int VoxelCaveGetBorder() const;

		/// Sets the minimum draw radius for the voxel cave.
		/// \param mindrawradius The minimum drawing radius, which will be not deceeded.
		void VoxelCaveSetMinDrawRadius(unsigned int mindrawradius);

		/// Gets the minimum draw radius of the voxel cave.
		/// \returnThe minimum drawing radius, which will be not deceeded.
		unsigned int VoxelCaveGetMinDrawRadius() const;

		/// Adds an erosion step for the voxel cave.
		/// param likelihood The erosion likelihood.
		void VoxelCaveAddErosionStep(double likelihood);
		
		/// Deletes all erosion steps for the voxel cave.
		void VoxelCaveDeleteErosionSteps();
		
		/// Gets the erosion steps for the voxel cave.
		/// \return The erosion steps.
		std::vector<double> VoxelCaveGetErosionSteps() const;

		/// Sets the filtering option for the voxel cave.
		/// \param filtering Shall filtering be enabled?
		void VoxelCaveSetFiltering(bool filtering);
		
		/// Gets the filtering option for the voxel cave.
		/// \return Is filtering be enabled?
		bool VoxelCaveGetFiltering() const;

		/// Estimates the mesh complexity for the mesh cave which results from the current voxel cave.
		/// \returns The estimated number of triangles needed.
		unsigned int VoxelCaveEstimateMeshComplexity() const;

		// Mesh cave parameter functions:

		/// Sets the warp enabled option for the mesh cave.
		/// \param warpEnabled Shall the voxels be warped, when creating the mesh cave?
		void MeshCaveSetWarpEnabled(bool warpEnabled);

		/// Gets the warp enabled option for the mesh cave.
		/// \return Shall the voxels be warped, when creating the mesh cave?
		bool MeshCaveGetWarpEnabled() const;

		/// Sets the smooth enabled option for the mesh cave.
		/// \param smoothEnabled Shall the warping smooth the mesh?
		void MeshCaveSetSmoothEnabled(bool smoothEnabled);

		/// Gets the smooth enabled option for the mesh cave.
		/// \return Shall the warping smooth the mesh?
		bool MeshCaveGetSmoothEnabled() const;

		/// Sets the warp strength for the mesh cave.
		/// \param warpStrength Strength of the warping. Will be clamped to [0,0.49].
		void MeshCaveSetWarpStrength(double warpStrength);

		/// Gets the warp strength for the mesh cave.
		/// \return Strength of the warping. Will be clamped to [0,0.49].
		double MeshCaveGetWarpStrength() const;

		/// Sets the warp random seed for the mesh cave.
		/// \param warpRandomSeed Random seed for the warping.
		void MeshCaveSetWarpRandomSeed(unsigned int warpRandomSeed);

		/// Gets the warp random seed for the mesh cave.
		/// \return Random seed for the warping.
		unsigned int MeshCaveGetWarpRandomSeed() const;

		/// Sets the normal weighting method for the mesh cave.
		/// \param value The normal weighting method.
		void MeshCaveSetNormalWeightMethod(ENormalWeightMethod::Enum value);

		/// Gets the normal weighting method for the mesh cave.
		/// \return The normal weighting method.
		ENormalWeightMethod::Enum MeshCaveGetNormalWeightMethod() const;

		// Material parameters:

		/// Chooses the single color material for the mesh cave and sets the parameters for it.
		
		/// Sets the backface culling option for the cave material.
		/// \param backFaceCulling Shall backface culling be enabled?
		void MaterialSetCaveBackfaceCulling(bool backFaceCulling);

		// Gets the backface culling option for the cave material.
		/// \return Shall backface culling be enabled?
		bool MaterialGetCaveBackfaceCulling() const;

		/// Sets the anti aliasing option for the cave material.
		/// \param antiAliasing Shall anti aliasing culling be enabled?
		void MaterialSetCaveAntiAliasing(bool antiAliasing);

		/// Gets the anti aliasing option for the cave material.
		/// \return Shall anti aliasing culling be enabled?
		bool MaterialGetCaveAntiAliasing() const;

		/// Sets the color for the cave single color material.
		/// \param color The color of the cave.
		void MaterialSetCaveColor(const irr::video::SColorf& color);

		/// Gets the color for the cave single color material.
		/// \return The color of the cave.
		irr::video::SColorf MaterialGetCaveColor() const;

		/// Sets a material for the cave.
		/// \param materialCave The type of the material.
		/// \param material The material being used, if CAVE_CUSTOM is used.
		void MaterialSetCaveMaterial(EMaterialType::Enum materialCave, CMaterialDunGen* material);

		/// Gets the material for the cave.
		/// \return The type of the material.
		EMaterialType::Enum MaterialGetCaveMaterial() const;

		// Object parameters:

		/// Delete all normal objects.
		void ObjectsDelete();

		/// Delete all bosses and the portal.
		void ObjectsDeleteStatic();

		/// Adds a single object.
		/// \param object The object to add.
		void ObjectsAddObject(const SPlacedObject& object);

		/// Gets the currently placed objects.
		/// \return The currently placed objects.
		std::vector<SPlacedObject> ObjectsGetObjects();

		/// Sets a boss object.
		/// \param type The type of the boss.
		/// \param object The boss object.
		void ObjectsSetBossObject(EBossObject::Enum type, const SBossObject& object);

		/// Gets boss object.
		/// \param type The type of the boss.
		/// \return The boss object.
		SBossObject ObjectsGetBossObject(EBossObject::Enum type) const;

		/// Sets the portal object.
		/// \param object The portal object.
		void ObjectsSetPortalObject(const SPortalObject& object);

		/// Gets the portal object.
		/// \return The portal object.
		SPortalObject ObjectsGetPortalObject() const;

		// Gameplay parameters:

		/// Sets the hit points for ship objects (not player).
		/// \param val The hit points.
		void SetMonsterHitpoints(unsigned int val);

		/// Returns the hit points for ship objects (not player).
		/// \return The hit points.
		unsigned int GetMonsterHitpoints() const;

		/// Sets the hit points for boss ship objects (not player).
		/// \param val The hit points.
		void SetBossMonsterHitpoints(unsigned int val);

		/// Returns the hit points for boss ship objects (not player).
		/// \return The hit points.
		unsigned int GetBossMonsterHitpoints() const;

		/// Sets the base fire time for ship objects (not player).
		/// \param val The base fire time.
		void SetMonsterFireTime(unsigned int val);

		/// Returns the base fire time for ship objects (not player).
		/// \return The base fire time.
		unsigned int GetMonsterFireTime() const;

		/// Sets the base fire time for boss ship objects (not player).
		/// \param val The base fire time.
		void SetBossMonsterFireTime(unsigned int val);

		/// Returns the base fire time for boss ship objects (not player).
		/// \return The base fire time.
		unsigned int GetBossMonsterFireTime() const;

		/// Sets the damage for all ship objects (not player).
		/// \param val The damage.
		void SetMonsterDamage(unsigned int val);

		/// Returns the damage for all ship objects (not player).
		/// \return The damage.
		unsigned int GetMonsterDamage() const;

		/// Sets the intro description text.
		/// \param val The intro description text.
		void SetDescriptionIntro(const std::wstring& val);

		/// Returns he intro description text.
		/// \return The intro description text.
		std::wstring GetDescriptionIntro() const;

		/// Sets the extro description text.
		/// \param val The extro description text.
		void SetDescriptionExtro(const std::wstring& val);

		/// Returns the extro description text.
		/// \return The extro description text.
		std::wstring GetDescriptionExtro() const;

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// other
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		/// Sets if status reports shall be printed to the console.
		/// \param value If true, status reports will be printed to the console.
		void SetPrintToConsole(bool value);

		/// Provides direct access to voxel space.
		///
		/// Use with: unsigned char (&voxelspace)[DunGen::SVoxelSpace::DimX][DunGen::SVoxelSpace::DimY][DunGen::SVoxelSpace::DimZ] = dunGen->GetVoxelSpace(); .
		/// Then you can set voxels with: voxelspace[x][y][z] = value; .
		/// Only set voxels to 0 (stone) or 1 (free space), preserve a three voxel border of 0-voxels.
		/// All 1-voxel have to be 6-connected.
		/// Be careful with this (could crash if you do not preserve the border).
		/// \returns A reference to the three-dimensional array of the voxel space.
		unsigned char (&GetVoxelSpace())[SVoxelSpace::DimX][SVoxelSpace::DimY][SVoxelSpace::DimZ];

	private:
		/// Private copy constructor, because it shall not be used.
		CDunGen(const CDunGen& other);
		/// Private assignment operator, because it shall not be used.
		CDunGen& operator=(const CDunGen& other);

		/// Implementation of the dungeon generator.
		CDungeonGenerator* DungeonGenerator;

		/// The material provider.
		CMaterialProvider* MaterialProvider;

		/// The DunGen XML reader.
		CDunGenXMLReader* DunGenXMLReader;

		/// The DunGen XML writer.
		CDunGenXMLWriter* DunGenXMLWriter;

		/// The irrlicht device.
		irr::IrrlichtDevice* IrrDevice;
	};

} // END NAMESPACE DunGen

#undef DllExport

#endif