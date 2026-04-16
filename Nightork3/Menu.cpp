// Copyright (C) 2011-2026 by Maximilian Hönig

#include <fstream>
#include <string>
#include "CGUITextBox.h"
#include "Credits.h"
#include "DunGenProxy.h"
#include "Helper.h"
#include "HighScores.h"
#include "Menu.h"

namespace
{
	static std::wstring KeyNames[] = 
	{
		L"-",
		L"Left Button",
		L"Right Button",
		L"Cancel",
		L"Middle Button",
		L"X Button 1",
		L"X Button 2",
		L"-",
		L"Back",
		L"Tab",
		L"-",
		L"-",
		L"Clear",
		L"Return",
		L"-",
		L"-",
		L"Shift",
		L"Control",
		L"Menu",
		L"Pause",
		L"Capital",
		L"Kana",
		L"-",	
		L"Junja",
		L"Final",
		L"Kanji",
		L"-",
		L"Escape",
		L"Convert",
		L"Nonconvert",
		L"Accept",
		L"Mode Change",
		L"Space",
		L"Priot",
		L"Next",
		L"End",
		L"Home",
		L"Left",
		L"Up",
		L"Right",
		L"Down",
		L"Select",
		L"Print",
		L"Execute",
		L"Snapshot",
		L"Insert",
		L"Delete",
		L"Help",
		L"0",
		L"1",
		L"2",
		L"3",
		L"4",
		L"5",
		L"6",
		L"7",
		L"8",
		L"9",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"A",
		L"B",
		L"C",
		L"D",
		L"E",
		L"F",
		L"G",
		L"H",
		L"I",
		L"J",
		L"K",
		L"L",
		L"M",
		L"N",
		L"O",
		L"P",
		L"Q",
		L"R",
		L"S",
		L"T",
		L"U",
		L"V",
		L"W",
		L"X",
		L"Y",
		L"Z",
		L"Left Windows",
		L"Right Windows",
		L"Apps",
		L"-",
		L"Sleep",
		L"Numpad 0",
		L"Numpad 1",
		L"Numpad 2",
		L"Numpad 3",
		L"Numpad 4",
		L"Numpad 5",
		L"Numpad 6",
		L"Numpad 7",
		L"Numpad 8",
		L"Numpad 9",
		L"Numpad *",
		L"Numpad +",
		L"Numpad /",
		L"Numpad -",
		L"Numpad .",
		L"Numpad /",
		L"F1",
		L"F2",
		L"F3",
		L"F4",
		L"F5",
		L"F6",
		L"F7",
		L"F8",
		L"F9",
		L"F10",
		L"F11",
		L"F12",
		L"F13",
		L"F14",
		L"F15",
		L"F16",
		L"F17",
		L"F18",
		L"F19",
		L"F20",
		L"F21",
		L"F22",
		L"F23",
		L"F24",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"Num Lock",
		L"Scroll Lock",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"Left Shift",
		L"Right Shight",
		L"Left Control",
		L"Right Control",
		L"Left Menu",
		L"Right Menu",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"Plus",
		L"Comma",
		L"Minus",
		L"Period",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"-",
		L"Attn",
		L"CrSel",
		L"ExSel",
		L"Erase OEF",
		L"Play",
		L"Zoom",
		L"PA1",
		L"OEM Clear",
		L"-",
		L"-",
		L"L.M.Button",
		L"R.M.Button",
		L"M.M.Button"
	};

	Nightork::CMenu::EGuiId::Enum GetIdFromControl(Nightork::CPilot::SControls::Enum control_)
	{
		switch (control_)
		{
		case Nightork::CPilot::SControls::Backward:
			return Nightork::CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_BACKWARD;
		case Nightork::CPilot::SControls::Left:
			return Nightork::CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_LEFT;
		case Nightork::CPilot::SControls::Right:
			return Nightork::CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_RIGHT;
		case Nightork::CPilot::SControls::Afterburner:
			return Nightork::CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_AFTERBURNER;
		case Nightork::CPilot::SControls::Highlighting:
			return Nightork::CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_HIGHLIGHTING;
		case Nightork::CPilot::SControls::Weapon1:
			return Nightork::CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_WEAPON_1;
		case Nightork::CPilot::SControls::Weapon2:
			return Nightork::CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_WEAPON_2;
		}
		return Nightork::CMenu::EGuiId::SETTINGSMENU_BUTTON_CONTROL_FORWARD;
	}
}

