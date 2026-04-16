// Copyright (C) 2011-2026 by Maximilian Hönig

#include <iostream>
#include <regex>
#include <time.h>
#include <thread>
#include <wchar.h>
#include "CollisionManager.h"
#include "DunGenProxy.h"
#include "EventReceiver.h"
#include "GameplayStats.h"
#include "GameStates.h"
#include "Helper.h"
#include "HighScores.h"
#include "Hud.h"
#include "Menu.h"
#include "MonsterManager.h"
#include "Music.h"
#include "Player.h"
#include "PowerUpManager.h"
#include "ProjectilManager.h"
#include "SoundEffects.h"

Nightork::CGameStates::CGameStates(irr::IrrlichtDevice* irrDevice_, irrklang::ISoundEngine* irrKlangDevice_, CEventReceiver* eventReceiver_)
	: IrrDevice(irrDevice_)
	, EventReceiver(eventReceiver_)
	, Pilot(new CPilot(irrDevice_))
	, HighScores(new CHighScores(irrDevice_))
	, DunGenProxy(new CDunGenProxy(irrDevice_))
	, Menu(new CMenu(irrDevice_, DunGenProxy, Pilot, HighScores))
	, CurrentState(EState::MainMenu)
	, Music(new CMusic(irrDevice_,irrKlangDevice_))
	, SoundEffects(new CSoundEffects(irrKlangDevice_))
	, Player(NULL)
	, MonsterManager(NULL)
	, CollisionManager(NULL)
	, ProjectileManager(NULL)
	, PowerUpManager(NULL)
	, Hud(NULL)
	, GameplayStats(NULL)
	, Clock(0)
	, LastTime(0)
{
	Menu->CreateBackground();
	Pilot->Initialize(Music, SoundEffects);
	ChangeGameState(CurrentState);
}

Nightork::CGameStates::~CGameStates()
{
	DestroyGame();
	delete SoundEffects;
	delete Music;
	delete Menu;
	delete DunGenProxy;
	delete HighScores;
	delete Pilot;
}

bool Nightork::CGameStates::CreateGame()
{
	// create dungeon
	if (!DunGenProxy->AssembleLevel())
		return false;

	// init sound effects
	SoundEffects->SetSoundLevel(Pilot->GetSoundLevelEffects());

	// create collision manager
	CollisionManager = new CCollisionManager(IrrDevice, DunGenProxy);

	/// create powerup manager
	PowerUpManager = new CPowerUpManager(IrrDevice);
	
	// create projectile manager
	ProjectileManager = new CProjectileManager(IrrDevice, CollisionManager, DunGenProxy);

	// create monster manager
	MonsterManager = new CMonsterManager(IrrDevice, SoundEffects, CollisionManager, PowerUpManager, ProjectileManager, DunGenProxy);

	// create gameplay stats
	GameplayStats = new CGameplayStats();

	// create player class
	Player = new CPlayer(IrrDevice, SoundEffects, EventReceiver, CollisionManager, PowerUpManager, ProjectileManager, GameplayStats, Pilot);

	ProjectileManager->Initialize(MonsterManager, Player);
	CollisionManager->Initialize(MonsterManager);

	// create hud
	Hud = new CHud(IrrDevice, EventReceiver, MonsterManager, Player, GameplayStats);

	Clock = 0;
	LastTime = clock();

	return true;
}

void Nightork::CGameStates::DestroyGame()
{
	delete Hud;
	Hud = NULL;
	delete Player;
	Player = NULL;
	delete GameplayStats;
	GameplayStats = NULL;
	delete ProjectileManager;
	ProjectileManager = NULL;
	delete PowerUpManager;
	PowerUpManager = NULL;
	delete MonsterManager;
	MonsterManager = NULL;
	delete CollisionManager;
	CollisionManager = NULL;
	SoundEffects->StopSounds();
}

