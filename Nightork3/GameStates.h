// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef GAMESTATES_H
#define GAMESTATES_H

#include <irrlicht.h>
#include <irrKlang.h>
#include "Constants.h"
#include "Pilot.h"

namespace Nightork
{

class CCampaign;
class CCollisionManager;
class CDunGenProxy;
class CEventReceiver;
class CGameplayStats;
class CHighScores;
class CHud;
class CMenu;
class CMonsterManager;
class CMusic;
class CPlayer;
class CPowerUpManager;
class CProjectileManager;
class CSoundEffects;

class CGameStates
{
public:

	/// The available game states.
	struct EState
	{
		enum Enum
		{
			MainMenu,
			ChangePilot,
			PlayArena,
			Highscores,
			Settings,
			Credits,
			Explanations,
			InGame,
			Intro,
			Extro
		};
	};

public:

	/// Constructor.
	CGameStates(irr::IrrlichtDevice* irrDevice_, irrklang::ISoundEngine* irrKlangDevice_, CEventReceiver* eventReceiver_);
	/// Destructor.
	~CGameStates();

	/// Updates the game based on the current state.
	void Update();

	/// Changes the game state.
	void ChangeGameState(EState::Enum newState_, Constants::Level::EArena::Enum arena_ = Constants::Level::EArena::Number, unsigned int deaths_ = 0, unsigned int timeNeeded_ = 0);

	/// Returns the current game state.
	EState::Enum GetCurrentGameState();

	/// Creates a new pilot.
	void CreatePilot();

	/// Loads a pilot.
	void LoadPilot();

	/// Deletes a pilot.
	void DeletePilot();

	/// Sets a control.
	void SetControl(CPilot::SControls::Enum control_, unsigned int value_);

	/// Returns a control.
	unsigned int GetControl(CPilot::SControls::Enum control_) const;

	// Press button message.
	void PressButton(CPilot::SControls::Enum button_);

	/// Control button configured.
	void ButtonPressed(CPilot::SControls::Enum button_, unsigned int val_);

	/// Saves the settings.
	void SaveSettings();

	/// Sets settings to default.
	void DefaultSettings();

	/// Selects a totem
	void TotemSelected();

	/// Saves the selected totem.
	void SaveTotem();

private:

	/// Creates a instance of the in game.
	bool CreateGame();

	/// Destroys the in game.
	void DestroyGame();

private:

	irr::IrrlichtDevice* IrrDevice;				///< The irrlicht device.
	CEventReceiver* EventReceiver;				///< The event receiver.
	CPilot* Pilot;								///< The pilot access.
	CHighScores* HighScores;					///< The high score access.
	CDunGenProxy* DunGenProxy;					///< The access to the dungen library.
	CMenu* Menu;								///< The menu construction and destruction.
	CMusic* Music;								///> The music.

	CSoundEffects* SoundEffects;				///< The sound effects.
	CPlayer* Player;							///< The player.
	CMonsterManager* MonsterManager;			///< The monster manager.
	CCollisionManager* CollisionManager;		///< The collision manager.
	CProjectileManager* ProjectileManager;		///< The projectile manager.
	CPowerUpManager* PowerUpManager;			///< The powerup manager.
	CHud* Hud;									///< The heads up display.
	CGameplayStats* GameplayStats;				///< The gameplay stats.

	EState::Enum CurrentState;					///< The current program state.
	Constants::Level::EArena::Enum CurrentArena; ///< The currently played arena.
	clock_t Clock;								///< Own clock.
	clock_t LastTime;							///< Last measured time.

};

}

#endif
