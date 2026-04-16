// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#include "DungeonGenerator.h"
#include "Helperfunctions.h"
#include "LSystem.h"
#include "ObjectPlacement.h"
#include "MaterialProvider.h"
#include "MeshCave.h"
#include "RandomGenerator.h"
#include "Timer.h"
#include "VoxelCave.h"
#include <algorithm>
#include <iostream>

DunGen::CDungeonGenerator::CDungeonGenerator(irr::IrrlichtDevice* irrDevice_, CMaterialProvider* materialProvider_)
: RandomGenerator(new CRandomGenerator())
, LSystem(new CLSystem())
, VoxelCave(new CVoxelCave(RandomGenerator))
, MeshCave(new CMeshCave(VoxelCave,RandomGenerator))
, ObjectPlacement(new CObjectPlacement())
, Timer(new CTimer())
, FilteringEnabled(true)
, MaterialProvider(materialProvider_)
, MaterialCave(EMaterialType::CAVE_MULTICOLOR)
, MaterialCaveBackfaceCulling(true)
, MaterialCaveAntiAliasing(true)
, IrrDevice(irrDevice_)
, MonsterHitpoints(80)
, BossMonsterHitpoints(600)
, MonsterFireTime(5000)
, BossMonsterFireTime(1500)
, MonsterDamage(30)
, DescriptionIntro()
, DescriptionExtro()
, PrintToConsole(false)
{
}

DunGen::CDungeonGenerator::~CDungeonGenerator()
{
	delete Timer;
	delete ObjectPlacement;
	delete MeshCave;
	delete VoxelCave;
	delete LSystem;
	delete RandomGenerator;
}

DunGen::CRandomGenerator* DunGen::CDungeonGenerator::GetRandomGenerator()
{
	return RandomGenerator;
}

DunGen::CLSystem* DunGen::CDungeonGenerator::GetLSystem()
{
	return LSystem;
}

DunGen::CVoxelCave* DunGen::CDungeonGenerator::GetVoxelCave()
{
	return VoxelCave;
}

DunGen::CMeshCave* DunGen::CDungeonGenerator::GetMeshCave()
{
	return MeshCave;
}

DunGen::CObjectPlacement* DunGen::CDungeonGenerator::GetObjectPlacement()
{
	return ObjectPlacement;
}

void DunGen::CDungeonGenerator::GenerateCave()
{
	// Generate L-system.
	if (PrintToConsole)
	{
		std::cout << "[LSystem:] start creation of L-system derivation..." << std::endl;
		Timer->Start(0);
	}

	LSystem->GenerateDerivations();
	
	if (PrintToConsole)
	{
		std::cout << "[LSystem:] completed , ";
		Timer->Stop(0);
		std::cout << "[VoxelCave:] start creating voxel cave ..." << std::endl;
		Timer->Start(0);
	}

	// Draw the last derivation of the L-system in voxel space.
	VoxelCave->Draw(LSystem->GetHighestDerivation()
		, LSystem->GetParameter(ELSystemParameter::ANGLE_YAW)
		, LSystem->GetParameter(ELSystemParameter::ANGLE_PITCH)
		, LSystem->GetParameter(ELSystemParameter::ANGLE_ROLL)
		, LSystem->GetParameter(ELSystemParameter::RADIUS_START)
		, LSystem->GetParameter(ELSystemParameter::RADIUS_FACTOR)
		, LSystem->GetParameter(ELSystemParameter::RADIUS_DECREMENT));

	if (PrintToConsole)
	{
		std::cout << "[VoxelCave:] completed , ";
		Timer->Stop(0);
	}

	// Erode.
	for (unsigned int i=0; i < ErosionSteps.size(); ++i)
	{
		if (PrintToConsole)
		{
			std::cout << "[VoxelCave:] start eroding..." << std::endl;
			Timer->Start(0);
		}

		VoxelCave->Erode(ErosionSteps[i]);

		if (PrintToConsole)
		{
			std::cout << "[VoxelCave:] completed , ";
			Timer->Stop(0);
		}
	}

	// Filter.
	if (FilteringEnabled)
	{
		if (PrintToConsole)
		{
			std::cout << "[VoxelCave:] start removing hovering fragments..." << std::endl;
			Timer->Start(0);
		}

		unsigned int numRemovedVoxels = VoxelCave->Filter();

		if (PrintToConsole)
		{
			std::cout << "[VoxelCave:] completed , removed voxels = " << numRemovedVoxels << " , ";
			Timer->Stop(0);
		}
	}

	// Generate Mesh Cave.
	if (PrintToConsole)
	{
		std::cout << "[MeshCave:] start converting voxel cave to mesh cave..." << std::endl;
		Timer->Start(0);
	}

	MeshCave->CreateMeshFromVoxels();

	if (PrintToConsole)
	{
		std::cout << "[MeshCave:] completed , ";
		Timer->Stop(0);
	}
}