Nightork::CMenu::CMenu(irr::IrrlichtDevice* irrDevice_, CDunGenProxy* dunGen_, const CPilot* pilot_, const CHighScores* highScores_)
	: GuiEnvironment(irrDevice_->getGUIEnvironment())
	, VideoDriver(irrDevice_->getVideoDriver())
	, SceneManager(irrDevice_->getSceneManager())
	, FileSystem(irrDevice_->getFileSystem())
	, DunGenProxy(dunGen_)
	, Pilot(pilot_)
	, HighScores(highScores_)
	, Credits(new CCredits(irrDevice_))
{

	const irr::video::SColor foregroudColor(255, 200, 120, 10);
	const irr::video::SColor shadowColor(255, 0, 0, 0);
	const irr::video::SColor borderColor(255, 0, 0, 255);

	// set hud font
	GuiEnvironment->getSkin()->setFont(GuiEnvironment->getFont("data/HudFont.xml"));
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_3D_DARK_SHADOW, shadowColor);
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_3D_FACE, foregroudColor);
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_3D_HIGH_LIGHT, foregroudColor);
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_3D_LIGHT, foregroudColor);
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_3D_SHADOW, foregroudColor);

	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_ACTIVE_BORDER, borderColor);
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_ACTIVE_CAPTION, irr::video::SColor(255,180,120,10));
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_APP_WORKSPACE, borderColor);
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_COUNT, shadowColor);

	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_GRAY_TEXT, borderColor);
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_HIGH_LIGHT, irr::video::SColor(255,170,200,10));
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_HIGH_LIGHT_TEXT, irr::video::SColor(255,0,0,0));
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_ICON, borderColor);
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_ICON_HIGH_LIGHT, borderColor);
													
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_INACTIVE_BORDER, borderColor);
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_INACTIVE_CAPTION, borderColor);
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_SCROLLBAR, irr::video::SColor(255,150,120,10));
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_TOOLTIP, borderColor);
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_TOOLTIP_BACKGROUND, borderColor);

	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_WINDOW, borderColor);
	GuiEnvironment->getSkin()->setColor(irr::gui::EGDC_WINDOW_SYMBOL, shadowColor);

	// create totem descriptions
	TotemDesriptions[Constants::Player::ETotemType::IceBear] = irr::core::stringw("The sturdy Ice Bear has increased survival capabilities.\n");
	TotemDesriptions[Constants::Player::ETotemType::SnowTiger] = irr::core::stringw("The fierce Snow Tiger has increased fighting capabilities.\n");
	TotemDesriptions[Constants::Player::ETotemType::FrostRaven] = irr::core::stringw("The cunning Frost Raven has increased movement capabilities.\n");
	for (unsigned int i=0; i<Constants::Player::ETotemType::Number; ++i)
	{
		TotemDesriptions[i] += irr::core::stringw("\nHitpoints: ") + irr::core::stringw(Constants::Player::HitpointMaximum[i]);
		TotemDesriptions[i] += irr::core::stringw("\nOne point per every: ") + irr::core::stringw(Constants::Player::TickHpRegen[i]) + irr::core::stringw(" ms.");
		TotemDesriptions[i] += irr::core::stringw("\nWeapon energy: ") + irr::core::stringw(Constants::Player::Projectile::Maximum[i]);
		TotemDesriptions[i] += irr::core::stringw("\nOne point per every: ") + irr::core::stringw(Constants::Player::Projectile::TickCharge[i]) + irr::core::stringw(" ms.");
		TotemDesriptions[i] += irr::core::stringw("\nAfterburner energy: ") + irr::core::stringw(Constants::Player::Afterburner::Maximum[i]);
		TotemDesriptions[i] += irr::core::stringw("\nOne point per every: ") + irr::core::stringw(Constants::Player::Afterburner::TickCharge[i]) + irr::core::stringw(" ms.");
	}
}

Nightork::CMenu::~CMenu()
{
	delete Credits;
}

void Nightork::CMenu::Update(bool drawExplanations_)
{
	const irr::core::dimension2du screenSize = VideoDriver->getScreenSize();

	const float baseWidth = 1600.0f;
	const float baseHeight = 900.0f;
	const float width = static_cast<float>(screenSize.Width);
	const float height = static_cast<float>(screenSize.Height);

	const float ratio = std::min(width / baseWidth, height / baseHeight);

	const unsigned int imgBaseWidth = 800;
	const unsigned int imgBaseHeight = 150;
	const unsigned int imgWidth = static_cast<unsigned int>(imgBaseWidth * ratio);
	const unsigned int imgHeight = static_cast<unsigned int>(imgBaseHeight * ratio);

	const unsigned int imgX = (screenSize.Width - imgWidth) / 2;
	const unsigned int imgY = 0;

	const irr::video::ITexture* texture = VideoDriver->getTexture("data/Nightork-Logo.png");
	VideoDriver->draw2DImage(texture, irr::core::recti(imgX, imgY, imgX + imgWidth, imgY + imgHeight),
		irr::core::recti(0, 0, imgBaseWidth, imgBaseHeight), 0, 0, true);

	if (drawExplanations_)
	{
		const unsigned int imgBaseWidth = 1350;
		const unsigned int imgBaseHeight = 680;
		const unsigned int imgWidth = static_cast<unsigned int>(imgBaseWidth * ratio);
		const unsigned int imgHeight = static_cast<unsigned int>(imgBaseHeight * ratio);
		const unsigned int imgHeightOffset = static_cast<unsigned int>(60 * ratio);
		
		const unsigned int imgX = (screenSize.Width - imgWidth) / 2;
		const unsigned int imgY = (screenSize.Height - imgHeight + imgHeightOffset) / 2;

		const irr::video::ITexture* texture = VideoDriver->getTexture("data/Explanation-Screen.png");
		VideoDriver->draw2DImage(texture, irr::core::recti(imgX, imgY, imgX + imgWidth, imgY + imgHeight),
			irr::core::recti(0, 0, imgBaseWidth, imgBaseHeight));
	}
}

