// Copyright (C) 2011-2014 by Maximilian Hönig

#include "Options.h"
#include "Gui.h"
#include "LSystemRandom.h"
#include "ObjectPlacement.h"
#include <iostream>
#include <time.h>

void Editor::COptions::WindowsStore()
{
	WindowStore_RandomCave();
	WindowStore_Cave();
	WindowStore_ObjectPlacement();
	WindowStore_RandomObjects();
	WindowStore_Gameplay();
	WindowStore_Description();
}

void Editor::COptions::WindowsApply()
{
	WindowApply_RandomCave();
	WindowApply_Cave();
	WindowApply_ObjectPlacement();
	WindowApply_RandomObjects();
	WindowApply_Gameplay();
	WindowApply_Description();
}

void Editor::COptions::WindowStore_RandomCave()
{
	// get root element
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	// test if window active
	irr::gui::IGUIElement* window = root->getElementFromId(CGUI::EGuiId::WINDOW_RANDOMCAVE, true);
	if (!window)
		return;

	// read values
	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ITERATION_RANDOM, true);
	irr::core::stringc tmpString = editBox->getText();
	LSystemRandom->RandomCaveIteration = atoi(tmpString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEYAW_MIN, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveAngleYawMin = atof(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEYAW_MAX, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveAngleYawMax = atof(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEYAW_DIGITS, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveAngleYawDigits = atoi(tmpString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEPITCH_MIN, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveAnglePitchMin = atof(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEPITCH_MAX, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveAnglePitchMax = atof(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEPITCH_DIGITS, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveAnglePitchDigits = atoi(tmpString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEROLL_MIN, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveAngleRollMin = atof(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEROLL_MAX, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveAngleRollMax = atof(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEROLL_DIGITS, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveAngleRollDigits = atoi(tmpString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RADIUSSTART_MIN, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveRadiusMin = atof(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RADIUSSTART_MAX, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveRadiusMax = atof(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RADIUSSTART_DIGITS, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveRadiusDigits = atoi(tmpString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_STARTSTRING_ALLOWED_SYMBOLS, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveStartAllowedSymbols = tmpString.c_str();
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_STARTSTRING_MINLENGTH, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveStartMinSymbols = atoi(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_STARTSTRING_MAXLENGTH, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveStartMaxSymbols = atoi(tmpString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RULE_ALLOWED_SYMBOLS, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveRuleAllowedSymbols = tmpString.c_str();
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RULE_MINLENGTH, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveRuleMinSymbols = atoi(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RULE_MAXLENGTH, true);
	tmpString = editBox->getText();
	LSystemRandom->RandomCaveRuleMaxSymbols = atoi(tmpString.c_str());
}

void Editor::COptions::WindowStore_Cave()
{
	// get root element
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	// test if window active
	irr::gui::IGUIElement* window = root->getElementFromId(CGUI::EGuiId::WINDOW_CAVE, true);
	if (!window)
		return;

	// read values
	irr::video::SColorf color;
	color.a = 1.0f;
	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_COLOR_CAVE_R, true);
	irr::core::stringc tmpString = editBox->getText();
	color.r = static_cast<float>(atof(tmpString.c_str()));
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_COLOR_CAVE_G, true);
	tmpString = editBox->getText();
	color.g = static_cast<float>(atof(tmpString.c_str()));
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_COLOR_CAVE_B, true);
	tmpString = editBox->getText();
	color.b = static_cast<float>(atof(tmpString.c_str()));
	DungeonGenerator->MaterialSetCaveColor(color);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ITERATION, true);
	tmpString = editBox->getText();
	DungeonGenerator->LSystemSetIteration(atoi(tmpString.c_str()));

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEYAW_VALUE, true);
	tmpString = editBox->getText();
	DungeonGenerator->LSystemSetParameter(DunGen::ELSystemParameter::ANGLE_YAW, atof(tmpString.c_str()));
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEPITCH_VALUE, true);
	tmpString = editBox->getText();
	DungeonGenerator->LSystemSetParameter(DunGen::ELSystemParameter::ANGLE_PITCH, atof(tmpString.c_str()));
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEROLL_VALUE, true);
	tmpString = editBox->getText();
	DungeonGenerator->LSystemSetParameter(DunGen::ELSystemParameter::ANGLE_ROLL, atof(tmpString.c_str()));
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RADIUSSTART_VALUE, true);
	tmpString = editBox->getText();
	DungeonGenerator->LSystemSetParameter(DunGen::ELSystemParameter::RADIUS_START, atof(tmpString.c_str()));

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_STARTSTRING_VALUE, true);
	tmpString = editBox->getText();
	DungeonGenerator->LSystemSetStart(tmpString.c_str());

	irr::gui::IGUICheckBox* checkBox =  (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_COLOR_CAVE_MULTICOLOR, true);
	if (checkBox->isChecked())
		DungeonGenerator->MaterialSetCaveMaterial(DunGen::EMaterialType::CAVE_SINGLECOLOR,NULL);
	else
		DungeonGenerator->MaterialSetCaveMaterial(DunGen::EMaterialType::CAVE_MULTICOLOR,NULL);

	checkBox =  (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_EROSION, true);
	DungeonGenerator->VoxelCaveDeleteErosionSteps();
	if (checkBox->isChecked())
		DungeonGenerator->VoxelCaveAddErosionStep(0.5);
	checkBox =  (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_FILTERING, true);
	DungeonGenerator->VoxelCaveSetFiltering(checkBox->isChecked());
}

void Editor::COptions::WindowStore_ObjectPlacement()
{
	// no functionality needed here
}

void Editor::COptions::WindowStore_RandomObjects()
{
	// get root element
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	// test if window active
	irr::gui::IGUIElement* window = root->getElementFromId(CGUI::EGuiId::WINDOW_RANDOMOBJECTS, true);
	if (!window)
		return;

	// read values
	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_SHIP_RED, true);
	irr::core::stringc tmpString = editBox->getText();
	ObjectPlacement->ObjectFactors[DunGen::EObject::SHIP_RED] = atoi(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_SHIP_GREEN, true);
	tmpString = editBox->getText();
	ObjectPlacement->ObjectFactors[DunGen::EObject::SHIP_GREEN] = atoi(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_SHIP_BLUE, true);
	tmpString = editBox->getText();
	ObjectPlacement->ObjectFactors[DunGen::EObject::SHIP_BLUE] = atoi(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_SHIP_YELLOW, true);
	tmpString = editBox->getText();
	ObjectPlacement->ObjectFactors[DunGen::EObject::SHIP_YELLOW] = atoi(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_SHIP_TEAL, true);
	tmpString = editBox->getText();
	ObjectPlacement->ObjectFactors[DunGen::EObject::SHIP_TEAL] = atoi(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_SHIP_PURPLE, true);
	tmpString = editBox->getText();
	ObjectPlacement->ObjectFactors[DunGen::EObject::SHIP_PURPLE] = atoi(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_NO_SHIP, true);
	tmpString = editBox->getText();
	ObjectPlacement->ObjectFactors[DunGen::EObject::NO_SHIP] = atoi(tmpString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ADDITIONAL_DISTANCE_SHIPS, true);
	tmpString = editBox->getText();
	ObjectPlacement->AdditionalDistanceToShip = atoi(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ADDITIONAL_DISTANCE_BOSSES, true);
	tmpString = editBox->getText();
	ObjectPlacement->AdditionalDistanceToBoss = atoi(tmpString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ADDITIONAL_DISTANCE_START, true);
	tmpString = editBox->getText();
	ObjectPlacement->AdditionalDistanceToPortal = (atoi(tmpString.c_str()));
}

void Editor::COptions::WindowStore_Gameplay()
{
	// get root element
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	// test if window active
	irr::gui::IGUIElement* window = root->getElementFromId(CGUI::EGuiId::WINDOW_GAMEPLAY, true);
	if (!window)
		return;

	// read values
	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_MONSTER_HITPOINTS, true);
	irr::core::stringc tmpString = editBox->getText();
	DungeonGenerator->SetMonsterHitpoints(atoi(tmpString.c_str()));
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_BOSSMONSTER_HITPOINTS, true);
	tmpString = editBox->getText();
	DungeonGenerator->SetBossMonsterHitpoints(atoi(tmpString.c_str()));
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_MONSTER_FIRETIME, true);
	tmpString = editBox->getText();
	DungeonGenerator->SetMonsterFireTime(atoi(tmpString.c_str()));
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_BOSSMONSTER_FIRETIME, true);
	tmpString = editBox->getText();
	DungeonGenerator->SetBossMonsterFireTime(atoi(tmpString.c_str()));
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_MONSTER_DAMAGE, true);
	tmpString = editBox->getText();
	DungeonGenerator->SetMonsterDamage(atoi(tmpString.c_str()));
}

