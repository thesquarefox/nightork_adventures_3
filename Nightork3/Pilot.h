// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef PILOT_H
#define PILOT_H

#include <irrlicht.h>
#include <vector>
#include "Constants.h"

namespace Nightork
{

class CMusic;
class CSoundEffects;

class CPilot
{
public:
	/// Player ship controls.
	struct SControls
	{
		enum Enum
		{
			Forward = 0,
			Backward,
			Left,
			Right,
			Afterburner,
			Highlighting,
			Weapon1,
			Weapon2,
			Number,
		};
	};

public:
	/// Constructor.
	CPilot(irr::IrrlichtDevice* irrDevice_);
	/// Destructor.
	~CPilot();

	void Initialize(CMusic* music_, CSoundEffects* soundEffects_);

	/// Creates a new pilot.
	bool CreateNewPilot(const irr::io::path& filename_);

	/// Saves the current pilot.
	bool SaveCurrentPilot();

	/// Loads a pilot.
	bool LoadPilot(const irr::io::path& filename_);

	/// Deletes a pilot.
	bool DeletePilot(const irr::io::path& filename_);

	/// Load the pilot name from a pilot file.
	irr::io::path LoadPilotName(const irr::io::path& filename_) const;

	/// Set the totem type.
	void SetTotemType(Constants::Player::ETotemType::Enum type_);

	/// Returns the totem type.
	Constants::Player::ETotemType::Enum GetTotemType() const;

	/// Sets the music sound level.
	void SetSoundLevelMusic(float val_);

	/// Returns the music sound level.
	float GetSoundLevelMusic() const;

	/// Sets the effects sound level.
	void SetSoundLevelEffects(float val_);

	/// Returns the effects sound level.
	float GetSoundLevelEffects() const;

	/// Sets the mouse sensitivity.
	void SetMouseSensitivity(float val_);

	/// Returns the mouse sensitivity
	float GetMouseSensitivity() const;

	/// Sets if the y mouse axis shall be inverted in game.
	void SetMouseInvertY(bool val_);

	/// Returns if the y mouse axis shall be inverted in game.
	bool GetMouseInvertY() const;

	/// Sets a control.
	void SetControl(SControls::Enum control_, unsigned int value_);

	/// Returns a control.
	unsigned int GetControl(SControls::Enum control_) const;

	/// Returns the current pilot name.
	irr::io::path GetCurrentPilotName() const;

private:
	irr::io::IFileSystem* FileSystem;				///< The file system.
	CMusic* Music;									///> The music.
	CSoundEffects* SoundEffects;					///< The sound effects.

	Constants::Player::ETotemType::Enum TotemType;	///< The totem type.
	float SoundLevelMusic;							///< The music sound level.
	float SoundLevelEffects;						///< The effects sound level.
	float MouseSensitivity;							///< The mouse sensitivity.
	bool MouseInvertY;								///< Specifies, if the mouse y axis shall be inverted in game.
	unsigned int Controls[SControls::Number];		///< Player ship controls.

	irr::io::path CurrentPilot;						///< The current pilot name.
	
};

}

#endif
