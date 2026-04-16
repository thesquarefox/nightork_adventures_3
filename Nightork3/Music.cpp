// Copyright (C) 2011-2026 by Maximilian Hönig

#include <stdlib.h>
#include <time.h>
#include "Music.h"

Nightork::CMusic::CMusic(irr::IrrlichtDevice* irrDevice_, irrklang::ISoundEngine* irrKlangDevice_)
	: IrrKlangDevice(irrKlangDevice_)
	, MainMenuMusicSource(NULL)
	, IntroMusicSource(NULL)
	, ExtroMusicSource(NULL)
	, CurrentMusicSource(NULL)
	, CurrentMusic(NULL)
	, SoundLevel(1.0f)
	, Style(EMusicStyle::MainMenu)
{
	const irr::io::path dir = irrDevice_->getFileSystem()->getWorkingDirectory();
	irrDevice_->getFileSystem()->changeWorkingDirectoryTo("music");
	irr::io::IFileList* fileList = irrDevice_->getFileSystem()->createFileList();
	irrDevice_->getFileSystem()->changeWorkingDirectoryTo(dir);
	if (fileList)
	{
		for (unsigned int i=2; i < fileList->getFileCount(); ++i)	// The first two entries are . and .. and are ignored.
		{
			const irr::c8* cs = fileList->getFileName(i).c_str();
			std::string name = std::string("music/")+std::string(cs);
			if ("music/_intro.ogg" == name)
			{
				IntroMusicSource = IrrKlangDevice->addSoundSourceFromFile(name.c_str());
				IntroMusicSource->setDefaultVolume(SoundLevel);
			}
			else if ("music/_extro.ogg" == name)
			{
				ExtroMusicSource = IrrKlangDevice->addSoundSourceFromFile(name.c_str());
				ExtroMusicSource->setDefaultVolume(SoundLevel);
			}
			else if ("music/_mainmenu.ogg" == name)
			{
				MainMenuMusicSource = IrrKlangDevice->addSoundSourceFromFile(name.c_str());
				MainMenuMusicSource->setDefaultVolume(SoundLevel);
			}
			else if (std::string::npos != name.find(".ogg"))
			{
				MusicList.push_back(name);
			}
		}
	}

	ReloadPlayList();
}

Nightork::CMusic::~CMusic()
{
}

void Nightork::CMusic::ReloadPlayList()
{
	PlayList = MusicList;
}

void Nightork::CMusic::PlayMusic()
{
	StopMusic();

	if (PlayList.empty())
	{
		ReloadPlayList();
	}

	srand(static_cast<unsigned int>(time(NULL)));
	unsigned int index = rand() % PlayList.size();

	const std::string track = PlayList[index];
	CurrentMusicSource = IrrKlangDevice->addSoundSourceFromFile(track.c_str());
	CurrentMusicSource->setDefaultVolume(SoundLevel);
	CurrentMusic = IrrKlangDevice->play2D(CurrentMusicSource,false,true);
	CurrentMusic->setIsPaused(false);

	PlayList.erase(PlayList.begin()+index);
}

void Nightork::CMusic::StopMusic()
{
	if (CurrentMusic)
	{
		CurrentMusic->stop();
		CurrentMusic->drop();
		CurrentMusic = NULL;
	}
	if (CurrentMusicSource)
	{
		IrrKlangDevice->removeSoundSource(CurrentMusicSource);
		CurrentMusicSource = NULL;
	}
}

void Nightork::CMusic::Update(EMusicStyle::Enum style_)
{
	if (Style != style_)
	{
		StopMusic();
	}

	Style = style_;

	switch (Style)
	{
	case EMusicStyle::Normal:
	default:
		{
			if (!CurrentMusic || CurrentMusic->isFinished())
			{
				PlayMusic();
			}
			break;
		}
	case EMusicStyle::Intro:
		{
			if (!CurrentMusic)
			{
				CurrentMusic = IrrKlangDevice->play2D(IntroMusicSource,true,true);
				CurrentMusic->setIsPaused(false);
			}
			break;
		}
	case EMusicStyle::Extro:
		{
			if (!CurrentMusic)
			{
				CurrentMusic = IrrKlangDevice->play2D(ExtroMusicSource, true, true);
				CurrentMusic->setIsPaused(false);
			}
			break;
		}
	case EMusicStyle::MainMenu:
		{
			if (!CurrentMusic)
			{
				CurrentMusic = IrrKlangDevice->play2D(MainMenuMusicSource,true,true);
				CurrentMusic->setIsPaused(false);
			}
			break;
		}
	}
}

void Nightork::CMusic::SetSoundLevel( float soundLevel_ )
{
	SoundLevel = soundLevel_;
	IntroMusicSource->setDefaultVolume(soundLevel_);
	ExtroMusicSource->setDefaultVolume(soundLevel_);
	MainMenuMusicSource->setDefaultVolume(soundLevel_);
	if (CurrentMusicSource)
	{
		CurrentMusicSource->setDefaultVolume(soundLevel_);
	}
	if (CurrentMusic)
	{
		CurrentMusic->setVolume(soundLevel_);
	}
}