void Editor::COptions::WindowStore_Description()
{
	// get root element
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	// test if window active
	irr::gui::IGUIElement* window = root->getElementFromId(CGUI::EGuiId::WINDOW_DESCRIPTION, true);
	if (!window)
		return;

	// read values
	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_DESCRIPTION_INTRO, true);
	DungeonGenerator->SetDescriptionIntro(editBox->getText());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_DESCRIPTION_EXTRO, true);
	DungeonGenerator->SetDescriptionExtro(editBox->getText());
}

void Editor::COptions::WindowApply_RandomCave()
{
	// get root element
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	// test if window active
	irr::gui::IGUIElement* window = root->getElementFromId(CGUI::EGuiId::WINDOW_RANDOMCAVE, true);
	if (!window)
		return;

	// read values
	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ITERATION_RANDOM, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveIteration).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEYAW_MIN, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveAngleYawMin).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEYAW_MAX, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveAngleYawMax).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEYAW_DIGITS, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveAngleYawDigits).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEPITCH_MIN, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveAnglePitchMin).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEPITCH_MAX, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveAnglePitchMax).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEPITCH_DIGITS, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveAnglePitchDigits).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEROLL_MIN, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveAngleRollMin).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEROLL_MAX, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveAngleRollMax).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEROLL_DIGITS, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveAngleRollDigits).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RADIUSSTART_MIN, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveRadiusMin).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RADIUSSTART_MAX, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveRadiusMax).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RADIUSSTART_DIGITS, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveRadiusDigits).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_STARTSTRING_ALLOWED_SYMBOLS, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveStartAllowedSymbols.c_str()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_STARTSTRING_MINLENGTH, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveStartMinSymbols).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_STARTSTRING_MAXLENGTH, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveStartMaxSymbols).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RULE_ALLOWED_SYMBOLS, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveRuleAllowedSymbols.c_str()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RULE_MINLENGTH, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveRuleMinSymbols).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RULE_MAXLENGTH, true);
	editBox->setText(irr::core::stringw(LSystemRandom->RandomCaveRuleMaxSymbols).c_str());
}

