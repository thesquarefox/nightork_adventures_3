// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#include "interface/DunGen.h"
#include "DunGenXMLReader.h"
#include "DunGenXMLWriter.h"
#include "DungeonGenerator.h"
#include "LSystem.h"
#include "MaterialProvider.h"
#include "MeshCave.h"
#include "ObjectPlacement.h"
#include "RandomGenerator.h"
#include "VoxelCave.h"

DunGen::CDunGen::CDunGen(irr::IrrlichtDevice* irrDevice)
: DungeonGenerator(NULL)
, DunGenXMLReader(NULL)
, DunGenXMLWriter(NULL)
, MaterialProvider(new CMaterialProvider(irrDevice))
, IrrDevice(irrDevice)
{
}

DunGen::CDunGen::~CDunGen()
{
	ShutDown();
	delete MaterialProvider;
}

void DunGen::CDunGen::StartUp()
{
	if (!DungeonGenerator)
		DungeonGenerator = new CDungeonGenerator(IrrDevice, MaterialProvider);
	if (!DunGenXMLReader)
		DunGenXMLReader = new CDunGenXMLReader(IrrDevice, this);
	if (!DunGenXMLWriter)
		DunGenXMLWriter = new CDunGenXMLWriter(IrrDevice, this);
}

void DunGen::CDunGen::ShutDown()
{
	delete DunGenXMLWriter;
	DunGenXMLWriter = NULL;
	delete DunGenXMLReader;
	DunGenXMLReader = NULL;
	delete DungeonGenerator;
	DungeonGenerator = NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// generation
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void DunGen::CDunGen::GenerateCave()
{
	if (DungeonGenerator)
		return DungeonGenerator->GenerateCave();
}

bool DunGen::CDunGen::ReadDungeonFromFile(const irr::io::path& filename)
{
	if (DunGenXMLReader)
		return DunGenXMLReader->Read(filename);
	else
		return false;
}

bool DunGen::CDunGen::WriteDungeonToFile(const irr::io::path& filename)
{
	if (DunGenXMLWriter)
		return DunGenXMLWriter->Write(filename);
	else
		return false;
}

void DunGen::CDunGen::AddDungeon(irr::scene::ISceneNode* parentNode, irr::scene::ISceneManager* sceneManager)
{
	if (DungeonGenerator)
		DungeonGenerator->AddDungeon(parentNode, sceneManager);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// parameters: random generator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void DunGen::CDunGen::RandomGeneratorSetSeed(unsigned int seed)
{
	if (DungeonGenerator)
		DungeonGenerator->GetRandomGenerator()->SetSeed(seed);
}

unsigned int DunGen::CDunGen::RandomGeneratorGetSeed() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetRandomGenerator()->GetSeed();
	return 0;
}

void DunGen::CDunGen::RandomGeneratorSetA(unsigned int a)
{
	if (DungeonGenerator)
		DungeonGenerator->GetRandomGenerator()->SetA(a);
}

unsigned int DunGen::CDunGen::RandomGeneratorGetA() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetRandomGenerator()->GetA();
	return 0;
}

void DunGen::CDunGen::RandomGeneratorSetC(unsigned int c)
{
	if (DungeonGenerator)
		DungeonGenerator->GetRandomGenerator()->SetC(c);
}

unsigned int DunGen::CDunGen::RandomGeneratorGetC() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetRandomGenerator()->GetC();
	return 0;
}

void DunGen::CDunGen::RandomGeneratorSetM(unsigned int m)
{
	if (DungeonGenerator)
		DungeonGenerator->GetRandomGenerator()->SetM(m);
}

unsigned int DunGen::CDunGen::RandomGeneratorGetM() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetRandomGenerator()->GetM();
	return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// parameters: L-system
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void DunGen::CDunGen::LSystemSetIteration(unsigned int iteration)
{
	if (DungeonGenerator)
		DungeonGenerator->GetLSystem()->SetIteration(iteration);
}

unsigned int DunGen::CDunGen::LSystemGetIteration() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetLSystem()->GetIteration();
	return 0;
}