void Nightork::CMenu::CreateBackground()
{
	irr::scene::IAnimatedMesh* mesh = SceneManager->getMesh("data/Orc.X");
	irr::scene::IAnimatedMeshSceneNode* node = SceneManager->addAnimatedMeshSceneNode(mesh,0,-1,
		irr::core::vector3df(-3.0f,-3.5f,10.0f),
		irr::core::vector3df(0.0f,0.0f,0.0f),
		irr::core::vector3df(1.0f,1.0f,1.0f));
	node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	node->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, true);
	node->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
	node->setFrameLoop(0,100);

	mesh = SceneManager->getMesh("data/WargXJ85.3DS");
	irr::scene::IMeshSceneNode* node2 = SceneManager->addMeshSceneNode(mesh,0,-1,
		irr::core::vector3df(0.0f,0.0f,0.0f),
		irr::core::vector3df(0.0f,0.0f,0.0f),
		irr::core::vector3df(1.0f,1.0f,1.0f));
	node2->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	node2->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
	node2->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);

	SceneManager->addCameraSceneNode(0,
		irr::core::vector3df(0.0f,0.0f,0.0f),
		irr::core::vector3df(0.0f,0.0f,10.0f));
}

void Nightork::CMenu::CreateMainMenu()
{
	const irr::core::dimension2du screenSize = VideoDriver->getScreenSize();

	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-100, screenSize.Height/2-170, screenSize.Width/2+100, screenSize.Height/2-140),
		0, EGuiId::MAINMENU_BUTTON_CHANGE_PILOT, L"Change Pilot");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-100, screenSize.Height/2-110, screenSize.Width/2+100, screenSize.Height/2-80),
		0, EGuiId::MAINMENU_BUTTON_PLAY_LEVEL, L"Play Arena");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-100, screenSize.Height/2-50, screenSize.Width/2+100, screenSize.Height/2-20),
		0, EGuiId::MAINMENU_BUTTON_EXPLANATIONS, L"How To Play");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-100, screenSize.Height/2+10, screenSize.Width/2+100, screenSize.Height/2+40),
		0, EGuiId::MAINMENU_BUTTON_HIGHSCORES, L"Highscores");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-100, screenSize.Height/2+70, screenSize.Width/2+100, screenSize.Height/2+100),
		0, EGuiId::MAINMENU_BUTTON_SETTINGS, L"Settings");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-100, screenSize.Height/2+130, screenSize.Width/2+100, screenSize.Height/2+160),
		0, EGuiId::MAINMENU_BUTTON_CREDITS, L"Credits");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-100, screenSize.Height/2+190, screenSize.Width/2+100, screenSize.Height/2+220),
		0, EGuiId::MAINMENU_BUTTON_EXIT, L"Exit");

	const irr::core::stringw pilottext = Pilot->GetCurrentPilotName().empty()
		? irr::core::stringw("No valid pilot selected.")
		: irr::core::stringw("Pilotname: ") + Pilot->GetCurrentPilotName();
	irr::gui::IGUIStaticText* text = GuiEnvironment->addStaticText(pilottext.c_str(),
		irr::core::recti(screenSize.Width/2-280,screenSize.Height/2+250,screenSize.Width/2+280,screenSize.Height/2+280),
		false, false, 0, EGuiId::MAINMENU_TEXT_CURRENT_PILOT, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	if (Pilot->GetCurrentPilotName().empty())
	{
		text = GuiEnvironment->addStaticText(L"Select pilot to save settings permanently.",
			irr::core::recti(screenSize.Width / 2 - 280, screenSize.Height / 2 + 280, screenSize.Width / 2 + 280, screenSize.Height / 2 + 310),
			false, false, 0, EGuiId::MAINMENU_TEXT_CHANGE_PILOT, false);
		text->setTextAlignment(irr::gui::EGUIA_CENTER, irr::gui::EGUIA_CENTER);
		text->setOverrideColor(irr::video::SColor(255, 255, 255, 255));
		text->enableOverrideColor(true);
	}

	text = GuiEnvironment->addStaticText(L"Copyright (C) in 2011-2026 by Warptear Entertainment.",
		irr::core::recti(screenSize.Width/2-280,screenSize.Height-60,screenSize.Width/2+280,screenSize.Height-30),
		false, false, 0, EGuiId::MAINMENU_TEXT_COPYRIGHT, false);
		text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
		text->setOverrideColor(irr::video::SColor(255,255,255,255));
		text->enableOverrideColor(true);
}

void Nightork::CMenu::CreateIntroMenu()
{
	const irr::core::dimension2du screenSize = VideoDriver->getScreenSize();

	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width-250, screenSize.Height-60, screenSize.Width-50, screenSize.Height-30),
		0, EGuiId::INTROMENU_BUTTON_START, L"Start");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(50, screenSize.Height-60, 250, screenSize.Height-30),
		0, EGuiId::INTROMENU_BUTTON_EXIT, L"Back");

	irr::gui::IGUIStaticText* text = GuiEnvironment->addStaticText(L"",
		irr::core::recti(screenSize.Width/2-100, screenSize.Height-60, screenSize.Width/2+100, screenSize.Height-30),
		false, false, 0, EGuiId::INTROMENU_TEXT_LOADING, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	MemoryString = DunGenProxy->GetDescriptionIntro();

	irr::gui::CGUITextBox* textBox = new irr::gui::CGUITextBox(GuiEnvironment->getSkin()->getFont(), irr::video::SColor(255,255,255,255),
		false, MemoryString.c_str(), GuiEnvironment,
		irr::core::recti(screenSize.Width/2-250,screenSize.Height/2-200,screenSize.Width/2+250,screenSize.Height/2+250),
		GuiEnvironment->getRootGUIElement(), EGuiId::INTROMENU_TEXTBOX_DESCRIPTION);
	textBox->setScrollbarRight(true);
	textBox->drop();
}