void Editor::COptions::WindowApply_Cave()
{
	// get root element
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	// test if window active
	irr::gui::IGUIElement* window = root->getElementFromId(CGUI::EGuiId::WINDOW_CAVE, true);
	if (!window)
		return;

	// read values
	irr::video::SColorf color = DungeonGenerator->MaterialGetCaveColor();

	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_COLOR_CAVE_R, true);
	editBox->setText(irr::core::stringw(color.r).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_COLOR_CAVE_G, true);
	editBox->setText(irr::core::stringw(color.g).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_COLOR_CAVE_B, true);
	editBox->setText(irr::core::stringw(color.b).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ITERATION, true);
	editBox->setText(irr::core::stringw(DungeonGenerator->LSystemGetIteration()).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEYAW_VALUE, true);
	editBox->setText(irr::core::stringw(DungeonGenerator->LSystemGetParameter(DunGen::ELSystemParameter::ANGLE_YAW)).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEPITCH_VALUE, true);
	editBox->setText(irr::core::stringw(DungeonGenerator->LSystemGetParameter(DunGen::ELSystemParameter::ANGLE_PITCH)).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ANGLEROLL_VALUE, true);
	editBox->setText(irr::core::stringw(DungeonGenerator->LSystemGetParameter(DunGen::ELSystemParameter::ANGLE_ROLL)).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RADIUSSTART_VALUE, true);
	editBox->setText(irr::core::stringw(DungeonGenerator->LSystemGetParameter(DunGen::ELSystemParameter::RADIUS_START)).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_STARTSTRING_VALUE, true);
	editBox->setText(irr::core::stringw(DungeonGenerator->LSystemGetStart().c_str()).c_str());

	irr::gui::IGUICheckBox* checkBox =  (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_COLOR_CAVE_MULTICOLOR, true);
	checkBox->setChecked(DunGen::EMaterialType::CAVE_SINGLECOLOR == DungeonGenerator->MaterialGetCaveMaterial());
	checkBox =  (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_EROSION, true);
	checkBox->setChecked(0 < DungeonGenerator->VoxelCaveGetErosionSteps().size());
	checkBox =  (irr::gui::IGUICheckBox*) root->getElementFromId(CGUI::EGuiId::CHECKBOX_FILTERING, true);
	checkBox->setChecked(DungeonGenerator->VoxelCaveGetFiltering());

	// ListBox with rules
	std::vector<DunGen::SSubstitutionRule> rules = DungeonGenerator->LSystemGetRules();
	irr::gui::IGUIListBox* listBox = (irr::gui::IGUIListBox*) root->getElementFromId(CGUI::EGuiId::LISTBOX_RULES, true);
	int selected = listBox->getSelected();
	listBox->clear();
	std::string tmpString;
	for (unsigned int i=0; i < rules.size(); ++i)
	{
		tmpString = "0";
		tmpString[0] = rules[i].Symbol;
		tmpString += " -> ";
		tmpString += rules[i].Substitution;

		listBox->addItem(irr::core::stringw(tmpString.c_str()).c_str());
	}

	if (0 > selected || static_cast<int>(rules.size()) <= selected)
		selected = 0;

	if (static_cast<int>(rules.size()) > selected)
	{
		listBox->setSelected(selected);
		tmpString.clear();
		tmpString += rules[selected].Symbol;
		editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RULE_SYMBOL, true);
		editBox->setText(irr::core::stringw(tmpString.c_str()).c_str());
		editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RULE_REPLACEMENT, true);
		editBox->setText(irr::core::stringw(rules[selected].Substitution.c_str()).c_str());
	}
	else
	{
		editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RULE_SYMBOL, true);
		editBox->setText(L"");
		editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_RULE_REPLACEMENT, true);
		editBox->setText(L"");
	}
}

