// Copyright (C) 2011-2026 by Maximilian Hönig

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "Music.h"
#include "Pilot.h"
#include "SoundEffects.h"

Nightork::CPilot::CPilot(irr::IrrlichtDevice* irrDevice_)
	: FileSystem(irrDevice_->getFileSystem())
	, Music(NULL)
	, SoundEffects(NULL)
	, TotemType(Constants::Player::ETotemType::IceBear)
	, SoundLevelMusic(0.5)
	, SoundLevelEffects(0.5)
	, MouseSensitivity(1.0)
	, MouseInvertY(false)
	, CurrentPilot()
{
	Controls[SControls::Forward] = irr::KEY_KEY_W;
	Controls[SControls::Backward] = irr::KEY_KEY_S;
	Controls[SControls::Left] = irr::KEY_KEY_A;
	Controls[SControls::Right] = irr::KEY_KEY_D;
	Controls[SControls::Afterburner] = irr::KEY_SPACE;
	Controls[SControls::Highlighting] = irr::KEY_KEY_R;
	Controls[SControls::Weapon1] = irr::KEY_KEY_CODES_COUNT + 0; // mouse button 1
	Controls[SControls::Weapon2] = irr::KEY_KEY_CODES_COUNT + 1; // mouse button 2
}

Nightork::CPilot::~CPilot()
{
	std::ofstream outfile("Pilot.cfg", std::ofstream::out & std::ofstream::binary);
	if (outfile)
	{
		outfile << CurrentPilot.c_str() << std::endl;
	}
}

void Nightork::CPilot::Initialize(Nightork::CMusic* music_, Nightork::CSoundEffects* soundEffects_)
{
	Music = music_;
	SoundEffects = soundEffects_;

	std::string pilotname;
	std::ifstream infile("Pilot.cfg", std::ifstream::in & std::ifstream::binary);
	if (infile)
	{
		std::getline(infile, pilotname);
		if (LoadPilot(pilotname.c_str()))
			return;
	}

	CreateNewPilot("Default");
}

bool Nightork::CPilot::CreateNewPilot(const irr::io::path& filename_)
{
	const irr::io::path dir = FileSystem->getWorkingDirectory();
	FileSystem->changeWorkingDirectoryTo("pilots");
	bool existFile = FileSystem->existFile(filename_);
	FileSystem->changeWorkingDirectoryTo(dir);
	if (existFile)
	{
		return false;
	}

	irr::io::path oldName = CurrentPilot;
	CurrentPilot = filename_;
	if (!SaveCurrentPilot())
	{
		LoadPilot(oldName);
		return false;
	}

	return true;
}

bool Nightork::CPilot::SaveCurrentPilot()
{
	std::cout << "[Pilot:] Writing " << CurrentPilot.c_str();

	// get write access to file
	const irr::io::path dir = FileSystem->getWorkingDirectory();
	FileSystem->changeWorkingDirectoryTo("pilots");
	irr::io::IWriteFile* file = FileSystem->createAndWriteFile(CurrentPilot);
	FileSystem->changeWorkingDirectoryTo(dir);

	if (!file)
	{
		std::cout << " failed." << std::endl;
		return false;
	}

	irr::io::IXMLWriter* xmlWriter = FileSystem->createXMLWriter(file);

	// write file content
	xmlWriter->writeXMLHeader();
	xmlWriter->writeLineBreak();

	xmlWriter->writeElement(L"Pilot",false, L"Name", irr::core::stringw(CurrentPilot).c_str());
	xmlWriter->writeLineBreak();

	xmlWriter->writeElement(L"Totem",true,
		L"Type", irr::core::stringw(TotemType).c_str());
	xmlWriter->writeLineBreak();

	xmlWriter->writeElement(L"Settings",true,
		L"SoundLevelMusic", irr::core::stringw(SoundLevelMusic).c_str(),
		L"SoundLevelEffects", irr::core::stringw(SoundLevelEffects).c_str(),
		L"MouseSensitivity", irr::core::stringw(MouseSensitivity).c_str(),
		L"MouseInvertY", irr::core::stringw(MouseInvertY ? 1 : 0).c_str());
	xmlWriter->writeLineBreak();

	xmlWriter->writeElement(L"ControlsMovement",true,
		L"Forward", irr::core::stringw(Controls[SControls::Forward]).c_str(),
		L"Backward", irr::core::stringw(Controls[SControls::Backward]).c_str(),
		L"Left", irr::core::stringw(Controls[SControls::Left]).c_str(),
		L"Right", irr::core::stringw(Controls[SControls::Right]).c_str(),
		L"Afterburner", irr::core::stringw(Controls[SControls::Afterburner]).c_str());
	xmlWriter->writeLineBreak();

	xmlWriter->writeElement(L"ControlsWeapon",true,
		L"Weapon1", irr::core::stringw(Controls[SControls::Weapon1]).c_str(),
		L"Weapon2", irr::core::stringw(Controls[SControls::Weapon2]).c_str(),
		L"Highlighting", irr::core::stringw(Controls[SControls::Highlighting]).c_str());
	xmlWriter->writeLineBreak();

	xmlWriter->writeClosingTag(L"Pilot");
	xmlWriter->writeLineBreak();

	// close file
	xmlWriter->drop();
	file->drop();

	std::cout << " succeeded." << std::endl;

	return true;
}