void Nightork::CMenu::CreateExtroMenu(unsigned int deaths_, unsigned int timeNeeded_)
{
	const irr::core::dimension2du screenSize = VideoDriver->getScreenSize();

	GuiEnvironment->addButton(irr::core::rect<irr::s32>(50, screenSize.Height-60, 250, screenSize.Height-30),
		0, EGuiId::EXTROMENU_BUTTON_EXIT, L"Back");

	irr::gui::IGUIStaticText* text = GuiEnvironment->addStaticText(
		(irr::core::stringw("Last Arena - Deaths: ") + irr::core::stringw(deaths_) + irr::core::stringw(" , Time: ") + GetTime(timeNeeded_,true)).c_str(),
		irr::core::recti(screenSize.Width/2-300, screenSize.Height/2+260, screenSize.Width/2+300, screenSize.Height/2+290),
		false, false, 0, EGuiId::EXTROMENU_TEXT_STATS, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	MemoryString = DunGenProxy->GetDescriptionExtro();

	irr::gui::CGUITextBox* textBox = new irr::gui::CGUITextBox(GuiEnvironment->getSkin()->getFont(), irr::video::SColor(255,255,255,255),
		false, MemoryString.c_str(), GuiEnvironment,
		irr::core::recti(screenSize.Width/2-250,screenSize.Height/2-200,screenSize.Width/2+250,screenSize.Height/2+250),
		GuiEnvironment->getRootGUIElement(), EGuiId::EXTROMENU_TEXTBOX_DESCRIPTION);
	textBox->setScrollbarRight(true);
	textBox->drop();
}

void Nightork::CMenu::CreatePilotMenu(const irr::core::stringw& status_)
{
	const irr::core::dimension2du screenSize = VideoDriver->getScreenSize();

	irr::gui::IGUIListBox* pilotList = GuiEnvironment->addListBox(irr::core::recti(screenSize.Width/2-415,screenSize.Height/2-50,screenSize.Width/2-165,screenSize.Height/2+300),
		0,EGuiId::PILOTMENU_LISTBOX_PILOTS,true);

	const irr::io::path dir = FileSystem->getWorkingDirectory();
	FileSystem->changeWorkingDirectoryTo("pilots");
	irr::io::IFileList* fileList = FileSystem->createFileList();
	FileSystem->changeWorkingDirectoryTo(dir);
	AddFileListToListBox(pilotList,fileList);

	GuiEnvironment->addEditBox(L"",irr::core::recti(screenSize.Width/2-125,screenSize.Height/2-50,screenSize.Width/2+125,screenSize.Height/2-20),
		true,0,EGuiId::PILOTMENU_EDITBOX_PILOTNAME);

	irr::gui::IGUIComboBox* comboBox = GuiEnvironment->addComboBox(irr::core::recti(screenSize.Width/2+165,screenSize.Height/2-50,screenSize.Width/2+365,screenSize.Height/2-20),
		0,EGuiId::PILOTMENU_COMBOBOX_TOTEM);
	comboBox->addItem(L"Ice Bear");
	comboBox->addItem(L"Snow Tiger");
	comboBox->addItem(L"Frost Raven");
	comboBox->setSelected(Pilot->GetTotemType());

	irr::gui::CGUITextBox* textBox = new irr::gui::CGUITextBox(GuiEnvironment->getSkin()->getFont(), irr::video::SColor(255,255,255,255),
		false, L"", GuiEnvironment,
		irr::core::recti(screenSize.Width/2+165,screenSize.Height/2-0,screenSize.Width/2+415,screenSize.Height/2+200),
		GuiEnvironment->getRootGUIElement(), EGuiId::PILOTMENU_TEXTBOX_TOTEM_DESCRIPTION);
	textBox->setScrollbarRight(true);
	textBox->drop();
	SelectTotem();

	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-100, screenSize.Height/2, screenSize.Width/2+100, screenSize.Height/2+30),
		0, EGuiId::PILOTMENU_BUTTON_NEW_PILOT, L"New Pilot");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-100, screenSize.Height/2+50, screenSize.Width/2+100, screenSize.Height/2+80),
		0, EGuiId::PILOTMENU_BUTTON_LOAD_PILOT, L"Load Pilot");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-100, screenSize.Height/2+100, screenSize.Width/2+100, screenSize.Height/2+130),
		0, EGuiId::PILOTMENU_BUTTON_DELETE_PILOT, L"Delete Pilot");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-100, screenSize.Height/2+150, screenSize.Width/2+100, screenSize.Height/2+180),
		0, EGuiId::PILOTMENU_BUTTON_EXIT, L"Back");

	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2+165, screenSize.Height/2+220, screenSize.Width/2+365, screenSize.Height/2+250),
		0, EGuiId::PILOTMENU_BUTTON_SAVE_TOTEM, L"Save Totem");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2+165, screenSize.Height/2+270, screenSize.Width/2+365, screenSize.Height/2+300),
		0, EGuiId::PILOTMENU_BUTTON_RESET_TOTEM, L"Reset Totem");

	const irr::core::stringw pilottext = Pilot->GetCurrentPilotName().empty()
		? irr::core::stringw("No valid pilot selected.")
		: irr::core::stringw("Pilotname: ") + Pilot->GetCurrentPilotName();
	irr::gui::IGUIStaticText* text = GuiEnvironment->addStaticText(pilottext.c_str(),
		irr::core::recti(screenSize.Width/2-280,screenSize.Height/2-180,screenSize.Width/2+280,screenSize.Height/2-150),
		false, false, 0, EGuiId::PILOTMENU_TEXT_CURRENT_PILOT, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	text = GuiEnvironment->addStaticText((irr::core::stringw("Level: ") + irr::core::stringw("0")).c_str(),
		irr::core::recti(screenSize.Width/2-280,screenSize.Height/2-150,screenSize.Width/2+280,screenSize.Height/2-120),
		false, false, 0, EGuiId::PILOTMENU_TEXT_CURRENT_PILOT, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	text = GuiEnvironment->addStaticText((irr::core::stringw("Deaths: ") + irr::core::stringw("0")).c_str(),
		irr::core::recti(screenSize.Width/2-280,screenSize.Height/2-120,screenSize.Width/2+280,screenSize.Height/2-90),
		false, false, 0, EGuiId::PILOTMENU_TEXT_CURRENT_PILOT, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	text = GuiEnvironment->addStaticText((irr::core::stringw("Time needed: ") + GetTime(0, true)).c_str(),
		irr::core::recti(screenSize.Width/2-280,screenSize.Height/2-90,screenSize.Width/2+280,screenSize.Height/2-60),
		false, false, 0, EGuiId::PILOTMENU_TEXT_CURRENT_PILOT, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	text = GuiEnvironment->addStaticText(L"Available Pilots:",
		irr::core::recti(screenSize.Width/2-415,screenSize.Height/2-90,screenSize.Width/2-165,screenSize.Height/2-60),
		false, false, 0, EGuiId::PILOTMENU_TEXT_CURRENT_PILOT, false);
	text->setTextAlignment(irr::gui::EGUIA_UPPERLEFT,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	text = GuiEnvironment->addStaticText(L"Choose Totem:",
		irr::core::recti(screenSize.Width/2+165,screenSize.Height/2-90,screenSize.Width/2+415,screenSize.Height/2-60),
		false, false, 0, EGuiId::PILOTMENU_TEXT_TOTEMS, false);
	text->setTextAlignment(irr::gui::EGUIA_UPPERLEFT,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	text = GuiEnvironment->addStaticText(status_.c_str(),
		irr::core::recti(screenSize.Width/2-280,screenSize.Height/2+320,screenSize.Width/2+280,screenSize.Height/2+350),
		false, false, 0, EGuiId::PILOTMENU_TEXT_FILE_STATUS, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);
}

void Nightork::CMenu::CreatePlayLevelMenu()
{
	const irr::core::dimension2du screenSize = VideoDriver->getScreenSize();
	
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width / 2 - 100, screenSize.Height / 2 - 70, screenSize.Width / 2 + 100, screenSize.Height / 2 - 40),
		0, EGuiId::LEVELMENU_BUTTON_SPHERE, L"The Sphere");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width / 2 - 100, screenSize.Height / 2, screenSize.Width / 2 + 100, screenSize.Height / 2 + 30),
		0, EGuiId::LEVELMENU_BUTTON_HIVE, L"The Hive");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width / 2 - 100, screenSize.Height / 2 + 70, screenSize.Width / 2 + 100, screenSize.Height / 2 + 100),
		0, EGuiId::LEVELMENU_BUTTON_CAVE, L"The Cave");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(50, screenSize.Height-60, 250, screenSize.Height-30),
		0, EGuiId::LEVELMENU_BUTTON_EXIT, L"Back");

	irr::gui::IGUIStaticText* text = GuiEnvironment->addStaticText(L"Choose arena:",
		irr::core::recti(screenSize.Width/2-280,screenSize.Height/2-150,screenSize.Width/2+280,screenSize.Height/2-120),
		false, false, 0, 0, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);
}