void Editor::COptions::WindowApply_ObjectPlacement()
{
	// get root element
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	// test if window active
	irr::gui::IGUIElement* window = root->getElementFromId(CGUI::EGuiId::WINDOW_OBJECTPLACEMENT, true);
	if (!window)
		return;

	// read values
	DunGen::SPortalObject portal = DungeonGenerator->ObjectsGetPortalObject();
	DunGen::SBossObject boss01 = DungeonGenerator->ObjectsGetBossObject(DunGen::EBossObject::BOSS_01);
	DunGen::SBossObject boss02 = DungeonGenerator->ObjectsGetBossObject(DunGen::EBossObject::BOSS_02);
	DunGen::SBossObject boss03 = DungeonGenerator->ObjectsGetBossObject(DunGen::EBossObject::BOSS_03);
	DunGen::SBossObject boss04 = DungeonGenerator->ObjectsGetBossObject(DunGen::EBossObject::BOSS_04);
	DunGen::SBossObject boss05 = DungeonGenerator->ObjectsGetBossObject(DunGen::EBossObject::BOSS_05);
	DunGen::SBossObject boss06 = DungeonGenerator->ObjectsGetBossObject(DunGen::EBossObject::BOSS_06);

	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_START_ROTATION_X, true);
	editBox->setText(irr::core::stringw(portal.Rotation.X).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_START_ROTATION_Y, true);
	editBox->setText(irr::core::stringw(portal.Rotation.Y).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_START_ROTATION_Z, true);
	editBox->setText(irr::core::stringw(portal.Rotation.Z).c_str());

	irr::gui::IGUIStaticText* staticText = (irr::gui::IGUIStaticText*) root->getElementFromId(CGUI::EGuiId::TEXTBOX_PLACED_START, true);
	staticText->setText(portal.IsPlaced ? L"Yes" : L"No");
	
	staticText = (irr::gui::IGUIStaticText*) root->getElementFromId(CGUI::EGuiId::TEXTBOX_PLACED_BOSS01, true);
	staticText->setText(boss01.IsPlaced ? L"Yes" : L"No");
	staticText = (irr::gui::IGUIStaticText*) root->getElementFromId(CGUI::EGuiId::TEXTBOX_PLACED_BOSS02, true);
	staticText->setText(boss02.IsPlaced ? L"Yes" : L"No");
	staticText = (irr::gui::IGUIStaticText*) root->getElementFromId(CGUI::EGuiId::TEXTBOX_PLACED_BOSS03, true);
	staticText->setText(boss03.IsPlaced ? L"Yes" : L"No");
	staticText = (irr::gui::IGUIStaticText*) root->getElementFromId(CGUI::EGuiId::TEXTBOX_PLACED_BOSS04, true);
	staticText->setText(boss04.IsPlaced ? L"Yes" : L"No");
	staticText = (irr::gui::IGUIStaticText*) root->getElementFromId(CGUI::EGuiId::TEXTBOX_PLACED_BOSS05, true);
	staticText->setText(boss05.IsPlaced ? L"Yes" : L"No");
	staticText = (irr::gui::IGUIStaticText*) root->getElementFromId(CGUI::EGuiId::TEXTBOX_PLACED_BOSS06, true);
	staticText->setText(boss06.IsPlaced ? L"Yes" : L"No");
}

