// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef SOUNDEFFECTS_H
#define SOUNDEFFECTS_H

#include <irrKlang.h>

namespace Nightork
{

class CSoundEffects
{
public:
	/// Constructor.
	CSoundEffects(irrklang::ISoundEngine* irrKlangDevice_);
	/// Destructor.
	~CSoundEffects();

	/// Stop all sounds.
	void StopSounds();

	/// Set sound level.
	void SetSoundLevel(float soundLevel_);

	/// Play or stop player engine sound.
	void PlayPlayerEngine(bool value_);

	/// Play or stop player afterburner sound.
	void PlayPlayerAfterburner(bool value_);

	/// Play player explosion sound.
	void PlayPlayerExplosion();

	/// Play player hit sound.
	void PlayPlayerHit();

	/// Play player fire sound.
	void PlayPlayerFire();

	/// Play power up on.
	void PlayPowerUpOn();

	/// Play power up off.
	void PlayPowerUpOff();

	/// Play monster explosion sound.
	void PlayMonsterExplosion(const irrklang::vec3df& pos_);

	/// Play monster explosion sound.
	void PlayMonsterCriticalDestruction(const irrklang::vec3df& pos_);

	/// Sets the listener position
	void SetListenerPosition(const irrklang::vec3df& pos_,const irrklang::vec3df& lookDir_,
		const irrklang::vec3df& up_);

private:
	irrklang::ISoundEngine* IrrKlangDevice;					///< The sound device.

	irrklang::ISoundSource* SoundSourcePlayerEngine;		///< The engine sound source.
	irrklang::ISoundSource* SoundSourcePlayerAfterburner;	///< The afterburner sound source.
	irrklang::ISoundSource* SoundSourcePlayerExplosion;		///< The explosion sound source.
	irrklang::ISoundSource* SoundSourcePlayerHit;			///< The hit sound source.
	irrklang::ISoundSource* SoundSourcePlayerFire;			///< The fire sound source.
	irrklang::ISoundSource* SoundSourcePowerUpOn;			///< The power up on sound source.
	irrklang::ISoundSource* SoundSourcePowerUpOff;			///< The power up off sound source.

	irrklang::ISoundSource* SoundSourceMonsterExplosion;	///< The monster explosion sound source.
	irrklang::ISoundSource* SoundSourceMonsterCriticalDestruction;	///< The monster critical destruction sound source.

	irrklang::ISound* SoundPlayerEngine;					///< The engine sound.
	irrklang::ISound* SoundPlayerAfterburner;				///< The afterburner sound.

};

}

#endif