void Nightork::CMenu::CreateSettingsMenu()
{
	const irr::core::dimension2du screenSize = VideoDriver->getScreenSize();

	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width-250, screenSize.Height-60, screenSize.Width-50, screenSize.Height-30),
		0, EGuiId::SETTINGSMENU_BUTTON_SAVE, L"Save");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(50, screenSize.Height-60, 250, screenSize.Height-30),
		0, EGuiId::SETTINGSMENU_BUTTON_EXIT, L"Back");
	GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-100, screenSize.Height-60, screenSize.Width/2+100, screenSize.Height-30),
		0, EGuiId::SETTINGSMENU_BUTTON_DEFAULTS, L"Defaults");

	irr::gui::IGUIStaticText* text = GuiEnvironment->addStaticText(L"Music volume:",
		irr::core::recti(screenSize.Width/2-300,screenSize.Height/2-230,screenSize.Width/2+300,screenSize.Height/2-200),
		false, false, 0, EGuiId::SETTINGSMENU_TEXT_SOUND_MUSIC, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	irr::gui::IGUIScrollBar* scrollBar = GuiEnvironment->addScrollBar(true,
		irr::core::recti(screenSize.Width/2-300,screenSize.Height/2-190,screenSize.Width/2+300,screenSize.Height/2-170),
		0, EGuiId::SETTINGSMENU_SCROLLBAR_SOUND_MUSIC);
	scrollBar->setMax(100);
	scrollBar->setPos(static_cast<int>(Pilot->GetSoundLevelMusic()*100));

	text = GuiEnvironment->addStaticText(L"Sound effects volume:",
		irr::core::recti(screenSize.Width/2-300,screenSize.Height/2-150,screenSize.Width/2+300,screenSize.Height/2-120),
		false, false, 0, EGuiId::SETTINGSMENU_TEXT_SOUND_EFFECTS, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	scrollBar = GuiEnvironment->addScrollBar(true,
		irr::core::recti(screenSize.Width/2-300,screenSize.Height/2-110,screenSize.Width/2+300,screenSize.Height/2-90),
		0, EGuiId::SETTINGSMENU_SCROLLBAR_SOUND_EFFECTS);
	scrollBar->setMax(100);
	scrollBar->setPos(static_cast<int>(Pilot->GetSoundLevelEffects()*100));

	text = GuiEnvironment->addStaticText(L"Mouse sensitivity:",
		irr::core::recti(screenSize.Width/2-300,screenSize.Height/2-70,screenSize.Width/2+300,screenSize.Height/2-40),
		false, false, 0, EGuiId::SETTINGSMENU_TEXT_MOUSE_SENSITIVITY, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	scrollBar = GuiEnvironment->addScrollBar(true,
		irr::core::recti(screenSize.Width/2-300,screenSize.Height/2-30,screenSize.Width/2+300,screenSize.Height/2-10),
		0, EGuiId::SETTINGSMENU_SCROLLBAR_MOUSE_SENSITIVITY);
	scrollBar->setMax(300);
	scrollBar->setMin(10);
	scrollBar->setPos(static_cast<int>(Pilot->GetMouseSensitivity()*100));

	text = GuiEnvironment->addStaticText(L"Mouse invert y-axis",
		irr::core::recti(screenSize.Width/2-300,screenSize.Height/2+20,screenSize.Width/2+300,screenSize.Height/2+50),
		false, false, 0, EGuiId::SETTINGSMENU_TEXT_MOUSE_INVERT_Y_AXIS, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	irr::gui::IGUICheckBox* checkBox = GuiEnvironment->addCheckBox(Pilot->GetMouseInvertY(),
		irr::core::recti(screenSize.Width/2-300,screenSize.Height/2+20,screenSize.Width/2+300,screenSize.Height/2+50),
		0, EGuiId::SETTINGSMENU_CHECKBOX_MOUSE_INVERT_Y_AXIS, L"");

	// control buttons
	text = GuiEnvironment->addStaticText(L"Forward move button",
		irr::core::recti(screenSize.Width/2-410,screenSize.Height/2+100,screenSize.Width/2-240,screenSize.Height/2+120),
		false, false, 0, EGuiId::SETTINGSMENU_TEXT_CONTROL_FORWARD, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);
	irr::gui::IGUIButton* button = GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-380, screenSize.Height/2+120, screenSize.Width/2-270, screenSize.Height/2+140),
		0, EGuiId::SETTINGSMENU_BUTTON_CONTROL_FORWARD, KeyNames[Pilot->GetControl(CPilot::SControls::Forward)].c_str());
	std::string control = std::to_string(static_cast<long long>(Pilot->GetControl(CPilot::SControls::Forward)));
	button->setName(control.c_str());

	text = GuiEnvironment->addStaticText(L"Backward move button",
		irr::core::recti(screenSize.Width/2-410,screenSize.Height/2+160,screenSize.Width/2-240,screenSize.Height/2+180),
		false, false, 0, EGuiId::SETTINGSMENU_TEXT_CONTROL_BACKWARD, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);
	button = GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-380, screenSize.Height/2+180, screenSize.Width/2-270, screenSize.Height/2+200),
		0, EGuiId::SETTINGSMENU_BUTTON_CONTROL_BACKWARD, KeyNames[Pilot->GetControl(CPilot::SControls::Backward)].c_str());
	control = std::to_string(static_cast<long long>(Pilot->GetControl(CPilot::SControls::Backward)));
	button->setName(control.c_str());

	text = GuiEnvironment->addStaticText(L"Left roll button",
		irr::core::recti(screenSize.Width/2-200,screenSize.Height/2+100,screenSize.Width/2-50,screenSize.Height/2+120),
		false, false, 0, EGuiId::SETTINGSMENU_TEXT_CONTROL_LEFT, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);
	button = GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-180, screenSize.Height/2+120, screenSize.Width/2-70, screenSize.Height/2+140),
		0, EGuiId::SETTINGSMENU_BUTTON_CONTROL_LEFT, KeyNames[Pilot->GetControl(CPilot::SControls::Left)].c_str());
	control = std::to_string(static_cast<long long>(Pilot->GetControl(CPilot::SControls::Left)));
	button->setName(control.c_str());

	text = GuiEnvironment->addStaticText(L"Right roll button",
		irr::core::recti(screenSize.Width/2-200,screenSize.Height/2+160,screenSize.Width/2-50,screenSize.Height/2+180),
		false, false, 0, EGuiId::SETTINGSMENU_TEXT_CONTROL_RIGHT, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);
	button = GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2-180, screenSize.Height/2+180, screenSize.Width/2-70, screenSize.Height/2+200),
		0, EGuiId::SETTINGSMENU_BUTTON_CONTROL_RIGHT, KeyNames[Pilot->GetControl(CPilot::SControls::Right)].c_str());
	control = std::to_string(static_cast<long long>(Pilot->GetControl(CPilot::SControls::Right)));
	button->setName(control.c_str());

	text = GuiEnvironment->addStaticText(L"Weapon Magic button",
		irr::core::recti(screenSize.Width/2+30,screenSize.Height/2+100,screenSize.Width/2+220,screenSize.Height/2+120),
		false, false, 0, EGuiId::SETTINGSMENU_TEXT_CONTROL_WEAPON_1, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);
	button = GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2+70, screenSize.Height/2+120, screenSize.Width/2+180, screenSize.Height/2+140),
		0, EGuiId::SETTINGSMENU_BUTTON_CONTROL_WEAPON_1, KeyNames[Pilot->GetControl(CPilot::SControls::Weapon1)].c_str());
	control = std::to_string(static_cast<long long>(Pilot->GetControl(CPilot::SControls::Weapon1)));
	button->setName(control.c_str());

	text = GuiEnvironment->addStaticText(L"Weapon Antimagic button",
		irr::core::recti(screenSize.Width/2+30,screenSize.Height/2+160,screenSize.Width/2+220,screenSize.Height/2+180),
		false, false, 0, EGuiId::SETTINGSMENU_TEXT_CONTROL_WEAPON_2, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);
	button = GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2+70, screenSize.Height/2+180, screenSize.Width/2+180, screenSize.Height/2+200),
		0, EGuiId::SETTINGSMENU_BUTTON_CONTROL_WEAPON_2, KeyNames[Pilot->GetControl(CPilot::SControls::Weapon2)].c_str());
	control = std::to_string(static_cast<long long>(Pilot->GetControl(CPilot::SControls::Weapon2)));
	button->setName(control.c_str());

	text = GuiEnvironment->addStaticText(L"Afterburner button",
		irr::core::recti(screenSize.Width/2+250,screenSize.Height/2+100,screenSize.Width/2+400,screenSize.Height/2+120),
		false, false, 0, EGuiId::SETTINGSMENU_TEXT_CONTROL_AFTERBURNER, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);
	button = GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2+270, screenSize.Height/2+120, screenSize.Width/2+380, screenSize.Height/2+140),
		0, EGuiId::SETTINGSMENU_BUTTON_CONTROL_AFTERBURNER, KeyNames[Pilot->GetControl(CPilot::SControls::Afterburner)].c_str());
	control = std::to_string(static_cast<long long>(Pilot->GetControl(CPilot::SControls::Afterburner)));
	button->setName(control.c_str());

	text = GuiEnvironment->addStaticText(L"Enemy highlight button",
		irr::core::recti(screenSize.Width/2+230,screenSize.Height/2+160,screenSize.Width/2+410,screenSize.Height/2+180),
		false, false, 0, EGuiId::SETTINGSMENU_TEXT_CONTROL_HIGHLIGHTING, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER, irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255, 255, 255, 255));
	text->enableOverrideColor(true);
	button = GuiEnvironment->addButton(irr::core::rect<irr::s32>(screenSize.Width/2+270, screenSize.Height/2+180, screenSize.Width/2+380, screenSize.Height/2+ 200),
		0, EGuiId::SETTINGSMENU_BUTTON_CONTROL_HIGHLIGHTING, KeyNames[Pilot->GetControl(CPilot::SControls::Highlighting)].c_str());
	control = std::to_string(static_cast<long long>(Pilot->GetControl(CPilot::SControls::Highlighting)));
	button->setName(control.c_str());
}