namespace
{
	float GetScale(const irr::core::aabbox3df& box_, unsigned int collisionRadius_)
	{
		irr::core::vector3df extend = 0.5f * box_.getExtent(); // half extend = some sort of radius
		irr::core::vector3df center = box_.getCenter();
		extend += irr::core::vector3df(abs(center.X),abs(center.Y),abs(center.Z)); // add center offset
		// (collisionradius - 1 voxel border) / (sqrt(3) * maxExtend)
		return static_cast<float>(collisionRadius_-1) / (1.732050807568877f * std::max(std::max(extend.X,extend.Y),extend.Z));
	}
}

void DunGen::CDungeonGenerator::AddDungeon(irr::scene::ISceneNode* parentNode_, irr::scene::ISceneManager* sceneManager_)
{
	if (PrintToConsole)
	{
		std::cout << "[DungeonGenerator:] start assembling of dungeon..." << std::endl;
		Timer->Start(0);
	}

	irr::scene::ISceneNode* dungeonRoot = sceneManager_->addEmptySceneNode(parentNode_);
	dungeonRoot->setName("DungeonRoot");

	// add the cave
	irr::scene::IMeshSceneNode* meshSceneNode = sceneManager_->addMeshSceneNode(MeshCave->GetMesh(), dungeonRoot, 0);
	meshSceneNode->setName("Cave");
	meshSceneNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, MaterialCaveBackfaceCulling);
	meshSceneNode->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, MaterialCaveAntiAliasing);
	meshSceneNode->setMaterialType(MaterialProvider->GetMaterial(MaterialCave));

	// add portal object
	SPortalObject portal = ObjectPlacement->GetPortalObject();
	if (portal.IsPlaced)
	{
		// create base portal sphere
		meshSceneNode = sceneManager_->addSphereSceneNode(
			static_cast<float>(CollisionRadius::Portal),16,parentNode_,0,
			vec3F(portal.Position),
			vec3F(portal.Rotation)
			);

		irr::video::ITexture* portalTexture = IrrDevice->getVideoDriver()->getTexture("data/Portal.jpg");
		meshSceneNode->setName("Portal");
		meshSceneNode->setMaterialTexture(0,portalTexture);
		meshSceneNode->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
		meshSceneNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		meshSceneNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
		meshSceneNode->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, MaterialCaveAntiAliasing);
		meshSceneNode->setVisible(false);

		// create animated portal out of base portal sphere
		irr::scene::ISceneNode* portalNode = sceneManager_->addWaterSurfaceSceneNode(
			meshSceneNode->getMesh(),				// mesh
			0.2f,									// wave height
			500.0f,									// wave speed
			0.4f,									// wave length
			parentNode_,							// parent node
			0,										// node id
			meshSceneNode->getPosition(),			// position
			meshSceneNode->getRotation(),			// rotation
			meshSceneNode->getScale()				// scale
			);
		portalNode->setName("Portal");

		// place player ship
		irr::scene::IMesh* mesh = sceneManager_->getMesh("data/WargXJ85.3DS");
		float scale = GetScale(mesh->getBoundingBox(), CollisionRadius::NormalShip);

		meshSceneNode = sceneManager_->addMeshSceneNode(mesh,parentNode_,0,
			vec3F(portal.Position),
			vec3F(portal.Rotation),
			irr::core::vector3df(scale,scale,scale)
			);
		meshSceneNode->setName("PlayerShip");
		meshSceneNode->setMaterialType(irr::video::EMT_SOLID);
		meshSceneNode->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		meshSceneNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, true);
		meshSceneNode->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, MaterialCaveAntiAliasing);
	}

	// add boss objects
	for (unsigned int i=0; i<EBossObject::NUMBER_OF_OBJECTS; ++i)
	{
		EBossObject::Enum type = static_cast<EBossObject::Enum>(i);
		SBossObject boss = ObjectPlacement->GetBossObject(type);
		if (boss.IsPlaced)
		{
			irr::scene::IMesh* mesh;
			int id;

			// boss
			switch (type)
			{
			case EBossObject::BOSS_01:
				mesh = sceneManager_->getMesh("data/Boss01.3DS");
				id = 1;
				break;
			case EBossObject::BOSS_02:
				mesh = sceneManager_->getMesh("data/Boss02.3DS");
				id = 2;
				break;
			case EBossObject::BOSS_03:
				mesh = sceneManager_->getMesh("data/Boss03.3DS");
				id = 3;
				break;
			case EBossObject::BOSS_04:
				mesh = sceneManager_->getMesh("data/Boss04.3DS");
				id = 4;
				break;
			case EBossObject::BOSS_05:
				mesh = sceneManager_->getMesh("data/Boss05.3DS");
				id = 5;
				break;
			default:
				mesh = sceneManager_->getMesh("data/Boss06.3DS");
				id = 6;
				break;
			}

			float scale = GetScale(mesh->getBoundingBox(), CollisionRadius::BossShip);
			meshSceneNode = sceneManager_->addMeshSceneNode(mesh,dungeonRoot,id,
				vec3F(boss.Position),
				irr::core::vector3df(0.0f,0.0f,0.0f),
				irr::core::vector3df(scale,scale,scale)
				);

			meshSceneNode->setName("BossShip");
			meshSceneNode->setMaterialType(irr::video::EMT_SOLID);
			meshSceneNode->setMaterialFlag(irr::video::EMF_LIGHTING, true);
			meshSceneNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, true);
			meshSceneNode->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, MaterialCaveAntiAliasing);
		}
	}

	// compute scalings for random objects
	float scaleRO[EObject::NO_SHIP];
	for (unsigned int i=0; i<EObject::NO_SHIP; ++i)
	{	
		irr::scene::IMesh* mesh;

		switch (i)
		{
		case EObject::SHIP_RED:
			mesh = sceneManager_->getMesh("data/ShipRed.3DS");
			break;
		case EObject::SHIP_GREEN:
			mesh = sceneManager_->getMesh("data/ShipGreen.3DS");
			break;
		case EObject::SHIP_BLUE:
			mesh = sceneManager_->getMesh("data/ShipBlue.3DS");
			break;
		case EObject::SHIP_YELLOW:
			mesh = sceneManager_->getMesh("data/ShipYellow.3DS");
			break;
		case EObject::SHIP_TEAL:
			mesh = sceneManager_->getMesh("data/ShipTeal.3DS");
			break;
		default:
			mesh = sceneManager_->getMesh("data/ShipPurple.3DS");
			break;
		}

		scaleRO[i] = GetScale(mesh->getBoundingBox(), CollisionRadius::NormalShip);
	}

	// add random objects
	std::vector<SPlacedObject> objects = ObjectPlacement->GetPlacedObjects();
	for (unsigned int i=0; i<objects.size(); ++i)
	{
		irr::scene::IMesh* mesh;
		int id;

		// ship
		switch (objects[i].Type)
		{
		case EObject::SHIP_RED:
			mesh = sceneManager_->getMesh("data/ShipRed.3DS");
			id = 1;
			break;
		case EObject::SHIP_GREEN:
			mesh = sceneManager_->getMesh("data/ShipGreen.3DS");
			id = 2;
			break;
		case EObject::SHIP_BLUE:
			mesh = sceneManager_->getMesh("data/ShipBlue.3DS");
			id = 3;
			break;
		case EObject::SHIP_YELLOW:
			mesh = sceneManager_->getMesh("data/ShipYellow.3DS");
			id = 4;
			break;
		case EObject::SHIP_TEAL:
			mesh = sceneManager_->getMesh("data/ShipTeal.3DS");
			id = 5;
			break;
		case EObject::SHIP_PURPLE:
			mesh = sceneManager_->getMesh("data/ShipPurple.3DS");
			id = 6;
			break;
		default:
			mesh = NULL;
			break;
		}

		if (mesh)
		{
			meshSceneNode = sceneManager_->addMeshSceneNode(mesh,dungeonRoot,id,
				vec3F(objects[i].Position),
				irr::core::vector3df(0.0f,0.0f,0.0f),
				irr::core::vector3df(scaleRO[objects[i].Type],scaleRO[objects[i].Type],scaleRO[objects[i].Type])
				);

			meshSceneNode->setName("Ship");
			meshSceneNode->setMaterialType(irr::video::EMT_SOLID);
			meshSceneNode->setMaterialFlag(irr::video::EMF_LIGHTING, true);
			meshSceneNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, true);
			meshSceneNode->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, MaterialCaveAntiAliasing);
		}
	}

	if (PrintToConsole)
	{
		std::cout << "[DungeonGenerator:] completed , ";
		Timer->Stop(0);
		std::cout << "[DungeonGenerator:] Dungeon was added to the desired node and scene." << std::endl;
	}
}

