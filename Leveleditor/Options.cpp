// Copyright (C) 2011-2014 by Maximilian Hönig

#include <iostream>
#include <steam_api.h>
#include <time.h>
#include "Options.h"
#include "Gui.h"
#include "LSystemRandom.h"
#include "ObjectPlacement.h"

Editor::COptions::COptions(irr::IrrlichtDevice* irrDevice_)
	: DungeonGenerator(new DunGen::CDunGen(irrDevice_))
	, IrrDevice(irrDevice_)
	, GUIEnvironment(irrDevice_->getGUIEnvironment())
	, LSystemRandom(new CLSystemRandom(DungeonGenerator))
	, WorkingDirectory(irrDevice_->getFileSystem()->getWorkingDirectory())
{
	DungeonGenerator->StartUp();
	DungeonGenerator->SetPrintToConsole(true);
	ObjectPlacement = new CObjectPlacement(DungeonGenerator);

	CreateScene();
}

Editor::COptions::~COptions()
{
	delete LSystemRandom;
	delete DungeonGenerator;
}

irr::scene::ICameraSceneNode* Editor::COptions::GetCamera()
{
	return Camera;
}

void Editor::COptions::IterationMinus()
{
	unsigned int iteration = DungeonGenerator->LSystemGetIteration();
	if (0 < iteration)
	{
		DungeonGenerator->LSystemSetIteration(iteration-1);
	}
}

void Editor::COptions::IterationPlus()
{
	DungeonGenerator->LSystemSetIteration(DungeonGenerator->LSystemGetIteration()+1);
}

void Editor::COptions::AssembleCave()
{
	// delete old objects
	DungeonGenerator->ObjectsDelete();
	DungeonGenerator->ObjectsDeleteStatic();

	// create cave
	DungeonGenerator->RandomGeneratorSetSeed(0);
	DungeonGenerator->GenerateCave();

	CreateScene();
}

void Editor::COptions::CreateScene()
{
	const irr::io::path dir = IrrDevice->getFileSystem()->getWorkingDirectory();
	IrrDevice->getFileSystem()->changeWorkingDirectoryTo(WorkingDirectory);

	// create camera
	IrrDevice->getSceneManager()->getRootSceneNode()->removeAll();
	Camera = IrrDevice->getSceneManager()->addCameraSceneNodeFPS(0,100.0f,0.05f);
	Camera->setNearValue(1.0f);
	Camera->setFarValue(10000.0f);
	Camera->setPosition(irr::core::vector3df(0,0,0));
	Camera->setTarget(irr::core::vector3df(99999.f, 99999.f, 99999.f));

	// create light
	irr::scene::ILightSceneNode* light = IrrDevice->getSceneManager()->addLightSceneNode(Camera);
	irr::video::SLight lightdata;
	lightdata.AmbientColor = irr::video::SColorf(0.0f,0.0f,0.0f);
	lightdata.DiffuseColor = irr::video::SColorf(1.0f,1.0f,1.0f);
	lightdata.SpecularColor = irr::video::SColorf(0.0f,0.0f,0.0f);
	lightdata.Radius = 1000.0f;
	lightdata.CastShadows = false;
	light->setLightData(lightdata);

	// add dungeon to scene
	DungeonGenerator->AddDungeon(IrrDevice->getSceneManager()->getRootSceneNode(),IrrDevice->getSceneManager());

	IrrDevice->getFileSystem()->changeWorkingDirectoryTo(dir);
}

void Editor::COptions::SetParametersRandom()
{
	LSystemRandom->SetParametersRandom();
}

void Editor::COptions::AddRule()
{
	// get root element
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	// test if window active
	irr::gui::IGUIElement* window = root->getElementFromId(CGUI::EGuiId::WINDOW_CAVE, true);
	if (!window)
		return;

	// read values
	DunGen::SSubstitutionRule rule;
	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RULE_SYMBOL, true);
	irr::core::stringc tmpString = editBox->getText();
	if (tmpString.size() < 1)
		return;	
	rule.Symbol = tmpString[0];

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RULE_REPLACEMENT, true);
	tmpString = editBox->getText();
	rule.Substitution = tmpString.c_str();

	DungeonGenerator->LSystemAddRule(rule);
}