void Nightork::CMenu::CreateHighScoreMenu()
{
	const irr::core::dimension2du screenSize = VideoDriver->getScreenSize();

	GuiEnvironment->addButton(irr::core::rect<irr::s32>(50, screenSize.Height-60, 250, screenSize.Height-30),
		0, EGuiId::HIGHSCOREMENU_BUTTON_EXIT, L"Back");

	irr::gui::IGUIStaticText* text = GuiEnvironment->addStaticText(L"Highscores:",
		irr::core::recti(screenSize.Width/2-350,screenSize.Height/2-200,screenSize.Width/2+350,screenSize.Height/2-170),
		false, false, 0, 0, false);
	text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	text->setOverrideColor(irr::video::SColor(255,255,255,255));
	text->enableOverrideColor(true);

	for (unsigned int a = 0; a < Constants::Level::EArena::Number; ++a)
		for (unsigned int i=0; i< CHighScores::HIGHSCORE_NUMBER; ++i)
		{
			Constants::Level::EArena::Enum arena = static_cast<Constants::Level::EArena::Enum>(a);
			text = GuiEnvironment->addStaticText(arena == Constants::Level::EArena::Sphere ? L"The Sphere" : arena == Constants::Level::EArena::Hive ? L"The Hive" : L"The Cave",
				irr::core::recti(screenSize.Width / 2 - 350, screenSize.Height / 2 - 160 + a * 150, screenSize.Width / 2 + 350, screenSize.Height / 2 - 130 + a * 150),
				false, false, 0, 0, false);
			text->setTextAlignment(irr::gui::EGUIA_CENTER, irr::gui::EGUIA_CENTER);
			text->setOverrideColor(irr::video::SColor(255, 255, 255, 255));
			text->enableOverrideColor(true);

			CHighScores::SHighScore score = HighScores->GetHighScore(arena, i);
					irr::core::stringw str = irr::core::stringw(score.PilotName.c_str())
				+ irr::core::stringw(" , Deaths: ") + irr::core::stringw(score.Deaths)
				+ irr::core::stringw(" , Time: ") + GetTime(score.TimeNeeded,true);

			text = GuiEnvironment->addStaticText(str.c_str(),
				irr::core::recti(screenSize.Width/2-350,screenSize.Height/2-130+a*150+i*30,screenSize.Width/2+350,screenSize.Height/2-100+a*150+i*30),
				false, false, 0, 0, false);
			text->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
			text->setOverrideColor(irr::video::SColor(255,255,255,255));
			text->enableOverrideColor(true);
	}
}