void Nightork::CGameStates::Update()
{
    if (EState::InGame == CurrentState)
    {
		clock_t actTime = clock();
		clock_t passedTime = actTime - LastTime;
		LastTime = actTime;
		std::cout << "FRAME:" << passedTime << " , ";

		const bool notPaused = !EventReceiver->IsCancelGameOverlayActive();

		if (notPaused)
		{
			Clock += std::min(passedTime, 100l);

			// power up update
			actTime = clock();
			PowerUpManager->Update(Clock);
			std::cout << "PUMu:" << clock() - actTime << " , ";

			// projectiles update
			actTime = clock();
			ProjectileManager->Update(Clock);
			std::cout << "PMu:" << clock() - actTime << " , ";

			// position ship and camera update
			actTime = clock();
			Player->Update(Clock);
			std::cout << "Pu:" << clock() - actTime << " , ";

			// monster update
			actTime = clock();
			MonsterManager->Update(Clock);
			std::cout << "MMu:" << clock() - actTime << " , ";
		}

		// draw everything
		actTime = clock();
		IrrDevice->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 0, 0, 0));
		std::cout << "VDb:" << clock() - actTime << " , ";


		actTime = clock();
		IrrDevice->getSceneManager()->drawAll();
		std::cout << "SMd:" << clock() - actTime << " , ";

		actTime = clock();
		Hud->Update(Clock);
		std::cout << "Hu:" << clock() - actTime << " , ";

		actTime = clock();
		IrrDevice->getGUIEnvironment()->drawAll();
		std::cout << "GEd:" << clock() - actTime << " , ";

		actTime = clock();
		IrrDevice->getVideoDriver()->endScene();
		std::cout << "VDe:" << clock() - actTime << " , ";

		if (notPaused)
		{
			// cleanup
			actTime = clock();
			ProjectileManager->Cleanup();
			std::cout << "PMc:" << clock() - actTime << " , ";

			actTime = clock();
			MonsterManager->Cleanup();
			std::cout << "MMc:" << clock() - actTime << " , ";

			if (0 == MonsterManager->GetNumberMonsters()
				|| (Pilot->GetCurrentPilotName() == "GodSaveDaHamster" && EventReceiver->IsKeyPressed(irr::KEY_F7) && EventReceiver->IsKeyPressed(irr::KEY_F8)) // cheat mode
				)
				ChangeGameState(EState::Extro, Constants::Level::EArena::Number, GameplayStats->GetDeathCount(), GameplayStats->GetPlayTime(Clock));

			//std::this_thread::sleep_for(std::chrono::milliseconds(10));
			std::cout << std::endl;
		}
    }
    else
    {
        // draw everything
        IrrDevice->getVideoDriver()->beginScene(true, true, irr::video::SColor(255,0,0,0));
        IrrDevice->getSceneManager()->drawAll();
        Menu->Update(EState::Explanations == CurrentState);
        IrrDevice->getGUIEnvironment()->drawAll();
        IrrDevice->getVideoDriver()->endScene();
    }

    // play music
    switch (CurrentState)
    {
    case EState::InGame:
        {
            Music->Update(CMusic::EMusicStyle::Normal);
            break;
        }
    case EState::Intro:
        {
            Music->Update(CMusic::EMusicStyle::Intro);
            break;
        }
    case EState::Extro:
        {
            Music->Update(CMusic::EMusicStyle::Extro);
            break;
        }
    default:
        {
            Music->Update(CMusic::EMusicStyle::MainMenu);
            break;
        }
    }
}

namespace
{
	std::string GetArenaPath(Nightork::Constants::Level::EArena::Enum arena_)
	{
		return	std::string("arenas/") + std::string(arena_ == Nightork::Constants::Level::EArena::Sphere ? "Sphere.xml" : arena_ == Nightork::Constants::Level::EArena::Hive ? "Hive.xml" : "Cave.xml");
	}
}

void Nightork::CGameStates::ChangeGameState(EState::Enum newState_, Constants::Level::EArena::Enum arena_, unsigned int deaths_, unsigned int timeNeeded_)
{
	// remove old state
	switch (CurrentState)
	{
	case EState::InGame:
		{
			IrrDevice->getSceneManager()->clear();
			DestroyGame();
			if (EState::Extro != newState_)
				DunGenProxy->UnloadLevel();
			Menu->CreateBackground();
			break;
		}
	case EState::Extro:
		{
			DunGenProxy->UnloadLevel();
			break;
		}
	case EState::Intro:
		{
			if (EState::InGame == newState_)
			{
				static_cast<irr::gui::IGUIStaticText*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::INTROMENU_TEXT_LOADING, true))
					->setText(L"Loading, please wait...");
				Update(); // draw GUI
			}
			break;
		}
	case EState::ChangePilot:
		{
			if (Pilot->GetCurrentPilotName().empty())
			{
				Pilot->CreateNewPilot("Default");
				Pilot->LoadPilot("Default");
			}
			break;
		}
	}

	IrrDevice->getGUIEnvironment()->clear();
	IrrDevice->getCursorControl()->setVisible(EState::InGame != newState_);

	CurrentState = newState_;

	// create new state
	switch (newState_)
	{
	case EState::MainMenu:
	{
		Menu->CreateMainMenu();
		break;
	}
	case EState::InGame:
	{
		IrrDevice->getSceneManager()->clear();
		DunGenProxy->LoadLevel(GetArenaPath(CurrentArena).c_str());

		if (!CreateGame())
		{
			ChangeGameState(EState::Intro);
			static_cast<irr::gui::IGUIStaticText*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::INTROMENU_TEXT_LOADING, true))
				->setText(L"Error loading level.");
		}

		EventReceiver->ClearKeys();

		break;
	}
	case EState::Intro:
	{
		CurrentArena = arena_;
		DunGenProxy->ParseIntroExtro(GetArenaPath(arena_).c_str());
		Menu->CreateIntroMenu();
		break;
	}
	case EState::Extro:
	{
		HighScores->AddHighScore(Pilot->GetCurrentPilotName() == "" ? "empty" : Pilot->GetCurrentPilotName(), CurrentArena, deaths_, timeNeeded_);
		Menu->CreateExtroMenu(deaths_, timeNeeded_);
		break;
	}
	case EState::PlayArena:
	{
		Menu->CreatePlayLevelMenu();
		break;
	}
	case EState::ChangePilot:
	{
		Menu->CreatePilotMenu(irr::core::stringw("No errors."));
		break;
	}
	case EState::Highscores:
	{
		Menu->CreateHighScoreMenu();
		break;
	}
	case EState::Settings:
	{
		Menu->CreateSettingsMenu();
		break;
	}
	case EState::Credits:
	{
		Menu->CreateCreditsMenu();
		break;
	}
	case EState::Explanations:
	{
		Menu->CreateExplanationMenu();
		break;
	}
	}
}