void DunGen::CDunGen::LSystemSetStart(const std::string& start)
{
	if (DungeonGenerator)
		DungeonGenerator->GetLSystem()->SetStart(start);
}

std::string DunGen::CDunGen::LSystemGetStart() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetLSystem()->GetStart();
	return std::string();
}

void DunGen::CDunGen::LSystemSetParameter(ELSystemParameter::Enum parameter, double value)
{
	if (DungeonGenerator)
		DungeonGenerator->GetLSystem()->SetParameter(parameter, value);
}

double DunGen::CDunGen::LSystemGetParameter(ELSystemParameter::Enum parameter) const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetLSystem()->GetParameter(parameter);
	return 0.0;
}

void DunGen::CDunGen::LSystemAddRule(const SSubstitutionRule& rule)
{
	if (DungeonGenerator)
		DungeonGenerator->GetLSystem()->AddRule(rule);
}

void DunGen::CDunGen::LSystemDeleteRules()
{
	if (DungeonGenerator)
		DungeonGenerator->GetLSystem()->DeleteRules();
}

std::vector<DunGen::SSubstitutionRule> DunGen::CDunGen::LSystemGetRules() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetLSystem()->GetRules();
	return std::vector<SSubstitutionRule>();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// parameters: voxel cave
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void DunGen::CDunGen::VoxelCaveSetBorder( unsigned int border )
{
	if (DungeonGenerator)
		DungeonGenerator->GetVoxelCave()->SetBorder(border);
}

unsigned int DunGen::CDunGen::VoxelCaveGetBorder() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetVoxelCave()->GetBorder();
	return 0;
}

void DunGen::CDunGen::VoxelCaveSetMinDrawRadius( unsigned int mindrawradius )
{
	if (DungeonGenerator)
		DungeonGenerator->GetVoxelCave()->SetMinDrawRadius(mindrawradius);
}

unsigned int DunGen::CDunGen::VoxelCaveGetMinDrawRadius() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetVoxelCave()->GetMinDrawRadius();
	return 0;
}


void DunGen::CDunGen::VoxelCaveAddErosionStep(double likelihood)
{
	if (DungeonGenerator)
		DungeonGenerator->VoxelCaveAddErosionStep(likelihood);
}

void DunGen::CDunGen::VoxelCaveDeleteErosionSteps()
{
	if (DungeonGenerator)
		DungeonGenerator->VoxelCaveDeleteErosionSteps();
}

std::vector<double> DunGen::CDunGen::VoxelCaveGetErosionSteps() const
{
	if (DungeonGenerator)
		return DungeonGenerator->VoxelCaveGetErosionSteps();
	return std::vector<double>();
}

void DunGen::CDunGen::VoxelCaveSetFiltering(bool filtering)
{
	if (DungeonGenerator)
		DungeonGenerator->VoxelCaveSetFiltering(filtering);
}

bool DunGen::CDunGen::VoxelCaveGetFiltering() const
{
	if (DungeonGenerator)
		return DungeonGenerator->VoxelCaveGetFiltering();
	return false;
}

unsigned int DunGen::CDunGen::VoxelCaveEstimateMeshComplexity() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetVoxelCave()->EstimateMeshComplexity();
	else
		return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// parameters: mesh cave
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void DunGen::CDunGen::MeshCaveSetWarpEnabled(bool warpEnabled)
{
	if (DungeonGenerator)
		DungeonGenerator->GetMeshCave()->SetWarpOption(warpEnabled);
}

bool DunGen::CDunGen::MeshCaveGetWarpEnabled() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetMeshCave()->GetWarpOption();
	return false;
}

void DunGen::CDunGen::MeshCaveSetSmoothEnabled(bool smoothEnabled)
{
	if (DungeonGenerator)
		DungeonGenerator->GetMeshCave()->SetSmoothOption(smoothEnabled);
}

bool DunGen::CDunGen::MeshCaveGetSmoothEnabled() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetMeshCave()->GetSmoothOption();
	return false;
}

