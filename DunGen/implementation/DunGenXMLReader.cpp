// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#include "interface/DunGen.h"
#include "DunGenXMLReader.h"
#include <iostream>
#include <string>

DunGen::CDunGenXMLReader::CDunGenXMLReader(irr::IrrlichtDevice* irrDevice_, CDunGen* dunGen_)
	: FileSystem(irrDevice_->getFileSystem())
	, DunGenInterface(dunGen_)
	, File(NULL)
	, XmlReader(NULL)
{
}

DunGen::CDunGenXMLReader::~CDunGenXMLReader()
{
}

bool DunGen::CDunGenXMLReader::Read(const irr::io::path& filename_)
{
	std::cout << "[DunGen:] Reading " << filename_.c_str();

	// get read access to file
	File = FileSystem->createAndOpenFile(filename_);

	if (!File)
	{
		std::cout << " failed." << std::endl;
		return false;
	}

	XmlReader = FileSystem->createXMLReader(File);

	// clear previous content
	DunGenInterface->LSystemDeleteRules();
	DunGenInterface->VoxelCaveDeleteErosionSteps();
	DunGenInterface->ObjectsDelete();
	DunGenInterface->ObjectsDeleteStatic();

	// process file content
	bool dunGenPart = false;
	while(XmlReader->read())
	{
		// is this part of DunGen XML?
		if (XmlReader->getNodeType() == irr::io::EXN_ELEMENT && irr::core::stringw("DunGen") == XmlReader->getNodeName())
		{
			dunGenPart = true;
		}
		else if (XmlReader->getNodeType() == irr::io::EXN_ELEMENT_END && irr::core::stringw("DunGen") == XmlReader->getNodeName())
		{
			dunGenPart = false;
		}

		// delegate XML reading, depending on tag
		if (dunGenPart && XmlReader->getNodeType() == irr::io::EXN_ELEMENT)
		{
			if (irr::core::stringw("Materials") == XmlReader->getNodeName())
				ReadMaterials();
			else if (irr::core::stringw("RandomGenerator") == XmlReader->getNodeName())
				ReadRandomGenerator();
			else if (irr::core::stringw("VoxelCave") == XmlReader->getNodeName())
				ReadVoxelCave();
			else if (irr::core::stringw("Erode") == XmlReader->getNodeName())
				ReadErode();
			else if (irr::core::stringw("Filter") == XmlReader->getNodeName())
				ReadFilter();
			else if (irr::core::stringw("MeshCave") == XmlReader->getNodeName())
				ReadMeshCave();
			else if (irr::core::stringw("Gameplay") == XmlReader->getNodeName())
				ReadGameplay();
			else if (irr::core::stringw("Description") == XmlReader->getNodeName())
				ReadDescription();
			else if (irr::core::stringw("PortalObject") == XmlReader->getNodeName())
				ReadPortalObject();
			else if (irr::core::stringw("BossObjects") == XmlReader->getNodeName())
				ReadBossObjects();
			else if (irr::core::stringw("Objects") == XmlReader->getNodeName())
				ReadObjects();
		}
	}

	// close file
	XmlReader->drop();
	File->drop();

	std::cout << " succeeded." << std::endl;

	DunGenInterface->GenerateCave();
	return true;
}

void DunGen::CDunGenXMLReader::ReadMaterials()
{
	DunGenInterface->MaterialSetCaveBackfaceCulling(irr::core::stringw("0") != XmlReader->getAttributeValue(L"BackfaceCulling"));
	DunGenInterface->MaterialSetCaveAntiAliasing(irr::core::stringw("0") != XmlReader->getAttributeValue(L"AntiAliasing"));

	while(XmlReader->read() && (XmlReader->getNodeType() != irr::io::EXN_ELEMENT_END || irr::core::stringw("Materials") != XmlReader->getNodeName()) )
	{
		if (XmlReader->getNodeType() == irr::io::EXN_ELEMENT)
		{
			// read cave material parameters
			if (irr::core::stringw("Cave") == XmlReader->getNodeName())
			{
				if (irr::core::stringw("SingleColor") == XmlReader->getAttributeValue(L"Shader"))
				{
					float red = XmlReader->getAttributeValueAsFloat(L"Red");
					float green = XmlReader->getAttributeValueAsFloat(L"Green");
					float blue = XmlReader->getAttributeValueAsFloat(L"Blue");
					DunGenInterface->MaterialSetCaveColor(irr::video::SColorf(red,green,blue));
					DunGenInterface->MaterialSetCaveMaterial(EMaterialType::CAVE_SINGLECOLOR, NULL);
				}
				else
				{
					DunGenInterface->MaterialSetCaveMaterial(EMaterialType::CAVE_MULTICOLOR, NULL);
				}
			}
		}
	}
}

