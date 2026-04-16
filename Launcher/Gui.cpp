// Copyright (C) 2011-2014 by Maximilian Hönig

#include "GUI.h"

// Constructor
Launcher::CGUI::CGUI(irr::gui::IGUIEnvironment* guiEnvironment_, irr::video::IVideoDriver* videoDriver_)
: GUIEnvironment(guiEnvironment_)
{
	TextureLogo = videoDriver_->getTexture("data/Nightork-Logo.png");
	TextureBackground = videoDriver_->getTexture("data/Launcher.png");
	GUIEnvironment->getSkin()->setFont(GUIEnvironment->getFont("data/HudFont.xml"));

	// change gui transparencies
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_3D_DARK_SHADOW, irr::video::SColor(255,0,0,0));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_3D_FACE, irr::video::SColor(255,200,120,10));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_3D_HIGH_LIGHT, irr::video::SColor(255,150,120,10));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_3D_LIGHT, irr::video::SColor(255,200,120,10));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_3D_SHADOW, irr::video::SColor(255,200,120,10));

	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_ACTIVE_BORDER, irr::video::SColor(255,0,0,255));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_ACTIVE_CAPTION, irr::video::SColor(255,180,120,10));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_APP_WORKSPACE, irr::video::SColor(255,0,0,255));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_COUNT, irr::video::SColor(255,0,0,0));

	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_GRAY_TEXT, irr::video::SColor(255,0,0,255));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_HIGH_LIGHT, irr::video::SColor(255,170,200,10));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_HIGH_LIGHT_TEXT, irr::video::SColor(255,0,0,0));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_ICON, irr::video::SColor(255,0,0,255));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_ICON_HIGH_LIGHT, irr::video::SColor(255,0,0,255));
													
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_INACTIVE_BORDER, irr::video::SColor(255,0,0,255));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_INACTIVE_CAPTION, irr::video::SColor(255,0,0,255));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_SCROLLBAR, irr::video::SColor(255,150,120,10));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_TOOLTIP, irr::video::SColor(255,0,0,255));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_TOOLTIP_BACKGROUND, irr::video::SColor(255,0,0,255));

	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_WINDOW, irr::video::SColor(255,0,0,255));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_WINDOW_SYMBOL, irr::video::SColor(255,0,0,0));

	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_BUTTON_TEXT, irr::video::SColor(255,255,255,255));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_EDITABLE, irr::video::SColor(255,150,120,10));
	GUIEnvironment->getSkin()->setColor(irr::gui::EGDC_FOCUSED_EDITABLE, irr::video::SColor(255,150,120,10));
}

// create main menu
void Launcher::CGUI::CreateMenue()
{
	irr::gui::IGUIImage* background = GUIEnvironment->addImage(irr::core::rect<irr::s32>(0,0,1023,767), 0);
	background->setImage(TextureBackground);
	irr::gui::IGUIImage* logo = GUIEnvironment->addImage(irr::core::rect<irr::s32>(112,0,1023,200), 0);
	logo->setImage(TextureLogo);

	const int xOffset = 387;
	const int yOffset = 200;

	GUIEnvironment->addStaticText(L"Game config:", irr::core::rect<irr::s32>(xOffset+10,yOffset-15,999,999), false, false, 0);

	GUIEnvironment->addStaticText(L"Resolution width:", irr::core::rect<irr::s32>(xOffset+10,yOffset+35,999,999), false, false, 0);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(xOffset+200,yOffset+30,xOffset+260,yOffset+60), true, 0, EGuiId::EDITBOX_NIGHTORK_RESOLUTION_X);
	GUIEnvironment->addStaticText(L"Resolution height:", irr::core::rect<irr::s32>(xOffset+10,yOffset+65,999,999), false, false, 0);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(xOffset+200,yOffset+60,xOffset+260,yOffset+90), true, 0, EGuiId::EDITBOX_NIGHTORK_RESOLUTION_Y);

	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(xOffset+10,yOffset+120,xOffset+260,yOffset+150),0, EGuiId::CHECKBOX_NIGHTORK_FULLSCREEN,L"Fullscreen");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(xOffset+10,yOffset+160,xOffset+260,yOffset+190),0, EGuiId::CHECKBOX_NIGHTORK_VSYNC,L"VSync");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(xOffset+10,yOffset+200,xOffset+260,yOffset+230),0, EGuiId::CHECKBOX_NIGHTORK_ANTIALIASING, L"Anti-Aliasing");

	irr::gui::IGUIComboBox* comboBox = GUIEnvironment->addComboBox(irr::core::rect<irr::s32>(xOffset+10,yOffset+265,xOffset+260,yOffset+295),0,EGuiId::COMBOBOX_NIGHTORK_RENDERER);
	comboBox->addItem(L"Preferred Renderer: DirectX 9",0);
	comboBox->addItem(L"Preferred Renderer: OpenGL",0);

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(xOffset+10,yOffset+360,xOffset+260,yOffset+390), 0, EGuiId::BUTTON_DEFAULTS, L"Defaults");

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(xOffset+10,yOffset+420,xOffset+260,yOffset+450), 0, EGuiId::BUTTON_START_NIGHTORK, L"Start Game");
}