Nightork::CGameStates::EState::Enum Nightork::CGameStates::GetCurrentGameState()
{
	return CurrentState;
}

void Nightork::CGameStates::CreatePilot()
{	
	irr::gui::IGUIEditBox* editBox = static_cast<irr::gui::IGUIEditBox*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::PILOTMENU_EDITBOX_PILOTNAME, true));
	
const irr::core::stringc name = editBox->getText();

	// Regex: Nur Buchstaben, Zahlen, Unterstrich und Minus erlaubt
	const std::regex validName("^[A-Za-z0-9_-]+$");
	irr::core::stringw status;

	if (16 < name.size())
	{
		status = irr::core::stringw("Maximum 16 characters allowed.");
	}
	else if (!std::regex_match(name.c_str(), validName))
	{
		status = irr::core::stringw("Only letters, numbers, underscore and minus allowed.");
	}
	else if (Pilot->CreateNewPilot(name))
	{
		status = irr::core::stringw("Pilot created.");
	}
	else
	{
		status = irr::core::stringw("Could not create pilot.");
	}

	IrrDevice->getGUIEnvironment()->clear();
	Menu->CreatePilotMenu(status);
}

void Nightork::CGameStates::LoadPilot()
{
	irr::gui::IGUIListBox* listBox = static_cast<irr::gui::IGUIListBox*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::PILOTMENU_LISTBOX_PILOTS, true));
	const int i = listBox->getSelected();
	irr::core::stringw status;
	if (0 <= i)
	{
		status = Pilot->LoadPilot(listBox->getListItem(i))
			? irr::core::stringw("Pilot loaded.") 
			: irr::core::stringw("Could not load pilot.");
	}
	else
	{
		status = irr::core::stringw("No pilot selected.");
	}

	IrrDevice->getGUIEnvironment()->clear();
	Menu->CreatePilotMenu(status);
}

void Nightork::CGameStates::DeletePilot()
{
	irr::gui::IGUIListBox* listBox = static_cast<irr::gui::IGUIListBox*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::PILOTMENU_LISTBOX_PILOTS, true));
	const int i = listBox->getSelected();
	irr::core::stringw status;
	if (0 <= i)
	{
		status = Pilot->DeletePilot(listBox->getListItem(i))
			? irr::core::stringw("Pilot deleted.") 
			: irr::core::stringw("Could not delete pilot.");
	}
	else
	{
		status = irr::core::stringw("No pilot selected.");
	}

	IrrDevice->getGUIEnvironment()->clear();
	Menu->CreatePilotMenu(status);
}

void Nightork::CGameStates::SetControl(CPilot::SControls::Enum control_, unsigned int value_)
{
	Pilot->SetControl(control_,value_);
}

unsigned int Nightork::CGameStates::GetControl(CPilot::SControls::Enum control_) const
{
	return Pilot->GetControl(control_);
}

void Nightork::CGameStates::PressButton(CPilot::SControls::Enum button_)
{
	Menu->PressButton(button_);
}

void Nightork::CGameStates::ButtonPressed(CPilot::SControls::Enum button_, unsigned int val_)
{
	Menu->ButtonPressed(button_, val_);
}

