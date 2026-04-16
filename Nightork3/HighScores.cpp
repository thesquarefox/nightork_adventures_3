// Copyright (C) 2011-2026 by Maximilian Hönig

#include <iostream>
#include <steam_api.h>
#include "HighScores.h"

bool Nightork::CHighScores::SHighScore::operator>(const SHighScore& other_)
{
	if (0 == other_.TimeNeeded)
		return true;

	return
		Deaths < other_.Deaths ? true :
		Deaths > other_.Deaths ? false :
		TimeNeeded < other_.TimeNeeded ? true :
		false;
}

Nightork::CHighScores::CHighScores(irr::IrrlichtDevice* irrDevice_)
	: FileSystem(irrDevice_->getFileSystem())
	, HighScores()
{
	std::cout << "[HighScores:] Reading high score data ";

	// get read access to file
	irr::io::IReadFile* file = FileSystem->createAndOpenFile("arenas/Highscores");

	if (!file)
	{
		std::cout << " failed." << std::endl;
	}
	else
	{
		irr::io::IXMLReader* xmlReader = FileSystem->createXMLReader(file);

		// process file content
		while(xmlReader->read())
		{
			if (xmlReader->getNodeType() == irr::io::EXN_ELEMENT)
			{
				if (irr::core::stringw("Score") == xmlReader->getNodeName())
				{
				
					SHighScore score;
					score.PilotName = xmlReader->getAttributeValueSafe(L"Pilot");
					score.Deaths = xmlReader->getAttributeValueAsInt(L"Deaths");
					score.TimeNeeded = xmlReader->getAttributeValueAsInt(L"Time");

					Constants::Level::EArena::Enum arena = static_cast<Constants::Level::EArena::Enum>(xmlReader->getAttributeValueAsInt(L"Arena"));
					HighScores[arena].push_back(score);
				}
			}
		}

		// close file
		xmlReader->drop();
		file->drop();
	}

	// fill list till full
	for (unsigned int a = 0; a < Constants::Level::EArena::Number; ++a)
	{
		for (unsigned int i = static_cast<unsigned int>(HighScores[a].size()); i < HIGHSCORE_NUMBER; ++i)
		{
			Nightork::CHighScores::SHighScore tmp;
			tmp.PilotName = "empty";
			tmp.Deaths = 0;
			tmp.TimeNeeded = 0;
			HighScores[a].push_back(tmp);
		}
		HighScores[a].resize(HIGHSCORE_NUMBER);
	}

	std::cout << " succeeded." << std::endl;
}

void Nightork::CHighScores::SaveHighScores()
{
	std::cout << "[HighScores:] Writing high score data ";

	// get write access to file
	irr::io::IWriteFile* file = FileSystem->createAndWriteFile("arenas/Highscores");

	if (!file)
	{
		std::cout << " failed." << std::endl;
		return;
	}

	irr::io::IXMLWriter* xmlWriter = FileSystem->createXMLWriter(file);

	// write file content
	xmlWriter->writeXMLHeader();
	xmlWriter->writeLineBreak();

	xmlWriter->writeElement(L"HighScores",false);
	xmlWriter->writeLineBreak();

	for (unsigned int a = 0; a < Constants::Level::EArena::Number; ++a)
		for (unsigned int i=0; i<HighScores[a].size(); ++i)
		{
			xmlWriter->writeElement(L"Score", true,
				L"Arena", irr::core::stringw(a).c_str(),
				L"Pilot", irr::core::stringw(HighScores[a][i].PilotName.c_str()).c_str(),
				L"Deaths", irr::core::stringw(HighScores[a][i].Deaths).c_str(),
				L"Time", irr::core::stringw(HighScores[a][i].TimeNeeded).c_str()
			);
			xmlWriter->writeLineBreak();
		}

	xmlWriter->writeClosingTag(L"HighScores");
	xmlWriter->writeLineBreak();

	// close file
	xmlWriter->drop();
	file->drop();

	std::cout << " succeeded." << std::endl;
}

Nightork::CHighScores::~CHighScores()
{
}

void Nightork::CHighScores::AddHighScore(const irr::io::path& pilotName_, Constants::Level::EArena::Enum arena_, unsigned int deaths_, unsigned int timeNeeded_)
{
	SHighScore score;
	score.PilotName = pilotName_;
	score.Deaths = deaths_;
	score.TimeNeeded = timeNeeded_;

	// add new score
	unsigned int index = 0;
	bool highScoresChanged = false;
	while (index < HighScores[arena_].size())
	{
		// better score than the actual?
		if (score>HighScores[arena_][index])
		{
			HighScores[arena_].insert(HighScores[arena_].begin()+index,score);
			highScoresChanged = true;
			break;
		}

		++index;
	}

	for (unsigned int a = 0; a < Constants::Level::EArena::Number; ++a)
		HighScores[arena_].resize(HIGHSCORE_NUMBER);	

	// save high scores
	if (highScoresChanged)
	{
		SaveHighScores();
	}

	if (SteamUserStats() && SteamUser())
		switch (arena_)
		{
		case Constants::Level::EArena::Sphere:
			SteamUserStats()->SetAchievement("ACH_SPHERE");
			break;
		case Constants::Level::EArena::Hive:
			SteamUserStats()->SetAchievement("ACH_HIVE");
			break;
		case Constants::Level::EArena::Cave:
			SteamUserStats()->SetAchievement("ACH_CAVE");
			break;
		}
}

Nightork::CHighScores::SHighScore Nightork::CHighScores::GetHighScore(Constants::Level::EArena::Enum arena_, unsigned int index_) const
{
	return HighScores[arena_][index_];
}
