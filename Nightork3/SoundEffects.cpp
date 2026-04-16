// Copyright (C) 2011-2026 by Maximilian Hönig

#include "SoundEffects.h"


Nightork::CSoundEffects::CSoundEffects(irrklang::ISoundEngine* irrKlangDevice_)
	: IrrKlangDevice(irrKlangDevice_)
	, SoundPlayerEngine(NULL)
	, SoundPlayerAfterburner(NULL)
{
	// load sounds
	SoundSourcePlayerEngine = IrrKlangDevice->addSoundSourceFromFile("sound/Engine.ogg");
	SoundSourcePlayerAfterburner = IrrKlangDevice->addSoundSourceFromFile("sound/Afterburner.ogg");
	SoundSourcePlayerExplosion = IrrKlangDevice->addSoundSourceFromFile("sound/PlayerShipExplosion.ogg");
	SoundSourcePlayerHit = IrrKlangDevice->addSoundSourceFromFile("sound/PlayerShipHit.ogg");
	SoundSourcePlayerFire = IrrKlangDevice->addSoundSourceFromFile("sound/PlayerShipFire.ogg");
	SoundSourcePowerUpOn = IrrKlangDevice->addSoundSourceFromFile("sound/PowerUpOn.ogg");
	SoundSourcePowerUpOff = IrrKlangDevice->addSoundSourceFromFile("sound/PowerUpOff.ogg");

	SoundSourceMonsterExplosion = IrrKlangDevice->addSoundSourceFromFile("sound/MonsterShipExplosion.ogg");
	SoundSourceMonsterExplosion->setDefaultMinDistance(30.0f);
	SoundSourceMonsterExplosion->setDefaultMaxDistance(300.0f);
	
	SoundSourceMonsterCriticalDestruction = IrrKlangDevice->addSoundSourceFromFile("sound/CriticalDestruction.ogg");
	SoundSourceMonsterCriticalDestruction->setDefaultMinDistance(30.0f);
	SoundSourceMonsterCriticalDestruction->setDefaultMaxDistance(300.0f);
}

Nightork::CSoundEffects::~CSoundEffects()
{
}

void Nightork::CSoundEffects::StopSounds()
{
	if (SoundPlayerEngine)
	{
		SoundPlayerEngine->stop();
		SoundPlayerEngine->drop();
		SoundPlayerEngine = NULL;
	}
	if (SoundPlayerAfterburner)
	{
		SoundPlayerAfterburner->stop();
		SoundPlayerAfterburner->drop();
		SoundPlayerAfterburner = NULL;
	}
}

void Nightork::CSoundEffects::SetSoundLevel( float soundLevel_ )
{
	SoundSourcePlayerEngine->setDefaultVolume(soundLevel_);
	SoundSourcePlayerAfterburner->setDefaultVolume(soundLevel_);
	SoundSourcePlayerExplosion->setDefaultVolume(soundLevel_);
	SoundSourcePlayerHit->setDefaultVolume(soundLevel_);
	SoundSourcePlayerFire->setDefaultVolume(soundLevel_);
	SoundSourcePowerUpOn->setDefaultVolume(soundLevel_);
	SoundSourcePowerUpOff->setDefaultVolume(soundLevel_);
	SoundSourceMonsterExplosion->setDefaultVolume(soundLevel_);
	SoundSourceMonsterCriticalDestruction->setDefaultVolume(soundLevel_);
}

void Nightork::CSoundEffects::PlayPlayerEngine(bool value_)
{
	if (value_ && !SoundPlayerEngine)
	{
 		SoundPlayerEngine = IrrKlangDevice->play2D(SoundSourcePlayerEngine,true,true);
		SoundPlayerEngine->setIsPaused(false);
	}
	else if (!value_ && SoundPlayerEngine)
	{
		SoundPlayerEngine->stop();
		SoundPlayerEngine->drop();
		SoundPlayerEngine = NULL;
	}
}

void Nightork::CSoundEffects::PlayPlayerAfterburner(bool value_)
{
	if (value_ && !SoundPlayerAfterburner)
	{
		SoundPlayerAfterburner = IrrKlangDevice->play2D(SoundSourcePlayerAfterburner,true,true);
		SoundPlayerAfterburner->setIsPaused(false);
	}
	else if (!value_ && SoundPlayerAfterburner)
	{
		SoundPlayerAfterburner->stop();
		SoundPlayerAfterburner->drop();
		SoundPlayerAfterburner = NULL;
	}
}

void Nightork::CSoundEffects::PlayPlayerExplosion()
{
	IrrKlangDevice->play2D(SoundSourcePlayerExplosion,false);
}

void Nightork::CSoundEffects::PlayPlayerHit()
{
	IrrKlangDevice->play2D(SoundSourcePlayerHit,false);
}

void Nightork::CSoundEffects::PlayPlayerFire()
{
	IrrKlangDevice->play2D(SoundSourcePlayerFire,false);
}

void Nightork::CSoundEffects::PlayPowerUpOn()
{
	IrrKlangDevice->play2D(SoundSourcePowerUpOn, false);
}

void Nightork::CSoundEffects::PlayPowerUpOff()
{
	IrrKlangDevice->play2D(SoundSourcePowerUpOff, false);
}

void Nightork::CSoundEffects::PlayMonsterExplosion(const irrklang::vec3df& pos_)
{
	IrrKlangDevice->play3D(SoundSourceMonsterExplosion,pos_,false);
}

void Nightork::CSoundEffects::PlayMonsterCriticalDestruction(const irrklang::vec3df& pos_)
{
	IrrKlangDevice->play3D(SoundSourceMonsterCriticalDestruction, pos_, false);
}

void Nightork::CSoundEffects::SetListenerPosition(const irrklang::vec3df& pos_,const irrklang::vec3df& lookDir_, const irrklang::vec3df& up_)
{
	IrrKlangDevice->setListenerPosition(pos_,lookDir_,irrklang::vec3df(0.0f,0.0f,0.0f),up_);
}