void DunGen::CDungeonGenerator::VoxelCaveAddErosionStep(double likelihood_)
{
	ErosionSteps.push_back(likelihood_);
}

void DunGen::CDungeonGenerator::VoxelCaveDeleteErosionSteps()
{
	ErosionSteps.clear();
}

std::vector<double> DunGen::CDungeonGenerator::VoxelCaveGetErosionSteps() const
{
	return ErosionSteps;
}

void DunGen::CDungeonGenerator::VoxelCaveSetFiltering(bool filtering_)
{
	FilteringEnabled = filtering_;
}

bool DunGen::CDungeonGenerator::VoxelCaveGetFiltering() const
{
	return FilteringEnabled;
}

void DunGen::CDungeonGenerator::MaterialSetCaveBackfaceCulling(bool backFaceCulling_)
{
	MaterialCaveBackfaceCulling = backFaceCulling_;
}

bool DunGen::CDungeonGenerator::MaterialGetCaveBackfaceCulling() const
{
	return MaterialCaveBackfaceCulling;
}

void DunGen::CDungeonGenerator::MaterialSetCaveAntiAliasing(bool antiAliasing_)
{
	MaterialCaveAntiAliasing = antiAliasing_;
}

bool DunGen::CDungeonGenerator::MaterialGetCaveAntiAliasing() const
{
	return MaterialCaveAntiAliasing;
}