void Nightork::CGameStates::SaveSettings()
{
	irr::gui::IGUIScrollBar* scrollBar = static_cast<irr::gui::IGUIScrollBar*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_SCROLLBAR_SOUND_MUSIC, true));
	Pilot->SetSoundLevelMusic(scrollBar->getPos()*0.01f);
	scrollBar = static_cast<irr::gui::IGUIScrollBar*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_SCROLLBAR_SOUND_EFFECTS, true));
	Pilot->SetSoundLevelEffects(scrollBar->getPos()*0.01f);
	scrollBar = static_cast<irr::gui::IGUIScrollBar*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_SCROLLBAR_MOUSE_SENSITIVITY, true));
	Pilot->SetMouseSensitivity(scrollBar->getPos()*0.01f);
	irr::gui::IGUICheckBox* checkBox = static_cast<irr::gui::IGUICheckBox*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_CHECKBOX_MOUSE_INVERT_Y_AXIS, true));
	Pilot->SetMouseInvertY(checkBox->isChecked());

	irr::gui::IGUIButton* button = static_cast<irr::gui::IGUIButton*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_FORWARD, true));
	Pilot->SetControl(CPilot::SControls::Forward, atoi(button->getName()));
	button = static_cast<irr::gui::IGUIButton*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_BACKWARD, true));
	Pilot->SetControl(CPilot::SControls::Backward, atoi(button->getName()));
	button = static_cast<irr::gui::IGUIButton*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_LEFT, true));
	Pilot->SetControl(CPilot::SControls::Left, atoi(button->getName()));
	button = static_cast<irr::gui::IGUIButton*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_RIGHT, true));
	Pilot->SetControl(CPilot::SControls::Right, atoi(button->getName()));
	button = static_cast<irr::gui::IGUIButton*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_AFTERBURNER, true));
	Pilot->SetControl(CPilot::SControls::Afterburner, atoi(button->getName()));
	button = static_cast<irr::gui::IGUIButton*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_HIGHLIGHTING, true));
	Pilot->SetControl(CPilot::SControls::Highlighting, atoi(button->getName()));
	button = static_cast<irr::gui::IGUIButton*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_WEAPON_1, true));
	Pilot->SetControl(CPilot::SControls::Weapon1, atoi(button->getName()));
	button = static_cast<irr::gui::IGUIButton*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_WEAPON_2, true));
	Pilot->SetControl(CPilot::SControls::Weapon2, atoi(button->getName()));

	Pilot->SaveCurrentPilot();

	Music->SetSoundLevel(Pilot->GetSoundLevelMusic());
}

void Nightork::CGameStates::DefaultSettings()
{
	irr::gui::IGUIScrollBar* scrollBar = static_cast<irr::gui::IGUIScrollBar*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_SCROLLBAR_SOUND_MUSIC, true));
	scrollBar->setPos(50);
	scrollBar = static_cast<irr::gui::IGUIScrollBar*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_SCROLLBAR_SOUND_EFFECTS, true));
	scrollBar->setPos(50);
	scrollBar = static_cast<irr::gui::IGUIScrollBar*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_SCROLLBAR_MOUSE_SENSITIVITY, true));
	scrollBar->setPos(100);
	irr::gui::IGUICheckBox* checkBox = static_cast<irr::gui::IGUICheckBox*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::SETTINGSMENU_CHECKBOX_MOUSE_INVERT_Y_AXIS, true));
	checkBox->setChecked(false);

	Menu->ButtonPressed(CPilot::SControls::Forward, irr::KEY_KEY_W);
	Menu->ButtonPressed(CPilot::SControls::Backward, irr::KEY_KEY_S);
	Menu->ButtonPressed(CPilot::SControls::Left, irr::KEY_KEY_A);
	Menu->ButtonPressed(CPilot::SControls::Right, irr::KEY_KEY_D);
	Menu->ButtonPressed(CPilot::SControls::Afterburner, irr::KEY_SPACE);
	Menu->ButtonPressed(CPilot::SControls::Highlighting, irr::KEY_KEY_R);
	Menu->ButtonPressed(CPilot::SControls::Weapon1, irr::KEY_KEY_CODES_COUNT + 0); // mouse button 1;
	Menu->ButtonPressed(CPilot::SControls::Weapon2, irr::KEY_KEY_CODES_COUNT + 1); // mouse button 2;
}

void Nightork::CGameStates::TotemSelected()
{
	Menu->SelectTotem();
}

void Nightork::CGameStates::SaveTotem()
{
	irr::gui::IGUIComboBox* comboBox = static_cast<irr::gui::IGUIComboBox*>(IrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(CMenu::EGuiId::PILOTMENU_COMBOBOX_TOTEM, true));
	Pilot->SetTotemType(static_cast<Constants::Player::ETotemType::Enum>(comboBox->getSelected()));
	Pilot->SaveCurrentPilot();
}