void Editor::COptions::DeleteRules()
{
	DungeonGenerator->LSystemDeleteRules();
}

void Editor::COptions::PlaceObjects()
{
	// place random objects
	ObjectPlacement->PlaceObjects();
	CreateScene();
}

void Editor::COptions::PlaceObject(unsigned int i)
{
	irr::core::vector3df camPos = Camera->getAbsolutePosition();
	irr::core::vector3d<double> position = irr::core::vector3d<double>(
		static_cast<double>(camPos.X),
		static_cast<double>(camPos.Y),
		static_cast<double>(camPos.Z));
	irr::core::vector3df camTarget = Camera->getTarget();

	DunGen::EBossObject::Enum bossType = DunGen::EBossObject::NUMBER_OF_OBJECTS;

	switch (i)
	{
	default:
		{
			DunGen::SPortalObject portal = DungeonGenerator->ObjectsGetPortalObject();
			portal.IsPlaced = true;
			portal.Position = position;
			DungeonGenerator->ObjectsSetPortalObject(portal);
			break;
		}
	case 1:
		bossType = DunGen::EBossObject::BOSS_01;
	case 2:
		if (DunGen::EBossObject::NUMBER_OF_OBJECTS == bossType)
			bossType = DunGen::EBossObject::BOSS_02;
	case 3:
		if (DunGen::EBossObject::NUMBER_OF_OBJECTS == bossType)
			bossType = DunGen::EBossObject::BOSS_03;
	case 4:
		if (DunGen::EBossObject::NUMBER_OF_OBJECTS == bossType)
			bossType = DunGen::EBossObject::BOSS_04;
	case 5:
		if (DunGen::EBossObject::NUMBER_OF_OBJECTS == bossType)
			bossType = DunGen::EBossObject::BOSS_05;
	case 6:
		if (DunGen::EBossObject::NUMBER_OF_OBJECTS == bossType)
			bossType = DunGen::EBossObject::BOSS_06;
		{
			DunGen::SBossObject boss;
			boss.IsPlaced = true;
			boss.Position = position;
			DungeonGenerator->ObjectsSetBossObject(bossType, boss);
			break;
		}
	}

	CreateScene();
	Camera->setPosition(camPos);
	Camera->setTarget(camTarget);
}

void Editor::COptions::DeleteObject(unsigned int i)
{
	irr::core::vector3df camPos = Camera->getAbsolutePosition();
	irr::core::vector3df camTarget = Camera->getTarget();

	DunGen::EBossObject::Enum bossType = DunGen::EBossObject::NUMBER_OF_OBJECTS;

	switch (i)
	{
	default:
		{
			DunGen::SPortalObject portal = DungeonGenerator->ObjectsGetPortalObject();
			portal.IsPlaced = false;
			DungeonGenerator->ObjectsSetPortalObject(portal);
			break;
		}
	case 1:
		bossType = DunGen::EBossObject::BOSS_01;
	case 2:
		if (DunGen::EBossObject::NUMBER_OF_OBJECTS == bossType)
			bossType = DunGen::EBossObject::BOSS_02;
	case 3:
		if (DunGen::EBossObject::NUMBER_OF_OBJECTS == bossType)
			bossType = DunGen::EBossObject::BOSS_03;
	case 4:
		if (DunGen::EBossObject::NUMBER_OF_OBJECTS == bossType)
			bossType = DunGen::EBossObject::BOSS_04;
	case 5:
		if (DunGen::EBossObject::NUMBER_OF_OBJECTS == bossType)
			bossType = DunGen::EBossObject::BOSS_05;
	case 6:
		if (DunGen::EBossObject::NUMBER_OF_OBJECTS == bossType)
			bossType = DunGen::EBossObject::BOSS_06;
		{
			DunGen::SBossObject boss = DungeonGenerator->ObjectsGetBossObject(bossType);
			boss.IsPlaced = false;
			DungeonGenerator->ObjectsSetBossObject(bossType, boss);
			break;
		}
	}

	CreateScene();
	Camera->setPosition(camPos);
	Camera->setTarget(camTarget);
	Camera->setInputReceiverEnabled(false);
}