void Nightork::CMenu::CreateCreditsMenu()
{
	const irr::core::dimension2du screenSize = VideoDriver->getScreenSize();

	GuiEnvironment->addButton(irr::core::rect<irr::s32>(50, screenSize.Height-60, 250, screenSize.Height-30),
		0, EGuiId::CREDITSMENU_BUTTON_EXIT, L"Back");

	irr::gui::CGUITextBox* textBox = new irr::gui::CGUITextBox(GuiEnvironment->getSkin()->getFont(), irr::video::SColor(255,255,255,255),
		true, Credits->GetCredits().c_str(), GuiEnvironment,
		irr::core::recti(screenSize.Width/2-250,screenSize.Height/2-200,screenSize.Width/2+250,screenSize.Height/2+250),
		GuiEnvironment->getRootGUIElement(), EGuiId::CREDITSMENU_TEXTBOX_CREDITS);
	textBox->setScrollbarRight(true);
	textBox->drop();
}

void Nightork::CMenu::CreateExplanationMenu()
{
	const irr::core::dimension2du screenSize = VideoDriver->getScreenSize();

	GuiEnvironment->addButton(irr::core::rect<irr::s32>(50, screenSize.Height - 60, 250, screenSize.Height - 30),
		0, EGuiId::EXPLANATIONMENU_BUTTON_EXIT, L"Back");
}

