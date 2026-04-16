// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef MENU_H
#define MENU_H

#include <irrlicht.h>
#include "Pilot.h"
#include "Constants.h"

namespace Nightork
{

class CCredits;
class CDunGenProxy;
class CHighScores;

class CMenu
{
public:

	/// Gui elements.
	struct EGuiId
	{
		enum Enum
		{
			MAINMENU_BUTTON_CHANGE_PILOT,
			MAINMENU_BUTTON_PLAY_LEVEL,
			MAINMENU_BUTTON_EXPLANATIONS,
			MAINMENU_BUTTON_HIGHSCORES,
			MAINMENU_BUTTON_SETTINGS,
			MAINMENU_BUTTON_CREDITS,
			MAINMENU_BUTTON_EXIT,
			MAINMENU_TEXT_CURRENT_PILOT,
			MAINMENU_TEXT_CHANGE_PILOT,
			MAINMENU_TEXT_COPYRIGHT,

			INTROMENU_BUTTON_START,
			INTROMENU_BUTTON_EXIT,
			INTROMENU_TEXT_LOADING,
			INTROMENU_TEXT_STATS,
			INTROMENU_TEXTBOX_DESCRIPTION,

			EXTROMENU_BUTTON_EXIT,
			EXTROMENU_TEXT_STATS,
			EXTROMENU_TEXTBOX_DESCRIPTION,

			PILOTMENU_BUTTON_NEW_PILOT,
			PILOTMENU_BUTTON_LOAD_PILOT,
			PILOTMENU_BUTTON_DELETE_PILOT,
			PILOTMENU_BUTTON_SAVE_TOTEM,
			PILOTMENU_BUTTON_RESET_TOTEM,
			PILOTMENU_BUTTON_EXIT,
			PILOTMENU_LISTBOX_PILOTS,
			PILOTMENU_EDITBOX_PILOTNAME,
			PILOTMENU_COMBOBOX_TOTEM,
			PILOTMENU_TEXT_TOTEMS,
			PILOTMENU_TEXT_AVAILABLE_PILOTS,
			PILOTMENU_TEXT_CURRENT_PILOT,
			PILOTMENU_TEXT_SELECT_PILOT,
			PILOTMENU_TEXT_PILOT_LEVEL,
			PILOTMENU_TEXT_PILOT_DEATHS,
			PILOTMENU_TEXT_PILOT_TIME,
			PILOTMENU_TEXT_FILE_STATUS,
			PILOTMENU_TEXTBOX_TOTEM_DESCRIPTION,

			LEVELMENU_BUTTON_SPHERE,
			LEVELMENU_BUTTON_HIVE,
			LEVELMENU_BUTTON_CAVE,
			LEVELMENU_BUTTON_EXIT,

			SETTINGSMENU_SCROLLBAR_SOUND_MUSIC,
			SETTINGSMENU_SCROLLBAR_SOUND_EFFECTS,
			SETTINGSMENU_SCROLLBAR_MOUSE_SENSITIVITY,
			SETTINGSMENU_CHECKBOX_MOUSE_INVERT_Y_AXIS,
			SETTINGSMENU_BUTTON_EXIT,
			SETTINGSMENU_BUTTON_SAVE,
			SETTINGSMENU_BUTTON_DEFAULTS,
			SETTINGSMENU_BUTTON_CONTROL_FORWARD,
			SETTINGSMENU_BUTTON_CONTROL_BACKWARD,
			SETTINGSMENU_BUTTON_CONTROL_LEFT,
			SETTINGSMENU_BUTTON_CONTROL_RIGHT,
			SETTINGSMENU_BUTTON_CONTROL_AFTERBURNER,
			SETTINGSMENU_BUTTON_CONTROL_HIGHLIGHTING,
			SETTINGSMENU_BUTTON_CONTROL_WEAPON_1,
			SETTINGSMENU_BUTTON_CONTROL_WEAPON_2,
			SETTINGSMENU_TEXT_SOUND_MUSIC,
			SETTINGSMENU_TEXT_SOUND_EFFECTS,
			SETTINGSMENU_TEXT_MOUSE_SENSITIVITY,
			SETTINGSMENU_TEXT_MOUSE_INVERT_Y_AXIS,
			SETTINGSMENU_TEXT_CONTROL_FORWARD,
			SETTINGSMENU_TEXT_CONTROL_BACKWARD,
			SETTINGSMENU_TEXT_CONTROL_LEFT,
			SETTINGSMENU_TEXT_CONTROL_RIGHT,
			SETTINGSMENU_TEXT_CONTROL_AFTERBURNER,
			SETTINGSMENU_TEXT_CONTROL_HIGHLIGHTING,
			SETTINGSMENU_TEXT_CONTROL_WEAPON_1,
			SETTINGSMENU_TEXT_CONTROL_WEAPON_2,

			HIGHSCOREMENU_BUTTON_EXIT,
			HIGHSCOREMENU_TEXT_HIGHSCORES,

			CREDITSMENU_BUTTON_EXIT,
			CREDITSMENU_TEXTBOX_CREDITS,

			EXPLANATIONMENU_BUTTON_EXIT
		};
	};

public:
	/// Constructor.
	CMenu(irr::IrrlichtDevice* irrDevice_, CDunGenProxy* dunGen_, const CPilot* pilot_, const CHighScores* highScores_);
	/// Destructor.
	~CMenu();

	/// Draws the Nightork Logo and further stuff needed.
	void Update(bool drawExplanations_);

	/// Creates the background scene for the menus.
	void CreateBackground();

	/// Creates the main menu.
	void CreateMainMenu();

	/// Creates the intro menu.
	void CreateIntroMenu();

	/// Creates the extro menu.
	void CreateExtroMenu(unsigned int deaths_, unsigned int timeNeeded_);

	/// Creates the pilot menu.
	void CreatePilotMenu(const irr::core::stringw& status_);

	/// Creates the play level menu.
	void CreatePlayLevelMenu();

	/// Creates the settings menu.
	void CreateSettingsMenu();

	/// Creates the high score menu.
	void CreateHighScoreMenu();

	/// Creates the credits menu.
	void CreateCreditsMenu();

	/// Create the explanations menu.
	void CreateExplanationMenu();

	// Press button message.
	void PressButton(CPilot::SControls::Enum button_);

	/// Control button configured.
	void ButtonPressed(CPilot::SControls::Enum button_, unsigned int val_);

	// Select Totem
	void SelectTotem();

private:
	void AddFileListToListBox(irr::gui::IGUIListBox* listBox_, irr::io::IFileList* fileList_);

private:
	irr::gui::IGUIEnvironment* GuiEnvironment;	///< The gui environement.
	irr::video::IVideoDriver* VideoDriver;		///< The video driver.
	irr::scene::ISceneManager* SceneManager;	///< The scene manager.
	irr::io::IFileSystem* FileSystem;			///< The file system.
	CDunGenProxy* DunGenProxy;					///< The access to dungen.
	const CPilot* Pilot;						///< The access to the pilot stats.
	const CHighScores* HighScores;				///< The access to the high scores.
	CCredits* Credits;							///< The access to the credits.
	irr::core::stringw TotemDesriptions[Constants::Player::ETotemType::Number];	///< The descriptions of the totems.
	std::wstring MemoryString;					///< String to save data from destruction.
};

}

#endif