void Editor::COptions::WindowApply_RandomObjects()
{
	// get root element
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	// test if window active
	irr::gui::IGUIElement* window = root->getElementFromId(CGUI::EGuiId::WINDOW_RANDOMOBJECTS, true);
	if (!window)
		return;

	// read values
	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_SHIP_RED, true);
	editBox->setText(irr::core::stringw(ObjectPlacement->ObjectFactors[DunGen::EObject::SHIP_RED]).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_SHIP_GREEN, true);
	editBox->setText(irr::core::stringw(ObjectPlacement->ObjectFactors[DunGen::EObject::SHIP_GREEN]).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_SHIP_BLUE, true);
	editBox->setText(irr::core::stringw(ObjectPlacement->ObjectFactors[DunGen::EObject::SHIP_BLUE]).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_SHIP_YELLOW, true);
	editBox->setText(irr::core::stringw(ObjectPlacement->ObjectFactors[DunGen::EObject::SHIP_YELLOW]).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_SHIP_TEAL, true);
	editBox->setText(irr::core::stringw(ObjectPlacement->ObjectFactors[DunGen::EObject::SHIP_TEAL]).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_SHIP_PURPLE, true);
	editBox->setText(irr::core::stringw(ObjectPlacement->ObjectFactors[DunGen::EObject::SHIP_PURPLE]).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_FACTOR_NO_SHIP, true);
	editBox->setText(irr::core::stringw(ObjectPlacement->ObjectFactors[DunGen::EObject::NO_SHIP]).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ADDITIONAL_DISTANCE_SHIPS, true);
	editBox->setText(irr::core::stringw(ObjectPlacement->AdditionalDistanceToShip).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ADDITIONAL_DISTANCE_BOSSES, true);
	editBox->setText(irr::core::stringw(ObjectPlacement->AdditionalDistanceToBoss).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_ADDITIONAL_DISTANCE_START, true);
	editBox->setText(irr::core::stringw(ObjectPlacement->AdditionalDistanceToPortal).c_str());
}

void Editor::COptions::WindowApply_Gameplay()
{
	// get root element
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	// test if window active
	irr::gui::IGUIElement* window = root->getElementFromId(CGUI::EGuiId::WINDOW_GAMEPLAY, true);
	if (!window)
		return;

	// read values
	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_MONSTER_HITPOINTS, true);
	editBox->setText(irr::core::stringw(DungeonGenerator->GetMonsterHitpoints()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_BOSSMONSTER_HITPOINTS, true);
	editBox->setText(irr::core::stringw(DungeonGenerator->GetBossMonsterHitpoints()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_MONSTER_FIRETIME, true);
	editBox->setText(irr::core::stringw(DungeonGenerator->GetMonsterFireTime()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_BOSSMONSTER_FIRETIME, true);
	editBox->setText(irr::core::stringw(DungeonGenerator->GetBossMonsterFireTime()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_MONSTER_DAMAGE, true);
	editBox->setText(irr::core::stringw(DungeonGenerator->GetMonsterDamage()).c_str());
}

void Editor::COptions::WindowApply_Description()
{
	// get root element
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	// test if window active
	irr::gui::IGUIElement* window = root->getElementFromId(CGUI::EGuiId::WINDOW_DESCRIPTION, true);
	if (!window)
		return;

	// read values
	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_DESCRIPTION_INTRO, true);
	editBox->setText(DungeonGenerator->GetDescriptionIntro().c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(CGUI::EGuiId::EDITBOX_DESCRIPTION_EXTRO, true);
	editBox->setText(DungeonGenerator->GetDescriptionExtro().c_str());
}