bool Nightork::CPilot::LoadPilot(const irr::io::path& filename_)
{
	std::cout << "[Pilot:] Reading " << filename_.c_str();

	// get read access to file
	const irr::io::path dir = FileSystem->getWorkingDirectory();
	FileSystem->changeWorkingDirectoryTo("pilots");
	irr::io::IReadFile* file = FileSystem->createAndOpenFile(filename_);
	FileSystem->changeWorkingDirectoryTo(dir);

	if (!file)
	{
		std::cout << " failed." << std::endl;
		return false;
	}

	irr::io::IXMLReader* xmlReader = FileSystem->createXMLReader(file);

	// process file content
	while(xmlReader->read())
	{
		if (xmlReader->getNodeType() == irr::io::EXN_ELEMENT)
		{
			if (irr::core::stringw("Totem") == xmlReader->getNodeName())
			{
				TotemType = static_cast<Constants::Player::ETotemType::Enum>(xmlReader->getAttributeValueAsInt(L"Type"));
			}
			else if (irr::core::stringw("Settings") == xmlReader->getNodeName())
			{
				SoundLevelMusic = xmlReader->getAttributeValueAsFloat(L"SoundLevelMusic");
				SoundLevelEffects = xmlReader->getAttributeValueAsFloat(L"SoundLevelEffects");
				MouseSensitivity = xmlReader->getAttributeValueAsFloat(L"MouseSensitivity");
				MouseInvertY = (0 != xmlReader->getAttributeValueAsInt(L"MouseInvertY"));
			}
			else if (irr::core::stringw("ControlsMovement") == xmlReader->getNodeName())
			{
				Controls[SControls::Forward] = xmlReader->getAttributeValueAsInt(L"Forward");
				Controls[SControls::Backward] = xmlReader->getAttributeValueAsInt(L"Backward");
				Controls[SControls::Left] = xmlReader->getAttributeValueAsInt(L"Left");
				Controls[SControls::Right] = xmlReader->getAttributeValueAsInt(L"Right");
				Controls[SControls::Afterburner] = xmlReader->getAttributeValueAsInt(L"Afterburner");
			}
			else if (irr::core::stringw("ControlsWeapon") == xmlReader->getNodeName())
			{
				Controls[SControls::Weapon1] = xmlReader->getAttributeValueAsInt(L"Weapon1");
				Controls[SControls::Weapon2] = xmlReader->getAttributeValueAsInt(L"Weapon2");
				Controls[SControls::Highlighting] = xmlReader->getAttributeValueAsInt(L"Highlighting");
			}
			else if (irr::core::stringw("Pilot") == xmlReader->getNodeName())
			{
				CurrentPilot = xmlReader->getAttributeValue(L"Name");
			}
		}
	}

	// close file
	xmlReader->drop();
	file->drop();

	std::cout << " succeeded." << std::endl;

	Music->SetSoundLevel(SoundLevelMusic);
	SoundEffects->SetSoundLevel(SoundLevelEffects);

	return true;
}

bool Nightork::CPilot::DeletePilot(const irr::io::path& filename_)
{
	if (filename_ == CurrentPilot)
		CurrentPilot = "";
	const irr::io::path dir = FileSystem->getWorkingDirectory();
	FileSystem->changeWorkingDirectoryTo("pilots");
	bool result = (0 == remove(filename_.c_str()));
	FileSystem->changeWorkingDirectoryTo(dir);
	return result;
}

irr::io::path Nightork::CPilot::LoadPilotName(const irr::io::path& filename_) const
{
	std::cout << "[Pilot:] Reading pilot name from " << filename_.c_str();

	// get read access to file
	const irr::io::path dir = FileSystem->getWorkingDirectory();
	FileSystem->changeWorkingDirectoryTo("pilots");
	irr::io::IReadFile* file = FileSystem->createAndOpenFile(filename_);
	FileSystem->changeWorkingDirectoryTo(dir);

	irr::io::path pilotname = "error!";

	if (!file)
	{
		std::cout << " failed." << std::endl;
		return pilotname;
	}

	irr::io::IXMLReader* xmlReader = FileSystem->createXMLReader(file);

	// process file content
	while (xmlReader->read())
	{
		if (xmlReader->getNodeType() == irr::io::EXN_ELEMENT)
		{
			if (irr::core::stringw("Pilot") == xmlReader->getNodeName())
			{
				pilotname = xmlReader->getAttributeValue(L"Name");
			}
		}
	}

	// close file
	xmlReader->drop();
	file->drop();

	std::cout << " succeeded." << std::endl;

	return pilotname;
}

void Nightork::CPilot::SetTotemType(Constants::Player::ETotemType::Enum type_)
{
	TotemType = type_;
}

Nightork::Constants::Player::ETotemType::Enum Nightork::CPilot::GetTotemType() const
{
	return TotemType;
}

void Nightork::CPilot::SetSoundLevelMusic(float val_)
{
	SoundLevelMusic = val_;
}

float Nightork::CPilot::GetSoundLevelMusic() const
{
	return SoundLevelMusic;
}

void Nightork::CPilot::SetSoundLevelEffects(float val_)
{
	SoundLevelEffects = val_;
}

float Nightork::CPilot::GetSoundLevelEffects() const
{
	return SoundLevelEffects;
}

void Nightork::CPilot::SetMouseSensitivity(float val_)
{
	MouseSensitivity = val_;
}

float Nightork::CPilot::GetMouseSensitivity() const
{
	return MouseSensitivity;
}

void Nightork::CPilot::SetMouseInvertY(bool val_)
{
	MouseInvertY = val_;
}

bool Nightork::CPilot::GetMouseInvertY() const
{
	return MouseInvertY;
}

void Nightork::CPilot::SetControl(SControls::Enum control_, unsigned int value_)
{
	Controls[control_] = value_;
}

unsigned int Nightork::CPilot::GetControl(SControls::Enum control_) const
{
	return Controls[control_];
}

irr::io::path Nightork::CPilot::GetCurrentPilotName() const
{
	return CurrentPilot;
}