void Nightork::CMenu::PressButton(CPilot::SControls::Enum button_)
{
	irr::gui::IGUIButton* button = static_cast<irr::gui::IGUIButton*>(GuiEnvironment->getRootGUIElement()->getElementFromId(GetIdFromControl(button_), true));
	button->setText(L"Press Button");
}

void Nightork::CMenu::ButtonPressed(CPilot::SControls::Enum button_, unsigned int val_)
{
	irr::gui::IGUIButton* button = static_cast<irr::gui::IGUIButton*>(GuiEnvironment->getRootGUIElement()->getElementFromId(GetIdFromControl(button_), true));
	std::string control = std::to_string(static_cast<long long>(val_));
	button->setName(control.c_str());
	button->setText(KeyNames[val_].c_str());
}

void Nightork::CMenu::SelectTotem()
{
	irr::gui::IGUIComboBox* comboBox = static_cast<irr::gui::IGUIComboBox*>(GuiEnvironment->getRootGUIElement()->getElementFromId(CMenu::EGuiId::PILOTMENU_COMBOBOX_TOTEM, true));
	irr::gui::CGUITextBox* textBox = static_cast<irr::gui::CGUITextBox*>(GuiEnvironment->getRootGUIElement()->getElementFromId(CMenu::EGuiId::PILOTMENU_TEXTBOX_TOTEM_DESCRIPTION, true));
	textBox->setText(TotemDesriptions[comboBox->getSelected()].c_str());
}

void Nightork::CMenu::AddFileListToListBox(irr::gui::IGUIListBox* listBox_, irr::io::IFileList* fileList_)
{
	if (fileList_)
	{
		for (unsigned int i = 2; i < fileList_->getFileCount(); ++i)	// The first two entries are . and .. and are ignored.
		{
			irr::io::path filename = fileList_->getFileName(i);
			irr::io::path pilotname = Pilot->LoadPilotName(filename);
			const irr::c8* chars = pilotname.c_str();

			wchar_t* wchars = new wchar_t[strlen(chars) + 1];
			const int len = static_cast<int>(mbstowcs(wchars, chars, strlen(chars)));

			wchars[len] = 0;
			irr::core::stringw stringvalue = wchars;
			delete[] wchars;

			listBox_->addItem(stringvalue.c_str());
		}
	}
}
