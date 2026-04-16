// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <irrlicht.h>
#include <vector>
#include "Constants.h"
#include "Pilot.h"

namespace Nightork
{

class CHighScores
{
public:
	static const unsigned int HIGHSCORE_NUMBER = 3;

	struct SHighScore
	{
		irr::io::path PilotName;
		unsigned int Deaths;
		unsigned int TimeNeeded;

		bool operator>(const SHighScore& other_);
	};

public:
	/// Constructor.
	CHighScores(irr::IrrlichtDevice* irrDevice_);
	/// Destructor.
	~CHighScores();

	/// Adds a high score to the high score list, if good enough.
	void AddHighScore(const irr::io::path& pilotName_, Constants::Level::EArena::Enum arena_, unsigned int deaths_, unsigned int timeNeeded_);

	/// Returns a high score.
	SHighScore GetHighScore(Constants::Level::EArena::Enum arena_, unsigned int index_) const;

private:
	// Saves the high scores.
	void SaveHighScores();

private:
	irr::io::IFileSystem* FileSystem;	///< The file system.
	std::vector<SHighScore> HighScores[Constants::Level::EArena::Number];	///< Stores the high scores.
};

}

#endif
