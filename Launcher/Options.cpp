// Copyright (C) 2011-2014 by Maximilian Hönig

#include "Options.h"
#include "Gui.h"
#include <iostream>
#include <time.h>
#include <fstream>

Launcher::COptions::COptions(irr::IrrlichtDevice* irrDevice_)
	: IrrDevice(irrDevice_)
	, GUIEnvironment(irrDevice_->getGUIEnvironment())
{
}

Launcher::COptions::~COptions()
{
}

void Launcher::COptions::Save()
{
	const unsigned int ANTIALIASING_LEVEL = 4;

	unsigned int resolution_x_nightork = 1600;
	unsigned int resolution_y_nightork = 900;
	unsigned int antialiasing_nightork = 0;
	bool fullscreen_nightork = false;
	bool vsync_nightork = false;
	bool directX_nightork = true;

	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	irr::gui::IGUICheckBox* checkBox;
	irr::gui::IGUIEditBox* editBox;
	irr::gui::IGUIComboBox* comboBox;
	irr::core::stringc tempString;

	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_NIGHTORK_FULLSCREEN, true);
	fullscreen_nightork = checkBox->isChecked();
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_NIGHTORK_VSYNC, true);
	vsync_nightork = checkBox->isChecked();
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_NIGHTORK_ANTIALIASING, true);
	antialiasing_nightork = checkBox->isChecked() ? ANTIALIASING_LEVEL : 0;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_NIGHTORK_RESOLUTION_X, true);
	tempString = editBox->getText();
	resolution_x_nightork = atoi(tempString.c_str());
	resolution_x_nightork = std::max(1600u, resolution_x_nightork);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_NIGHTORK_RESOLUTION_Y, true);
	tempString = editBox->getText();
	resolution_y_nightork = atoi(tempString.c_str());
	resolution_y_nightork = std::max(900u, resolution_y_nightork);

	comboBox = (irr::gui::IGUIComboBox*) root->getElementFromId(CGUI::EGuiId::COMBOBOX_NIGHTORK_RENDERER, true);
	directX_nightork = (0 == comboBox->getSelected());

	std::ofstream outfile("Nightork.cfg", std::ofstream::out & std::ofstream::binary);
	if (outfile)
	{
		outfile << resolution_x_nightork << std::endl;
		outfile << resolution_y_nightork << std::endl;
		outfile << fullscreen_nightork << std::endl;
		outfile << vsync_nightork << std::endl;
		outfile << antialiasing_nightork << std::endl;
		outfile << directX_nightork << std::endl;
		outfile.close();
	}

	std::cout << "Config saved." << std::endl;
}

void Launcher::COptions::Load()
{
	unsigned int resolution_x_nightork = 1600;
	unsigned int resolution_y_nightork = 900;
	unsigned int antialiasing_nightork = 0;
	bool fullscreen_nightork = false;
	bool vsync_nightork = false;
	bool directX_nightork = true;

	std::ifstream infile("Nightork.cfg", std::ifstream::in & std::ifstream::binary);
	if (infile)
	{
		infile >> resolution_x_nightork;
		infile >> resolution_y_nightork;
		infile >> fullscreen_nightork;
		infile >> vsync_nightork;
		infile >> antialiasing_nightork;
		infile >> directX_nightork;
		infile.close();
	}

	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	irr::gui::IGUICheckBox* checkBox;
	irr::gui::IGUIEditBox* editBox;
	irr::gui::IGUIComboBox* comboBox;

	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_NIGHTORK_FULLSCREEN, true);
	checkBox->setChecked(fullscreen_nightork);
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_NIGHTORK_VSYNC, true);
	checkBox->setChecked(vsync_nightork);
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_NIGHTORK_ANTIALIASING, true);
	checkBox->setChecked(antialiasing_nightork != 0);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_NIGHTORK_RESOLUTION_X, true);
	editBox->setText(irr::core::stringw(resolution_x_nightork).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_NIGHTORK_RESOLUTION_Y, true);
	editBox->setText(irr::core::stringw(resolution_y_nightork).c_str());

	comboBox = (irr::gui::IGUIComboBox*) root->getElementFromId(CGUI::EGuiId::COMBOBOX_NIGHTORK_RENDERER, true);
	comboBox->setSelected(directX_nightork ? 0 : 1);

	std::cout << "Config loaded." << std::endl;
}

void Launcher::COptions::Defaults()
{
	unsigned int resolution_x_nightork = 1600;
	unsigned int resolution_y_nightork = 900;
	unsigned int antialiasing_nightork = 0;
	bool fullscreen_nightork = false;
	bool vsync_nightork = false;
	bool directX_nightork = true;

	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	irr::gui::IGUICheckBox* checkBox;
	irr::gui::IGUIEditBox* editBox;
	irr::gui::IGUIComboBox* comboBox;

	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_NIGHTORK_FULLSCREEN, true);
	checkBox->setChecked(fullscreen_nightork);
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_NIGHTORK_VSYNC, true);
	checkBox->setChecked(vsync_nightork);
	checkBox = (irr::gui::IGUICheckBox*)root->getElementFromId(CGUI::EGuiId::CHECKBOX_NIGHTORK_ANTIALIASING, true);
	checkBox->setChecked(antialiasing_nightork != 0);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_NIGHTORK_RESOLUTION_X, true);
	editBox->setText(irr::core::stringw(resolution_x_nightork).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_NIGHTORK_RESOLUTION_Y, true);
	editBox->setText(irr::core::stringw(resolution_y_nightork).c_str());

	comboBox = (irr::gui::IGUIComboBox*) root->getElementFromId(CGUI::EGuiId::COMBOBOX_NIGHTORK_RENDERER, true);
	comboBox->setSelected(directX_nightork ? 0 : 1);

	std::cout << "Default config restored." << std::endl;
}
