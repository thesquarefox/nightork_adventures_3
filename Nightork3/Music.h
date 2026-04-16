// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef MUSIC_H
#define MUSIC_H

#include <irrKlang.h>
#include <irrlicht.h>
#include <string>
#include <vector>

namespace Nightork
{

class CMusic
{
public:
	struct EMusicStyle
	{
		enum Enum
		{
			Normal,
			MainMenu,
			Intro,
			Extro
		};
	};

public:
	/// Constructor.
	CMusic(irr::IrrlichtDevice* irrDevice_, irrklang::ISoundEngine* irrKlangDevice_);
	/// Destructor.
	~CMusic();

	/// Set sound level.
	void SetSoundLevel(float soundLevel_);

	/// Update
	void Update(EMusicStyle::Enum style_);

private:
	/// Plays music.
	void PlayMusic();

	/// Stops music.
	void StopMusic();

	/// Reloads the play list.
	void ReloadPlayList();

private:
	irrklang::ISoundEngine* IrrKlangDevice;			///< The sound device.
	irrklang::ISoundSource* MainMenuMusicSource;	///< The main menu music source.
	irrklang::ISoundSource* IntroMusicSource;		///< The intro music source.
	irrklang::ISoundSource* ExtroMusicSource;		///< The extro music source.
	irrklang::ISoundSource* CurrentMusicSource;		///< The current music source.
	irrklang::ISound* CurrentMusic;					///< The current playing music.

	std::vector<std::string> MusicList;				///< The music list.
	std::vector<std::string> PlayList;				///< The current play list.

	float SoundLevel;								///< The current sound level.
	EMusicStyle::Enum Style;						///< The current music style.
};

}

#endif