void DunGen::CDunGen::MeshCaveSetWarpStrength(double warpStrength)
{
	if (DungeonGenerator)
		DungeonGenerator->GetMeshCave()->SetWarpStrength(warpStrength);
}

double DunGen::CDunGen::MeshCaveGetWarpStrength() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetMeshCave()->GetWarpStrength();
	return 0.0;
}

void DunGen::CDunGen::MeshCaveSetWarpRandomSeed(unsigned int warpRandomSeed)
{
	if (DungeonGenerator)
		DungeonGenerator->GetMeshCave()->SetWarpRandomSeed(warpRandomSeed);
}

unsigned int DunGen::CDunGen::MeshCaveGetWarpRandomSeed() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetMeshCave()->GetWarpRandomSeed();
	return 0;
}

void DunGen::CDunGen::MeshCaveSetNormalWeightMethod(ENormalWeightMethod::Enum value)
{
	if (DungeonGenerator)
		DungeonGenerator->GetMeshCave()->SetNormalWeightMethod(value);
}

DunGen::ENormalWeightMethod::Enum DunGen::CDunGen::MeshCaveGetNormalWeightMethod() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetMeshCave()->GetNormalWeightMethod();
	return ENormalWeightMethod::BY_AREA;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// parameters: materials
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void DunGen::CDunGen::MaterialSetCaveBackfaceCulling(bool backFaceCulling)
{
	if (DungeonGenerator)
		DungeonGenerator->MaterialSetCaveBackfaceCulling(backFaceCulling);
}

bool DunGen::CDunGen::MaterialGetCaveBackfaceCulling() const
{
	if (DungeonGenerator)
		return DungeonGenerator->MaterialGetCaveBackfaceCulling();
	return false;
}

void DunGen::CDunGen::MaterialSetCaveAntiAliasing(bool antiAliasing)
{
	if (DungeonGenerator)
		DungeonGenerator->MaterialSetCaveAntiAliasing(antiAliasing);
}

bool DunGen::CDunGen::MaterialGetCaveAntiAliasing() const
{
	if (DungeonGenerator)
		return DungeonGenerator->MaterialGetCaveAntiAliasing();
	return false;
}

void DunGen::CDunGen::MaterialSetCaveColor(const irr::video::SColorf& color)
{
	if (DungeonGenerator)
		DungeonGenerator->MaterialSetCaveColor(color);
}

irr::video::SColorf DunGen::CDunGen::MaterialGetCaveColor() const
{
	if (DungeonGenerator)
		return DungeonGenerator->MaterialGetCaveColor();
	return irr::video::SColorf();
}

void DunGen::CDunGen::MaterialSetCaveMaterial(EMaterialType::Enum materialCave, CMaterialDunGen* material)
{
	if (DungeonGenerator)
		DungeonGenerator->MaterialSetCaveMaterial(materialCave,material);
}

DunGen::EMaterialType::Enum DunGen::CDunGen::MaterialGetCaveMaterial() const
{
	if (DungeonGenerator)
		return DungeonGenerator->MaterialGetCaveMaterial();
	return EMaterialType::CAVE_MULTICOLOR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// parameters: objects
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void DunGen::CDunGen::ObjectsDelete()
{
	if (DungeonGenerator)
		DungeonGenerator->GetObjectPlacement()->DeleteObjects();
}

void DunGen::CDunGen::ObjectsDeleteStatic()
{
	if (DungeonGenerator)
		DungeonGenerator->GetObjectPlacement()->DeleteStaticObjects();
}

void DunGen::CDunGen::ObjectsAddObject(const SPlacedObject& object)
{
	if (DungeonGenerator)
		DungeonGenerator->GetObjectPlacement()->AddObject(object);
}

std::vector<DunGen::SPlacedObject> DunGen::CDunGen::ObjectsGetObjects()
{
	if (DungeonGenerator)
		return DungeonGenerator->GetObjectPlacement()->GetPlacedObjects();
	return std::vector<SPlacedObject>();
}

void DunGen::CDunGen::ObjectsSetBossObject(EBossObject::Enum type, const SBossObject& object)
{
	if (DungeonGenerator)
		DungeonGenerator->GetObjectPlacement()->SetBossObject(type,object);
}

DunGen::SBossObject DunGen::CDunGen::ObjectsGetBossObject(EBossObject::Enum type) const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetObjectPlacement()->GetBossObject(type);
	SBossObject tmp;
	tmp.Position = irr::core::vector3d<double>(0.0,0.0,0.0);
	tmp.IsPlaced = false;
	return tmp;
}

