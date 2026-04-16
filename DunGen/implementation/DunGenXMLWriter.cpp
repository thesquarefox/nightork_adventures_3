// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#include "interface/DunGen.h"
#include "DunGenXMLWriter.h"
#include <iostream>
#include <string>

DunGen::CDunGenXMLWriter::CDunGenXMLWriter(irr::IrrlichtDevice* irrDevice_, CDunGen* dunGen_)
	: FileSystem(irrDevice_->getFileSystem())
	, DunGenInterface(dunGen_)
	, File(NULL)
	, XmlWriter(NULL)
{
}

DunGen::CDunGenXMLWriter::~CDunGenXMLWriter()
{
}

bool DunGen::CDunGenXMLWriter::Write(const irr::io::path& filename_)
{
	std::cout << "[DunGen:] Writing " << filename_.c_str();

	// get write access to file
	File = FileSystem->createAndWriteFile(filename_);

	if (!File)
	{
		std::cout << " failed." << std::endl;
		return false;
	}

	XmlWriter = FileSystem->createXMLWriter(File);

	// write file content
	XmlWriter->writeXMLHeader();
	XmlWriter->writeLineBreak();

	XmlWriter->writeElement(L"DunGen",false);
	XmlWriter->writeLineBreak();

	// delegate XML writing, depending on tag
	WriteMaterials();
	WriteRandomGenerator();
	WriteVoxelCave();
	WriteErode();
	WriteFilter();
	WriteMeshCave();

	WriteGameplay();
	WriteDescription();
	WritePortalObject();
	WriteBossObjects();
	WriteObjects();

	XmlWriter->writeClosingTag(L"DunGen");
	XmlWriter->writeLineBreak();

	// close file
	XmlWriter->drop();
	File->drop();

	std::cout << " succeeded." << std::endl;

	return true;
}

void DunGen::CDunGenXMLWriter::WriteMaterials()
{
	XmlWriter->writeElement(L"Materials",false,
		L"BackfaceCulling", DunGenInterface->MaterialGetCaveBackfaceCulling() ? L"1" : L"0",
		L"AntiAliasing", DunGenInterface->MaterialGetCaveAntiAliasing() ? L"1" : L"0");
	XmlWriter->writeLineBreak();

	irr::video::SColorf color = DunGenInterface->MaterialGetCaveColor();
	XmlWriter->writeElement(L"Cave",true,
		L"Shader", EMaterialType::CAVE_SINGLECOLOR == DunGenInterface->MaterialGetCaveMaterial() ? L"SingleColor" : L"MultiColor",
		L"Red",irr::core::stringw(color.r).c_str(),
		L"Green",irr::core::stringw(color.g).c_str(),
		L"Blue",irr::core::stringw(color.b).c_str());
	XmlWriter->writeLineBreak();

	XmlWriter->writeClosingTag(L"Materials");
	XmlWriter->writeLineBreak();
}

void DunGen::CDunGenXMLWriter::WriteRandomGenerator()
{
	XmlWriter->writeElement(L"RandomGenerator",true,
		L"Seed", irr::core::stringw(DunGenInterface->RandomGeneratorGetSeed()).c_str(),
		L"A", irr::core::stringw(DunGenInterface->RandomGeneratorGetA()).c_str(),
		L"C", irr::core::stringw(DunGenInterface->RandomGeneratorGetC()).c_str(),
		L"M", irr::core::stringw(DunGenInterface->RandomGeneratorGetM()).c_str());
	XmlWriter->writeLineBreak();
}

