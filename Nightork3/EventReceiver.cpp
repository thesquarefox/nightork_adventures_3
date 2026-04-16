// Copyright (C) 2011-2026 by Maximilian Hönig

#include <iostream>
#include "Constants.h"
#include "EventReceiver.h"
#include "Gamestates.h"
#include "Menu.h"

Nightork::CEventReceiver::CEventReceiver()
	: CursorControl(NULL)
	, ActiveControlButton(CPilot::SControls::Number)
	, CancelGameOverlayActive(false)
	, EndProgram(false)
{
	ClearKeys();
}

Nightork::CEventReceiver::~CEventReceiver()
{
}

void Nightork::CEventReceiver::Initialize(irr::IrrlichtDevice* irrDevice_, CGameStates* gameStates_)
{
	CursorControl = irrDevice_->getCursorControl();
	VideoDriver = irrDevice_->getVideoDriver();
	GameStates = gameStates_;
}

bool Nightork::CEventReceiver::IsKeyPressed(irr::EKEY_CODE keyCode_) const
{
	return KeyPressed[keyCode_];
}

bool Nightork::CEventReceiver::IsManeuver(CPilot::SControls::Enum maneuver_)
{
	const unsigned int control = GameStates->GetControl(maneuver_);

	switch (maneuver_)
	{
	case CPilot::SControls::Weapon1:
	case CPilot::SControls::Weapon2:
		{
			bool result = KeyPressed[control] || KeyWasPressed[control];
			KeyWasPressed[control] = false;
			return result;
		}
	default:
		{
			return KeyPressed[control];
		}
	}
}

bool Nightork::CEventReceiver::IsCancelGameOverlayActive()
{
	return CancelGameOverlayActive;
}

bool Nightork::CEventReceiver::GetEndProgram()
{
	return EndProgram;
}

irr::core::position2df Nightork::CEventReceiver::GetRelativeMousePosition()
{
	// compute mouse position relative to window center
	irr::core::position2df temp = CursorControl->getRelativePosition() - irr::core::position2df(0.5f,0.5f);

	const float MAX_DELTA = 0.3f;

	if (temp.X < -MAX_DELTA)
		temp.X = -MAX_DELTA;
	if (temp.Y < -MAX_DELTA)
		temp.Y = -MAX_DELTA;
	if (temp.X > MAX_DELTA)
		temp.X = MAX_DELTA;
	if (temp.Y > MAX_DELTA)
		temp.Y = MAX_DELTA;
	
	CursorControl->setPosition(0.5f, 0.5f);
	
	return temp;
}