void DunGen::CDunGenXMLReader::ReadRandomGenerator()
{
	DunGenInterface->RandomGeneratorSetSeed(XmlReader->getAttributeValueAsInt(L"Seed"));
	DunGenInterface->RandomGeneratorSetA(XmlReader->getAttributeValueAsInt(L"A"));
	DunGenInterface->RandomGeneratorSetC(XmlReader->getAttributeValueAsInt(L"C"));
	DunGenInterface->RandomGeneratorSetM(XmlReader->getAttributeValueAsInt(L"M"));
}

void DunGen::CDunGenXMLReader::ReadVoxelCave()
{
	irr::core::stringc tmpString = XmlReader->getAttributeValue(L"StartString");
	DunGenInterface->LSystemSetStart(tmpString.c_str());

	DunGenInterface->LSystemSetParameter(ELSystemParameter::RADIUS_START, XmlReader->getAttributeValueAsFloat(L"StartRadius"));
	DunGenInterface->LSystemSetIteration(XmlReader->getAttributeValueAsInt(L"Derivation"));

	while(XmlReader->read() && (XmlReader->getNodeType() != irr::io::EXN_ELEMENT_END || irr::core::stringw("VoxelCave") != XmlReader->getNodeName()) )
	{
		if (XmlReader->getNodeType() == irr::io::EXN_ELEMENT)
		{
			if (irr::core::stringw("Basic") == XmlReader->getNodeName())
			{
				DunGenInterface->VoxelCaveSetBorder(XmlReader->getAttributeValueAsInt(L"VoxelBorder"));
				DunGenInterface->VoxelCaveSetMinDrawRadius(XmlReader->getAttributeValueAsInt(L"MinDrawRadius"));
			}
			else if (irr::core::stringw("Radius") == XmlReader->getNodeName())
			{
				DunGenInterface->LSystemSetParameter(ELSystemParameter::RADIUS_FACTOR,  XmlReader->getAttributeValueAsFloat(L"RadiusFactor"));
				DunGenInterface->LSystemSetParameter(ELSystemParameter::RADIUS_DECREMENT, XmlReader->getAttributeValueAsFloat(L"RadiusDecrement"));
			}
			else if (irr::core::stringw("Angle") == XmlReader->getNodeName())
			{
				DunGenInterface->LSystemSetParameter(ELSystemParameter::ANGLE_YAW, XmlReader->getAttributeValueAsFloat(L"Yaw"));
				DunGenInterface->LSystemSetParameter(ELSystemParameter::ANGLE_PITCH, XmlReader->getAttributeValueAsFloat(L"Pitch"));
				DunGenInterface->LSystemSetParameter(ELSystemParameter::ANGLE_ROLL, XmlReader->getAttributeValueAsFloat(L"Roll"));
			}
			else if (irr::core::stringw("Rule") == XmlReader->getNodeName())
			{
				SSubstitutionRule rule;
				tmpString = XmlReader->getAttributeValue(L"Symbol");
				rule.Symbol = tmpString[0];
				rule.Substitution = irr::core::stringc(XmlReader->getAttributeValue(L"Substitution")).c_str();

				DunGenInterface->LSystemAddRule(rule);
			}
		}
	}
}

void DunGen::CDunGenXMLReader::ReadErode()
{
	DunGenInterface->VoxelCaveAddErosionStep(XmlReader->getAttributeValueAsFloat(L"Likelihood"));	
}

void DunGen::CDunGenXMLReader::ReadFilter()
{
	DunGenInterface->VoxelCaveSetFiltering(irr::core::stringw("0") != XmlReader->getAttributeValue(L"Enabled"));
}

void DunGen::CDunGenXMLReader::ReadMeshCave()
{
	DunGenInterface->MeshCaveSetWarpEnabled(irr::core::stringw("0") != XmlReader->getAttributeValue(L"Warping"));
	DunGenInterface->MeshCaveSetSmoothEnabled(irr::core::stringw("0") != XmlReader->getAttributeValue(L"Smoothing"));
	DunGenInterface->MeshCaveSetWarpStrength(XmlReader->getAttributeValueAsFloat(L"WarpStrength"));
	DunGenInterface->MeshCaveSetWarpRandomSeed(XmlReader->getAttributeValueAsInt(L"WarpRandomSeed"));
	DunGenInterface->MeshCaveSetNormalWeightMethod(static_cast<DunGen::ENormalWeightMethod::Enum>(
		XmlReader->getAttributeValueAsInt(L"NormalWeighting") ));
}