void DunGen::CDungeonGenerator::MaterialSetCaveColor(const irr::video::SColorf& color_)
{
	MaterialProvider->SetCaveColor(color_);
}

irr::video::SColorf DunGen::CDungeonGenerator::MaterialGetCaveColor() const
{
	return MaterialProvider->GetCaveColor();
}

void DunGen::CDungeonGenerator::MaterialSetCaveMaterial(EMaterialType::Enum materialCave_, CMaterialDunGen* material_)
{
	MaterialProvider->SetMaterialCustom(material_);
	MaterialCave = materialCave_;
}

DunGen::EMaterialType::Enum DunGen::CDungeonGenerator::MaterialGetCaveMaterial() const
{
	return MaterialCave;
}

void DunGen::CDungeonGenerator::SetMonsterHitpoints(unsigned int val_)
{
	MonsterHitpoints = val_;
}

unsigned int DunGen::CDungeonGenerator::GetMonsterHitpoints() const
{
	return MonsterHitpoints;
}

void DunGen::CDungeonGenerator::SetBossMonsterHitpoints(unsigned int val_)
{
	BossMonsterHitpoints = val_;
}

unsigned int DunGen::CDungeonGenerator::GetBossMonsterHitpoints() const
{
	return BossMonsterHitpoints;
}

void DunGen::CDungeonGenerator::SetMonsterFireTime(unsigned int val_)
{
	MonsterFireTime = val_;
}

unsigned int DunGen::CDungeonGenerator::GetMonsterFireTime() const
{
	return MonsterFireTime;
}

void DunGen::CDungeonGenerator::SetBossMonsterFireTime(unsigned int val_)
{
	BossMonsterFireTime = val_;
}

unsigned int DunGen::CDungeonGenerator::GetBossMonsterFireTime() const
{
	return BossMonsterFireTime;
}

void DunGen::CDungeonGenerator::SetMonsterDamage(unsigned int val_)
{
	MonsterDamage = val_;
}

unsigned int DunGen::CDungeonGenerator::GetMonsterDamage() const
{
	return MonsterDamage;
}

void DunGen::CDungeonGenerator::SetDescriptionIntro(const std::wstring& val_)
{
	DescriptionIntro = val_;
}

std::wstring DunGen::CDungeonGenerator::GetDescriptionIntro() const
{
	return DescriptionIntro;
}

void DunGen::CDungeonGenerator::SetDescriptionExtro(const std::wstring& val_)
{
	DescriptionExtro = val_;
}

std::wstring DunGen::CDungeonGenerator::GetDescriptionExtro() const
{
	return DescriptionExtro;
}

void DunGen::CDungeonGenerator::SetPrintToConsole(bool enabled_)
{
	PrintToConsole = enabled_;
	VoxelCave->SetPrintToConsole(enabled_);
	MeshCave->SetPrintToConsole(enabled_);
}