bool Nightork::CEventReceiver::OnEvent(const irr::SEvent& event_)
{
	// configuration of controls
	if (CPilot::SControls::Number != ActiveControlButton)
	{
		switch (event_.EventType)
		{
		case irr::EET_MOUSE_INPUT_EVENT:
			{
				switch (event_.MouseInput.Event)
				{
				case irr::EMIE_LMOUSE_PRESSED_DOWN:
					{
						GameStates->ButtonPressed(ActiveControlButton, irr::KEY_KEY_CODES_COUNT+0);
						ActiveControlButton = CPilot::SControls::Number;
						break;
					}
				case irr::EMIE_RMOUSE_PRESSED_DOWN:
					{
						GameStates->ButtonPressed(ActiveControlButton, irr::KEY_KEY_CODES_COUNT+1);
						ActiveControlButton = CPilot::SControls::Number;
						break;
					}
				case irr::EMIE_MMOUSE_PRESSED_DOWN:
					{
						GameStates->ButtonPressed(ActiveControlButton, irr::KEY_KEY_CODES_COUNT+2);
						ActiveControlButton = CPilot::SControls::Number;
						break;
					}
				}
				break;
			}

		case irr::EET_KEY_INPUT_EVENT:
				{
					GameStates->ButtonPressed(ActiveControlButton, event_.KeyInput.Key);
					ActiveControlButton = CPilot::SControls::Number;
					break;
				}
		}

		return true;
	}

	// normal event handling
	switch (event_.EventType)
	{
	case irr::EET_KEY_INPUT_EVENT:
		{
			KeyPressed[event_.KeyInput.Key] = event_.KeyInput.PressedDown;
			if (event_.KeyInput.PressedDown)
			{
				KeyWasPressed[event_.KeyInput.Key] = true;
			}

			if (CGameStates::EState::InGame == GameStates->GetCurrentGameState())
			{
				switch(event_.KeyInput.Key)
				{
				case irr::KEY_ESCAPE:
					{
						CancelGameOverlayActive = true;
						break;
					}
				case irr::KEY_KEY_N:
					{
						CancelGameOverlayActive = false;
						break;
					}
				case irr::KEY_KEY_Y:
					{
						if (CancelGameOverlayActive)
							GameStates->ChangeGameState(CGameStates::EState::MainMenu);
						break;
					}
				}
			}
			break;
		}
	case irr::EET_MOUSE_INPUT_EVENT:
		{
			if (CGameStates::EState::InGame == GameStates->GetCurrentGameState())
			{
				switch (event_.MouseInput.Event)
				{
				case irr::EMIE_LMOUSE_PRESSED_DOWN:
					{
						KeyPressed[irr::KEY_KEY_CODES_COUNT+0] = true;
						KeyWasPressed[irr::KEY_KEY_CODES_COUNT+0] = true;
						break;
					}
				case irr::EMIE_LMOUSE_LEFT_UP:
					{
						KeyPressed[irr::KEY_KEY_CODES_COUNT+0] = false;
						break;
					}
				case irr::EMIE_RMOUSE_PRESSED_DOWN:
					{
						KeyPressed[irr::KEY_KEY_CODES_COUNT+1] = true;
						KeyWasPressed[irr::KEY_KEY_CODES_COUNT+1] = true;
						break;
					}
				case irr::EMIE_RMOUSE_LEFT_UP:
					{
						KeyPressed[irr::KEY_KEY_CODES_COUNT+1] = false;
						break;
					}
				case irr::EMIE_MMOUSE_PRESSED_DOWN:
					{
						KeyPressed[irr::KEY_KEY_CODES_COUNT+2] = true;
						KeyWasPressed[irr::KEY_KEY_CODES_COUNT+2] = true;
						break;
					}
				case irr::EMIE_MMOUSE_LEFT_UP:
					{
						KeyPressed[irr::KEY_KEY_CODES_COUNT+2] = false;
						break;
					}
				}
			}
			
			break;
		}
	case irr::EET_GUI_EVENT:
		{
			irr::s32 id = event_.GUIEvent.Caller->getID();
			switch(event_.GUIEvent.EventType)
			{
			case irr::gui::EGET_BUTTON_CLICKED:
				{
					switch(id)
					{
					case CMenu::EGuiId::MAINMENU_BUTTON_CHANGE_PILOT:
					case CMenu::EGuiId::PILOTMENU_BUTTON_RESET_TOTEM:
						{
							GameStates->ChangeGameState(CGameStates::EState::ChangePilot);
							break;
						}
					case CMenu::EGuiId::MAINMENU_BUTTON_PLAY_LEVEL:
						{
							GameStates->ChangeGameState(CGameStates::EState::PlayArena);
							break;
						}
					case CMenu::EGuiId::MAINMENU_BUTTON_HIGHSCORES:
						{
							GameStates->ChangeGameState(CGameStates::EState::Highscores);
							break;
						}
					case CMenu::EGuiId::MAINMENU_BUTTON_SETTINGS:
						{
							GameStates->ChangeGameState(CGameStates::EState::Settings);
							break;
						}
					case CMenu::EGuiId::MAINMENU_BUTTON_CREDITS:
						{
							GameStates->ChangeGameState(CGameStates::EState::Credits);
							break;
						}
					case CMenu::EGuiId::MAINMENU_BUTTON_EXPLANATIONS:
						{
							GameStates->ChangeGameState(CGameStates::EState::Explanations);
							break;
						}
					case CMenu::EGuiId::MAINMENU_BUTTON_EXIT:
						{
							EndProgram = true;
							break;
						}
					case CMenu::EGuiId::INTROMENU_BUTTON_START:
						{
							CancelGameOverlayActive = false;
							for (int i=0; i<irr::KEY_KEY_CODES_COUNT+3; i++)
							{
								KeyPressed[i] = false;
								KeyWasPressed[i] = false;
							}
							GameStates->ChangeGameState(CGameStates::EState::InGame);
							break;
						}
					case CMenu::EGuiId::PILOTMENU_BUTTON_NEW_PILOT:
						{
							GameStates->CreatePilot();
							break;
						}
					case CMenu::EGuiId::PILOTMENU_BUTTON_LOAD_PILOT:
						{
							GameStates->LoadPilot();
							break;
						}
					case CMenu::EGuiId::PILOTMENU_BUTTON_DELETE_PILOT:
						{
							GameStates->DeletePilot();
							break;
						}
					case CMenu::EGuiId::LEVELMENU_BUTTON_SPHERE:
						{
							GameStates->ChangeGameState(CGameStates::EState::Intro, Constants::Level::EArena::Sphere);
							break;
						}
					case CMenu::EGuiId::LEVELMENU_BUTTON_HIVE:
						{
						GameStates->ChangeGameState(CGameStates::EState::Intro, Constants::Level::EArena::Hive);
							break;
						}
					case CMenu::EGuiId::LEVELMENU_BUTTON_CAVE:
						{
						GameStates->ChangeGameState(CGameStates::EState::Intro, Constants::Level::EArena::Cave);
							break;
						}
					case CMenu::EGuiId::SETTINGSMENU_BUTTON_SAVE:
						{
							GameStates->SaveSettings();
							GameStates->ChangeGameState(CGameStates::EState::MainMenu);
							break;
						}
					case CMenu::EGuiId::SETTINGSMENU_BUTTON_DEFAULTS:
						{
							GameStates->DefaultSettings();
							break;
						}
	  				case CMenu::EGuiId::EXTROMENU_BUTTON_EXIT:
					case CMenu::EGuiId::LEVELMENU_BUTTON_EXIT:
					case CMenu::EGuiId::PILOTMENU_BUTTON_EXIT:
					case CMenu::EGuiId::SETTINGSMENU_BUTTON_EXIT:
					case CMenu::EGuiId::HIGHSCOREMENU_BUTTON_EXIT:
					case CMenu::EGuiId::CREDITSMENU_BUTTON_EXIT:
					case CMenu::EGuiId::EXPLANATIONMENU_BUTTON_EXIT:
						{
							GameStates->ChangeGameState(CGameStates::EState::MainMenu);
							break;
						}
					case CMenu::EGuiId::INTROMENU_BUTTON_EXIT:
						{
							GameStates->ChangeGameState(CGameStates::EState::PlayArena);
							break;
						}
					case CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_FORWARD:
						{
							ActiveControlButton = CPilot::SControls::Forward;
							GameStates->PressButton(ActiveControlButton);
							break;
						}
					case CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_BACKWARD:
						{
							ActiveControlButton = CPilot::SControls::Backward;
							GameStates->PressButton(ActiveControlButton);
							break;
						}
					case CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_LEFT:
						{
							ActiveControlButton = CPilot::SControls::Left;
							GameStates->PressButton(ActiveControlButton);
							break;
						}
					case CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_RIGHT:
						{
							ActiveControlButton = CPilot::SControls::Right;
							GameStates->PressButton(ActiveControlButton);
							break;
						}
					case CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_AFTERBURNER:
						{
							ActiveControlButton = CPilot::SControls::Afterburner;
							GameStates->PressButton(ActiveControlButton);
							break;
						}
					case CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_HIGHLIGHTING:
						{
							ActiveControlButton = CPilot::SControls::Highlighting;
							GameStates->PressButton(ActiveControlButton);
							break;
						}
					case CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_WEAPON_1:
						{
							ActiveControlButton = CPilot::SControls::Weapon1;
							GameStates->PressButton(ActiveControlButton);
							break;
						}
					case CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_WEAPON_2:
						{
							ActiveControlButton = CPilot::SControls::Weapon2;
							GameStates->PressButton(ActiveControlButton);
							break;
						}
					case CMenu::EGuiId::PILOTMENU_BUTTON_SAVE_TOTEM:
						{
							GameStates->SaveTotem();
							break;
						}
					}
					break;
				}
			case irr::gui::EGET_COMBO_BOX_CHANGED:
				{
					switch(id)
					{
					case CMenu::EGuiId::PILOTMENU_COMBOBOX_TOTEM:
						{
							GameStates->TotemSelected();
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}
	return false;
}

void Nightork::CEventReceiver::ClearKeys()
{
	for (int i = 0; i < irr::KEY_KEY_CODES_COUNT + 3; i++)
	{
		KeyPressed[i] = false;
		KeyWasPressed[i] = false;
	}
}