void DunGen::CDunGen::ObjectsSetPortalObject(const SPortalObject& object)
{
	if (DungeonGenerator)
		DungeonGenerator->GetObjectPlacement()->SetPortalObject(object);
}

DunGen::SPortalObject DunGen::CDunGen::ObjectsGetPortalObject() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetObjectPlacement()->GetPortalObject();
	SPortalObject tmp;
	tmp.Position = irr::core::vector3d<double>(0.0,0.0,0.0);
	tmp.Rotation = irr::core::vector3d<double>(0.0,0.0,0.0);
	tmp.IsPlaced = false;
	return tmp;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// parameters: gameplay
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void DunGen::CDunGen::SetMonsterHitpoints(unsigned int val)
{
	if (DungeonGenerator)
		DungeonGenerator->SetMonsterHitpoints(val);
}

unsigned int DunGen::CDunGen::GetMonsterHitpoints() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetMonsterHitpoints();
	return 80;
}

void DunGen::CDunGen::SetBossMonsterHitpoints(unsigned int val)
{
	if (DungeonGenerator)
		DungeonGenerator->SetBossMonsterHitpoints(val);
}

unsigned int DunGen::CDunGen::GetBossMonsterHitpoints() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetBossMonsterHitpoints();
	return 600;
}

void DunGen::CDunGen::SetMonsterFireTime(unsigned int val)
{
	if (DungeonGenerator)
		DungeonGenerator->SetMonsterFireTime(val);
}

unsigned int DunGen::CDunGen::GetMonsterFireTime() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetMonsterFireTime();
	return 5000;
}

void DunGen::CDunGen::SetBossMonsterFireTime(unsigned int val)
{
	if (DungeonGenerator)
		DungeonGenerator->SetBossMonsterFireTime(val);
}

unsigned int DunGen::CDunGen::GetBossMonsterFireTime() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetBossMonsterFireTime();
	return 1500;
}

void DunGen::CDunGen::SetMonsterDamage(unsigned int val)
{
	if (DungeonGenerator)
		DungeonGenerator->SetMonsterDamage(val);
}

unsigned int DunGen::CDunGen::GetMonsterDamage() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetMonsterDamage();
	return 30;
}

void DunGen::CDunGen::SetDescriptionIntro(const std::wstring& val)
{
	if (DungeonGenerator)
		DungeonGenerator->SetDescriptionIntro(val);
}

std::wstring DunGen::CDunGen::GetDescriptionIntro() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetDescriptionIntro();
	return std::wstring();
}

void DunGen::CDunGen::SetDescriptionExtro(const std::wstring& val)
{
	if (DungeonGenerator)
		DungeonGenerator->SetDescriptionExtro(val);
}

std::wstring DunGen::CDunGen::GetDescriptionExtro() const
{
	if (DungeonGenerator)
		return DungeonGenerator->GetDescriptionExtro();
	return std::wstring();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// other
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void DunGen::CDunGen::SetPrintToConsole(bool value)
{
	if (DungeonGenerator)
		DungeonGenerator->SetPrintToConsole(value);
}

unsigned char (&DunGen::CDunGen::GetVoxelSpace())[SVoxelSpace::DimX][SVoxelSpace::DimY][SVoxelSpace::DimZ]
{
	// no test for DungeonGenerator here, what shall we return if it doesn't exist?
	return DungeonGenerator->GetVoxelCave()->GetVoxelSpace();
}