void DunGen::CDunGenXMLWriter::WriteVoxelCave()
{
	XmlWriter->writeElement(L"VoxelCave",false,
		L"StartString",irr::core::stringw(DunGenInterface->LSystemGetStart().c_str()).c_str(),
		L"StartRadius",irr::core::stringw(DunGenInterface->LSystemGetParameter(ELSystemParameter::RADIUS_START)).c_str(),
		L"Derivation",irr::core::stringw(DunGenInterface->LSystemGetIteration()).c_str());
	XmlWriter->writeLineBreak();

	XmlWriter->writeElement(L"Basic",true,
		L"VoxelBorder",irr::core::stringw(DunGenInterface->VoxelCaveGetBorder()).c_str(),
		L"MinDrawRadius",irr::core::stringw(DunGenInterface->VoxelCaveGetMinDrawRadius()).c_str());
	XmlWriter->writeLineBreak();

	XmlWriter->writeElement(L"Radius",true,
		L"RadiusFactor",irr::core::stringw(DunGenInterface->LSystemGetParameter(ELSystemParameter::RADIUS_FACTOR)).c_str(),
		L"RadiusDecrement",irr::core::stringw(DunGenInterface->LSystemGetParameter(ELSystemParameter::RADIUS_DECREMENT)).c_str());
	XmlWriter->writeLineBreak();

	XmlWriter->writeElement(L"Angle",true,
		L"Yaw",irr::core::stringw(DunGenInterface->LSystemGetParameter(ELSystemParameter::ANGLE_YAW)).c_str(),
		L"Pitch",irr::core::stringw(DunGenInterface->LSystemGetParameter(ELSystemParameter::ANGLE_PITCH)).c_str(),
		L"Roll",irr::core::stringw(DunGenInterface->LSystemGetParameter(ELSystemParameter::ANGLE_ROLL)).c_str());
	XmlWriter->writeLineBreak();

	std::vector<SSubstitutionRule> rules = DunGenInterface->LSystemGetRules();
	std::string tmpString = "0";
	for (unsigned int i=0; i<rules.size(); ++i)
	{
		tmpString[0] = rules[i].Symbol;
		XmlWriter->writeElement(L"Rule",true,
			L"Symbol",irr::core::stringw(tmpString.c_str()).c_str(),
			L"Substitution",irr::core::stringw(rules[i].Substitution.c_str()).c_str()),
			XmlWriter->writeLineBreak();
	}

	XmlWriter->writeClosingTag(L"VoxelCave");
	XmlWriter->writeLineBreak();
}

void DunGen::CDunGenXMLWriter::WriteErode()
{
	std::vector<double> erosionSteps = DunGenInterface->VoxelCaveGetErosionSteps();
	for (unsigned int i=0; i<erosionSteps.size(); ++i)
	{
		XmlWriter->writeElement(L"Erode",true,
			L"Likelihood", irr::core::stringw(erosionSteps[i]).c_str());
		XmlWriter->writeLineBreak();
	}
}

void DunGen::CDunGenXMLWriter::WriteFilter()
{
	XmlWriter->writeElement(L"Filter",true,
		L"Enabled", DunGenInterface->VoxelCaveGetFiltering() ? L"1" : L"0");
	XmlWriter->writeLineBreak();
}

void DunGen::CDunGenXMLWriter::WriteMeshCave()
{
	XmlWriter->writeElement(L"MeshCave",true,
		L"Warping", DunGenInterface->MeshCaveGetWarpEnabled() ? L"1" : L"0",
		L"Smoothing", DunGenInterface->MeshCaveGetSmoothEnabled() ? L"1" : L"0",
		L"WarpStrength", irr::core::stringw(DunGenInterface->MeshCaveGetWarpStrength()).c_str(),
		L"WarpRandomSeed", irr::core::stringw(DunGenInterface->MeshCaveGetWarpRandomSeed()).c_str(),
		L"NormalWeighting", irr::core::stringw(DunGenInterface->MeshCaveGetNormalWeightMethod()).c_str());
	XmlWriter->writeLineBreak();
}