void Editor::COptions::PortalRotationApply()
{
	// get root element
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	// test if window active
	irr::gui::IGUIElement* window = root->getElementFromId(CGUI::EGuiId::WINDOW_OBJECTPLACEMENT, true);
	if (!window)
		return;

	// read camera settings
	irr::core::vector3df camPos = Camera->getAbsolutePosition();
	irr::core::vector3df camTarget = Camera->getTarget();

	// read values
	DunGen::SPortalObject portal = DungeonGenerator->ObjectsGetPortalObject();
	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_START_ROTATION_X, true);
	irr::core::stringc tmpString = editBox->getText();
	portal.Rotation.X = atof(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_START_ROTATION_Y, true);
	tmpString = editBox->getText();
	portal.Rotation.Y = atof(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_START_ROTATION_Z, true);
	tmpString = editBox->getText();
	portal.Rotation.Z = atof(tmpString.c_str());

	// apply
	DungeonGenerator->ObjectsSetPortalObject(portal);
	CreateScene();
	Camera->setPosition(camPos);
	Camera->setTarget(camTarget);
	Camera->setInputReceiverEnabled(false);
}

void Editor::COptions::TakeScreenshot()
{
	// read screenshot
	irr::video::IImage* screenShot = IrrDevice->getVideoDriver()->createScreenShot();

	const irr::io::path dir = IrrDevice->getFileSystem()->getWorkingDirectory();
	IrrDevice->getFileSystem()->changeWorkingDirectoryTo(WorkingDirectory);

	// get time info
	time_t timeStamp;
	tm *timeInfo;
	timeStamp = time(NULL);
	timeInfo = localtime(&timeStamp);
	
	// generate filename
	irr::io::path timePartOfFile = irr::core::stringc(timeInfo->tm_year+1900) + "-";
	if (timeInfo->tm_mon < 9) timePartOfFile += "0";
	timePartOfFile += irr::core::stringc(timeInfo->tm_mon+1) + "-";
	if (timeInfo->tm_mday < 10) timePartOfFile += "0";
	timePartOfFile += irr::core::stringc(timeInfo->tm_mday) + "_";

	if (timeInfo->tm_hour < 10) timePartOfFile += "0";
	timePartOfFile += irr::core::stringc(timeInfo->tm_hour) + "-";
	if (timeInfo->tm_min < 10) timePartOfFile += "0";
	timePartOfFile += irr::core::stringc(timeInfo->tm_min) + "-";
	if (timeInfo->tm_sec < 10) timePartOfFile += "0";
	timePartOfFile += irr::core::stringc(timeInfo->tm_sec) + "_";

	unsigned int msec = timeStamp%1000;
	if (msec < 100) timePartOfFile += "0";
	if (msec < 10) timePartOfFile += "0";
	timePartOfFile += irr::core::stringc(msec);

	irr::io::path path = "screenshots/";
	irr::io::path filename = "Editor_Screenshot_";
	filename = filename + timePartOfFile + ".png";

	// save file
	std::cout << "[Options]: Saving screenshot to file " << filename.c_str() << " ...";
	filename = path + filename;
	irr::io::IWriteFile* file = IrrDevice->getFileSystem()->createAndWriteFile(filename);

	// if file access successful
	if (file)
	{
		// save screenshot
		IrrDevice->getVideoDriver()->writeImageToFile(screenShot,file,1);
		// close file
		file->drop();
		std::cout << " finished." << std::endl;
	}
	else
		std::cout << " failed." << std::endl;

	// delete screenshot from memory
	screenShot->drop();	

	IrrDevice->getFileSystem()->changeWorkingDirectoryTo(dir);
}

void Editor::COptions::SaveFile(const irr::io::path& filename_)
{
	// save dungeon
	DungeonGenerator->RandomGeneratorSetSeed(0);
	DungeonGenerator->WriteDungeonToFile(filename_);

	// achievement
	if (DungeonGenerator->ObjectsGetPortalObject().IsPlaced && SteamUserStats() && SteamUser())
	{
		// seems to be a valid level -> unlock achievement
		SteamUserStats()->SetStat("LevelCreated", 1);
		SteamUserStats()->SetAchievement("CREATED_LEVEL");
		SteamUserStats()->StoreStats();
	}
}

void Editor::COptions::LoadFile(const irr::io::path& filename_)
{
	// load file and process dungeon
	DungeonGenerator->ReadDungeonFromFile(filename_);
	CreateScene();
}