void DunGen::CDunGenXMLReader::ReadGameplay()
{
	DunGenInterface->SetMonsterHitpoints(XmlReader->getAttributeValueAsInt(L"MonsterHitpoints"));
	DunGenInterface->SetBossMonsterHitpoints(XmlReader->getAttributeValueAsInt(L"BossMonsterHitpoints"));
	DunGenInterface->SetMonsterFireTime(XmlReader->getAttributeValueAsInt(L"MonsterFireTime"));
	DunGenInterface->SetBossMonsterFireTime(XmlReader->getAttributeValueAsInt(L"BossMonsterFireTime"));
	DunGenInterface->SetMonsterDamage(XmlReader->getAttributeValueAsInt(L"MonsterDamage"));
}

void DunGen::CDunGenXMLReader::ReadDescription()
{
	DunGenInterface->SetDescriptionIntro(XmlReader->getAttributeValue(L"Intro"));
	DunGenInterface->SetDescriptionExtro(XmlReader->getAttributeValue(L"Extro"));
}

void DunGen::CDunGenXMLReader::ReadPortalObject()
{
	SPortalObject portal;
	portal.IsPlaced = (irr::core::stringw("0") != XmlReader->getAttributeValue(L"IsPlaced"));

	while(XmlReader->read() && (XmlReader->getNodeType() != irr::io::EXN_ELEMENT_END || irr::core::stringw("PortalObject") != XmlReader->getNodeName()) )
	{
		if (XmlReader->getNodeType() == irr::io::EXN_ELEMENT)
		{
			// read object parameters
			if (irr::core::stringw("Position") == XmlReader->getNodeName())
			{
				portal.Position.X = XmlReader->getAttributeValueAsFloat(L"X");
				portal.Position.Y = XmlReader->getAttributeValueAsFloat(L"Y");
				portal.Position.Z = XmlReader->getAttributeValueAsFloat(L"Z");
			}
			else if (irr::core::stringw("Rotation") == XmlReader->getNodeName())
			{
				portal.Rotation.X = XmlReader->getAttributeValueAsFloat(L"X");
				portal.Rotation.Y = XmlReader->getAttributeValueAsFloat(L"Y");
				portal.Rotation.Z = XmlReader->getAttributeValueAsFloat(L"Z");
			}
		}
	}

	DunGenInterface->ObjectsSetPortalObject(portal);
}

void DunGen::CDunGenXMLReader::ReadBossObjects()
{
	while(XmlReader->read() && (XmlReader->getNodeType() != irr::io::EXN_ELEMENT_END || irr::core::stringw("BossObjects") != XmlReader->getNodeName()) )
	{
		if (XmlReader->getNodeType() == irr::io::EXN_ELEMENT)
		{
			// read object parameters
			if (irr::core::stringw("Boss") == XmlReader->getNodeName())
			{
				SBossObject boss;
				EBossObject::Enum type = static_cast<EBossObject::Enum>(XmlReader->getAttributeValueAsInt(L"Type"));
				boss.Position.X = XmlReader->getAttributeValueAsFloat(L"X");
				boss.Position.Y = XmlReader->getAttributeValueAsFloat(L"Y");
				boss.Position.Z = XmlReader->getAttributeValueAsFloat(L"Z");
				boss.IsPlaced = true;
				DunGenInterface->ObjectsSetBossObject(type,boss);
			}
		}
	}
}

void DunGen::CDunGenXMLReader::ReadObjects()
{
	while(XmlReader->read() && (XmlReader->getNodeType() != irr::io::EXN_ELEMENT_END || irr::core::stringw("Objects") != XmlReader->getNodeName()) )
	{
		if (XmlReader->getNodeType() == irr::io::EXN_ELEMENT)
		{
			// read object parameters
			if (irr::core::stringw("Ship") == XmlReader->getNodeName())
			{
				SPlacedObject object;
				object.Type = static_cast<EObject::Enum>(XmlReader->getAttributeValueAsInt(L"Type"));
				object.Position.X = XmlReader->getAttributeValueAsFloat(L"X");
				object.Position.Y = XmlReader->getAttributeValueAsFloat(L"Y");
				object.Position.Z = XmlReader->getAttributeValueAsFloat(L"Z");
				DunGenInterface->ObjectsAddObject(object);
			}
		}
	}
}