void DunGen::CDunGenXMLWriter::WriteGameplay()
{
	XmlWriter->writeElement(L"Gameplay",true,
		L"MonsterHitpoints", irr::core::stringw(DunGenInterface->GetMonsterHitpoints()).c_str(),
		L"BossMonsterHitpoints", irr::core::stringw(DunGenInterface->GetBossMonsterHitpoints()).c_str(),
		L"MonsterFireTime", irr::core::stringw(DunGenInterface->GetMonsterFireTime()).c_str(),
		L"BossMonsterFireTime", irr::core::stringw(DunGenInterface->GetBossMonsterFireTime()).c_str(),
		L"MonsterDamage",irr::core::stringw(DunGenInterface->GetMonsterDamage()).c_str());
	XmlWriter->writeLineBreak();
}

void DunGen::CDunGenXMLWriter::WriteDescription()
{
	XmlWriter->writeElement(L"Description",true,
		L"Intro", irr::core::stringw(DunGenInterface->GetDescriptionIntro().c_str()).c_str(),
		L"Extro", irr::core::stringw(DunGenInterface->GetDescriptionExtro().c_str()).c_str());
	XmlWriter->writeLineBreak();
}

void DunGen::CDunGenXMLWriter::WritePortalObject()
{
	SPortalObject portal = DunGenInterface->ObjectsGetPortalObject();

	XmlWriter->writeElement(L"PortalObject",false, L"IsPlaced",
		portal.IsPlaced ? L"1" : L"0");
	XmlWriter->writeLineBreak();

	XmlWriter->writeElement(L"Position",true,
		L"X", irr::core::stringw(portal.Position.X).c_str(),
		L"Y", irr::core::stringw(portal.Position.Y).c_str(),
		L"Z", irr::core::stringw(portal.Position.Z).c_str()
		);
	XmlWriter->writeLineBreak();

	XmlWriter->writeElement(L"Rotation",true,
		L"X", irr::core::stringw(portal.Rotation.X).c_str(),
		L"Y", irr::core::stringw(portal.Rotation.Y).c_str(),
		L"Z", irr::core::stringw(portal.Rotation.Z).c_str()
		);
	XmlWriter->writeLineBreak();

	XmlWriter->writeClosingTag(L"PortalObject");
	XmlWriter->writeLineBreak();
}

void DunGen::CDunGenXMLWriter::WriteBossObjects()
{
	XmlWriter->writeElement(L"BossObjects",false);
	XmlWriter->writeLineBreak();

	for (unsigned int i=0; i<EBossObject::NUMBER_OF_OBJECTS; ++i)
	{
		SBossObject boss = DunGenInterface->ObjectsGetBossObject(static_cast<EBossObject::Enum>(i));
		if (boss.IsPlaced)
		{
			XmlWriter->writeElement(L"Boss",true,
			L"Type", irr::core::stringw(i).c_str(),
			L"X", irr::core::stringw(boss.Position.X).c_str(),
			L"Y", irr::core::stringw(boss.Position.Y).c_str(),
			L"Z", irr::core::stringw(boss.Position.Z).c_str()
			);
			XmlWriter->writeLineBreak();
		}
	}

	XmlWriter->writeClosingTag(L"BossObjects");
	XmlWriter->writeLineBreak();
}

void DunGen::CDunGenXMLWriter::WriteObjects()
{
	XmlWriter->writeElement(L"Objects",false);
	XmlWriter->writeLineBreak();

	std::vector<SPlacedObject> placedObjects = DunGenInterface->ObjectsGetObjects();
	for (unsigned int i=0; i<placedObjects.size(); ++i)
	{
		XmlWriter->writeElement(L"Ship",true,
			L"Type", irr::core::stringw(placedObjects[i].Type).c_str(),
			L"X", irr::core::stringw(placedObjects[i].Position.X).c_str(),
			L"Y", irr::core::stringw(placedObjects[i].Position.Y).c_str(),
			L"Z", irr::core::stringw(placedObjects[i].Position.Z).c_str()
			);
		XmlWriter->writeLineBreak();
	}

	XmlWriter->writeClosingTag(L"Objects");
	XmlWriter->writeLineBreak();
}
