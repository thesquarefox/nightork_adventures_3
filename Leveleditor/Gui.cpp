// Copyright (C) 2011-2014 by Maximilian Hönig

#include "GUI.h"
#include "CGUIFileOpenDialog_Modified.h"

// Construktor
Editor::CGUI::CGUI(irr::gui::IGUIEnvironment* guiEnvironment_)
: GUIEnvironment(guiEnvironment_)
{
	// change gui transparencies
	for (irr::u32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
	{
		irr::video::SColor color = GUIEnvironment->getSkin()->getColor((irr::gui::EGUI_DEFAULT_COLOR)i);
		color.setAlpha(255);
		GUIEnvironment->getSkin()->setColor((irr::gui::EGUI_DEFAULT_COLOR)i, color);
	}
}

// create main menue
void Editor::CGUI::CreateMenue()
{
	irr::gui::IGUIToolBar* toolbar = GUIEnvironment->addToolBar();
	toolbar->addButton(EGuiId::BUTTON_LOAD, L"  Load  ");
	toolbar->addButton(EGuiId::BUTTON_SAVE, L"  Save  ");
	toolbar->addButton(EGuiId::BUTTON_RANDOMCAVE, L"Random Cave");
	toolbar->addButton(EGuiId::BUTTON_CAVE, L"  Cave  ");
	toolbar->addButton(EGuiId::BUTTON_OBJECTPLACEMENT, L"Object Placement");
	toolbar->addButton(EGuiId::BUTTON_RANDOMOBJECTS, L"Random Objects");
	toolbar->addButton(EGuiId::BUTTON_GAMEPLAY, L" Gameplay ");
	toolbar->addButton(EGuiId::BUTTON_DESCRIPTION, L" Description ");
}

// create random caves window
void Editor::CGUI::CreateWindow_RandomCave()
{
	// close window if already present
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	irr::gui::IGUIElement* element = root->getElementFromId(EGuiId::WINDOW_RANDOMCAVE, true);
	if (element)
	{
		// remove
		element->remove();
		return;
	}

	// create window
	irr::gui::IGUIWindow* window = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,100,350,390),
		false, L"Random Cave", 0, EGuiId::WINDOW_RANDOMCAVE);

	GUIEnvironment->addStaticText(L"Iteration:", irr::core::rect<irr::s32>(10,25,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,20,120,40), true, window, EGuiId::EDITBOX_ITERATION_RANDOM);

	// parameter
	GUIEnvironment->addStaticText(L"Min", irr::core::rect<irr::s32>(80,48,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"Max", irr::core::rect<irr::s32>(140,48,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"Digits", irr::core::rect<irr::s32>(200,48,999,999), false, false, window);

	GUIEnvironment->addStaticText(L"Angle Yaw:", irr::core::rect<irr::s32>(10,65,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"Angle Pitch:", irr::core::rect<irr::s32>(10,85,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"Angle Roll:", irr::core::rect<irr::s32>(10,105,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"Radius:", irr::core::rect<irr::s32>(10,125,999,999), false, false, window);

	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,60,120,80), true, window, EGuiId::EDITBOX_ANGLEYAW_MIN);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(140,60,180,80), true, window, EGuiId::EDITBOX_ANGLEYAW_MAX);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,60,240,80), true, window, EGuiId::EDITBOX_ANGLEYAW_DIGITS);

	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,80,120,100), true, window, EGuiId::EDITBOX_ANGLEPITCH_MIN);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(140,80,180,100), true, window, EGuiId::EDITBOX_ANGLEPITCH_MAX);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,80,240,100), true, window, EGuiId::EDITBOX_ANGLEPITCH_DIGITS);	

	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,100,120,120), true, window, EGuiId::EDITBOX_ANGLEROLL_MIN);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(140,100,180,120), true, window, EGuiId::EDITBOX_ANGLEROLL_MAX);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,100,240,120), true, window, EGuiId::EDITBOX_ANGLEROLL_DIGITS);

	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,120,120,140), true, window, EGuiId::EDITBOX_RADIUSSTART_MIN);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(140,120,180,140), true, window, EGuiId::EDITBOX_RADIUSSTART_MAX);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,120,240,140), true, window, EGuiId::EDITBOX_RADIUSSTART_DIGITS);

	// start string
	GUIEnvironment->addStaticText(L"Startstring:", irr::core::rect<irr::s32>(10,155,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"Allowed Symbols:", irr::core::rect<irr::s32>(10,175,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,170,240,190), true, window, EGuiId::EDITBOX_STARTSTRING_ALLOWED_SYMBOLS);
	GUIEnvironment->addStaticText(L"Min. Length:", irr::core::rect<irr::s32>(10,195,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,190,120,210), true, window, EGuiId::EDITBOX_STARTSTRING_MINLENGTH);
	GUIEnvironment->addStaticText(L"Max. Length:", irr::core::rect<irr::s32>(130,195,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,190,240,210), true, window, EGuiId::EDITBOX_STARTSTRING_MAXLENGTH);

	// rules
	GUIEnvironment->addStaticText(L"Rules:", irr::core::rect<irr::s32>(10,225,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"Allowed Symbols:", irr::core::rect<irr::s32>(10,245,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,240,240,260), true, window, EGuiId::EDITBOX_RULE_ALLOWED_SYMBOLS);
	GUIEnvironment->addStaticText(L"Min. Length:", irr::core::rect<irr::s32>(10,265,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,260,120,280), true, window, EGuiId::EDITBOX_RULE_MINLENGTH);
	GUIEnvironment->addStaticText(L"Max. Length", irr::core::rect<irr::s32>(130,265,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,260,240,280), true, window, EGuiId::EDITBOX_RULE_MAXLENGTH);
}

void Editor::CGUI::CreateWindow_Cave()
{
	// close window if already present
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	irr::gui::IGUIElement* element = root->getElementFromId(EGuiId::WINDOW_CAVE, true);
	if (element)
	{
		// remove
		element->remove();
		return;
	}

	// create window
	irr::gui::IGUIWindow* window = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,100,350,590),
		false, L"Cave", 0, EGuiId::WINDOW_CAVE);

	// material
	GUIEnvironment->addStaticText(L"Cave Color (0.0 to 1.0):", irr::core::rect<irr::s32>(10,25,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"Red:", irr::core::rect<irr::s32>(10,45,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(40,40,80,60), true, window, EGuiId::EDITBOX_COLOR_CAVE_R);
	GUIEnvironment->addStaticText(L"Green:", irr::core::rect<irr::s32>(90,45,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(120,40,160,60), true, window, EGuiId::EDITBOX_COLOR_CAVE_G);
	GUIEnvironment->addStaticText(L"Blue:", irr::core::rect<irr::s32>(170,45,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,40,240,60), true, window, EGuiId::EDITBOX_COLOR_CAVE_B);

	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(40,65,240,85),window, EGuiId::CHECKBOX_COLOR_CAVE_MULTICOLOR,L"Use Single Color");

	// parameter
	GUIEnvironment->addStaticText(L"Iteration:", irr::core::rect<irr::s32>(10,105,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,100,120,120), true, window, EGuiId::EDITBOX_ITERATION);

	GUIEnvironment->addStaticText(L"Angle Yaw:", irr::core::rect<irr::s32>(10,125,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,120,120,140), true, window, EGuiId::EDITBOX_ANGLEYAW_VALUE);

	GUIEnvironment->addStaticText(L"Angle Pitch:", irr::core::rect<irr::s32>(10,145,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,140,120,160), true, window, EGuiId::EDITBOX_ANGLEPITCH_VALUE);

	GUIEnvironment->addStaticText(L"Angle Roll:", irr::core::rect<irr::s32>(10,165,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,160,120,180), true, window, EGuiId::EDITBOX_ANGLEROLL_VALUE);

	GUIEnvironment->addStaticText(L"Radius:", irr::core::rect<irr::s32>(10,185,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,180,120,200), true, window, EGuiId::EDITBOX_RADIUSSTART_VALUE);
	
	// start string
	GUIEnvironment->addStaticText(L"Startstring:", irr::core::rect<irr::s32>(10,215,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,210,240,230), true, window, EGuiId::EDITBOX_STARTSTRING_VALUE);

	// rules
	GUIEnvironment->addStaticText(L"Rules:", irr::core::rect<irr::s32>(10,245,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"Symbol", irr::core::rect<irr::s32>(10,265,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,260,120,280), true, window, EGuiId::EDITBOX_RULE_SYMBOL);
	GUIEnvironment->addStaticText(L"Replacement:", irr::core::rect<irr::s32>(10,285,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,280,240,300), true, window, EGuiId::EDITBOX_RULE_REPLACEMENT);

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,305,125,320), window, EGuiId::BUTTON_RULE_ADD, L"Add Rule");
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(125,305,240,320), window, EGuiId::BUTTON_RULE_DELETEALL, L"Delete Rules");

	GUIEnvironment->addStaticText(L"Current Rules:", irr::core::rect<irr::s32>(10,325,999,999), false, false, window);
	GUIEnvironment->addListBox(irr::core::rect<irr::s32>(10,340,240,420), window, EGuiId::LISTBOX_RULES, true);

	// post processing
	GUIEnvironment->addStaticText(L"Postprocessing:", irr::core::rect<irr::s32>(10,425,999,999), false, false, window);
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,440,240,460),window, EGuiId::CHECKBOX_EROSION,L"Use Erosion");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,460,240,480),window, EGuiId::CHECKBOX_FILTERING,L"Remove hovering Fragments");
}

void Editor::CGUI::CreateWindow_ObjectPlacement()
{
	// close window if already present
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	irr::gui::IGUIElement* element = root->getElementFromId(EGuiId::WINDOW_OBJECTPLACEMENT, true);
	if (element)
	{
		// remove
		element->remove();
		return;
	}

	// create window
	irr::gui::IGUIWindow* window = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,100,230,585),
		false, L"Object Placement", 0, EGuiId::WINDOW_OBJECTPLACEMENT);

	// start portal
	GUIEnvironment->addStaticText(L"Start Portal:", irr::core::rect<irr::s32>(10,25,999,999), false, false, window);

	GUIEnvironment->addStaticText(L"Placed:", irr::core::rect<irr::s32>(10,45,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"No", irr::core::rect<irr::s32>(80,45,999,999), false, false, window, EGuiId::TEXTBOX_PLACED_START);

	GUIEnvironment->addStaticText(L"Rotation X:", irr::core::rect<irr::s32>(10,65,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,60,120,80), true, window, EGuiId::EDITBOX_START_ROTATION_X);
	GUIEnvironment->addStaticText(L"Rotation Y:", irr::core::rect<irr::s32>(10,85,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,80,120,100), true, window, EGuiId::EDITBOX_START_ROTATION_Y);
	GUIEnvironment->addStaticText(L"Rotation Z:", irr::core::rect<irr::s32>(10,105,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,100,120,120), true, window, EGuiId::EDITBOX_START_ROTATION_Z);

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,125,65,145), window, EGuiId::BUTTON_DELETE_START, L"Delete");
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(65,125,120,145), window, EGuiId::BUTTON_APPLY_START, L"Apply");

	// bosses
	GUIEnvironment->addStaticText(L"Boss Ships:", irr::core::rect<irr::s32>(10,165,999,999), false, false, window);

	GUIEnvironment->addStaticText(L"Boss01 placed:", irr::core::rect<irr::s32>(10,185,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"No", irr::core::rect<irr::s32>(80,185,999,999), false, false, window, EGuiId::TEXTBOX_PLACED_BOSS01);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,205,120,225), window, EGuiId::BUTTON_DELETE_BOSS01, L"Delete");

	GUIEnvironment->addStaticText(L"Boss02 placed:", irr::core::rect<irr::s32>(10,235,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"No", irr::core::rect<irr::s32>(80,235,999,999), false, false, window, EGuiId::TEXTBOX_PLACED_BOSS02);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,255,120,275), window, EGuiId::BUTTON_DELETE_BOSS02, L"Delete");

	GUIEnvironment->addStaticText(L"Boss03 placed:", irr::core::rect<irr::s32>(10,285,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"No", irr::core::rect<irr::s32>(80,285,999,999), false, false, window, EGuiId::TEXTBOX_PLACED_BOSS03);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,305,120,325), window, EGuiId::BUTTON_DELETE_BOSS03, L"Delete");

	GUIEnvironment->addStaticText(L"Boss04 placed:", irr::core::rect<irr::s32>(10,335,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"No", irr::core::rect<irr::s32>(80,335,999,999), false, false, window, EGuiId::TEXTBOX_PLACED_BOSS04);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,355,120,375), window, EGuiId::BUTTON_DELETE_BOSS04, L"Delete");

	GUIEnvironment->addStaticText(L"Boss05 placed:", irr::core::rect<irr::s32>(10,385,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"No", irr::core::rect<irr::s32>(80,385,999,999), false, false, window, EGuiId::TEXTBOX_PLACED_BOSS05);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,405,120,425), window, EGuiId::BUTTON_DELETE_BOSS05, L"Delete");

	GUIEnvironment->addStaticText(L"Boss06 placed:", irr::core::rect<irr::s32>(10,435,999,999), false, false, window);
	GUIEnvironment->addStaticText(L"No", irr::core::rect<irr::s32>(80,435,999,999), false, false, window, EGuiId::TEXTBOX_PLACED_BOSS06);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,455,120,475), window, EGuiId::BUTTON_DELETE_BOSS06, L"Delete");

}

void Editor::CGUI::CreateWindow_RandomObjects()
{
	// close window if already present
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	irr::gui::IGUIElement* element = root->getElementFromId(EGuiId::WINDOW_RANDOMOBJECTS, true);
	if (element)
	{
		// remove
		element->remove();
		return;
	}

	// create window
	irr::gui::IGUIWindow* window = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,100,230,380),
		false, L"Random Objects", 0, EGuiId::WINDOW_RANDOMOBJECTS);

	GUIEnvironment->addStaticText(L"Probability Factors Ships:", irr::core::rect<irr::s32>(10,25,999,999), false, false, window);

	GUIEnvironment->addStaticText(L"Ship Red:", irr::core::rect<irr::s32>(10,45,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,40,120,60), true, window, EGuiId::EDITBOX_FACTOR_SHIP_RED);
	GUIEnvironment->addStaticText(L"Ship Green:", irr::core::rect<irr::s32>(10,65,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,60,120,80), true, window, EGuiId::EDITBOX_FACTOR_SHIP_GREEN);
	GUIEnvironment->addStaticText(L"Ship Blue:", irr::core::rect<irr::s32>(10,85,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,80,120,100), true, window, EGuiId::EDITBOX_FACTOR_SHIP_BLUE);
	GUIEnvironment->addStaticText(L"Ship Yellow:", irr::core::rect<irr::s32>(10,105,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,100,120,120), true, window, EGuiId::EDITBOX_FACTOR_SHIP_YELLOW);
	GUIEnvironment->addStaticText(L"Ship Teal:", irr::core::rect<irr::s32>(10,125,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,120,120,140), true, window, EGuiId::EDITBOX_FACTOR_SHIP_TEAL);
	GUIEnvironment->addStaticText(L"Ship Purple:", irr::core::rect<irr::s32>(10,145,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,140,120,160), true, window, EGuiId::EDITBOX_FACTOR_SHIP_PURPLE);
	GUIEnvironment->addStaticText(L"No Ship:", irr::core::rect<irr::s32>(10,165,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,160,120,180), true, window, EGuiId::EDITBOX_FACTOR_NO_SHIP);

	GUIEnvironment->addStaticText(L"Additional Distance to:", irr::core::rect<irr::s32>(10,195,999,999), false, false, window);

	GUIEnvironment->addStaticText(L"Other ships:", irr::core::rect<irr::s32>(10,215,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,210,120,230), true, window, EGuiId::EDITBOX_ADDITIONAL_DISTANCE_SHIPS);
	GUIEnvironment->addStaticText(L"Boss ships:", irr::core::rect<irr::s32>(10,235,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,230,120,250), true, window, EGuiId::EDITBOX_ADDITIONAL_DISTANCE_BOSSES);
	GUIEnvironment->addStaticText(L"Start Location:", irr::core::rect<irr::s32>(10,255,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,250,120,270), true, window, EGuiId::EDITBOX_ADDITIONAL_DISTANCE_START);
}

void Editor::CGUI::CreateWindow_Gameplay()
{
	// close window if already present
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	irr::gui::IGUIElement* element = root->getElementFromId(EGuiId::WINDOW_GAMEPLAY, true);
	if (element)
	{
		// remove
		element->remove();
		return;
	}

	// create window
	irr::gui::IGUIWindow* window = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,100,270,250),
		false, L"Gameplay", 0, EGuiId::WINDOW_GAMEPLAY);

	GUIEnvironment->addStaticText(L"Parameters for Monsters:", irr::core::rect<irr::s32>(10,25,999,999), false, false, window);

	GUIEnvironment->addStaticText(L"Hitpoints:", irr::core::rect<irr::s32>(10,45,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(120,40,160,60), true, window, EGuiId::EDITBOX_MONSTER_HITPOINTS);
	GUIEnvironment->addStaticText(L"Boss Hitpoints:", irr::core::rect<irr::s32>(10,65,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(120,60,160,80), true, window, EGuiId::EDITBOX_BOSSMONSTER_HITPOINTS);
	GUIEnvironment->addStaticText(L"Base Firetime (ms):", irr::core::rect<irr::s32>(10,85,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(120,80,160,100), true, window, EGuiId::EDITBOX_MONSTER_FIRETIME);
	GUIEnvironment->addStaticText(L"Boss Base Firetime (ms):", irr::core::rect<irr::s32>(10,105,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(120,100,160,120), true, window, EGuiId::EDITBOX_BOSSMONSTER_FIRETIME);
	GUIEnvironment->addStaticText(L"Damage:", irr::core::rect<irr::s32>(10,125,999,999), false, false, window);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(120,120,160,140), true, window, EGuiId::EDITBOX_MONSTER_DAMAGE);
}

void Editor::CGUI::CreateWindow_Description()
{
	// close window if already present
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
	irr::gui::IGUIElement* element = root->getElementFromId(EGuiId::WINDOW_DESCRIPTION, true);
	if (element)
	{
		// remove
		element->remove();
		return;
	}

	// create window
	irr::gui::IGUIWindow* window = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,100,320,570),
		false, L"Description", 0, EGuiId::WINDOW_DESCRIPTION);

	GUIEnvironment->addStaticText(L"Intro Text:", irr::core::rect<irr::s32>(10,25,999,999), false, false, window);
	irr::gui::IGUIEditBox* eb = GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(10,40,210,240), true, window, EGuiId::EDITBOX_DESCRIPTION_INTRO);
	eb->setWordWrap(true);
	eb->setMultiLine(true);
	GUIEnvironment->addStaticText(L"Extro Text:", irr::core::rect<irr::s32>(10,245,999,999), false, false, window);
	eb = GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(10,260,210,460), true, window, EGuiId::EDITBOX_DESCRIPTION_EXTRO);
	eb->setWordWrap(true);
	eb->setMultiLine(true);
}

void Editor::CGUI::CreateFileLoadDialog()
{
	irr::gui::IGUIFileOpenDialog* fileOpenDialog =
		new irr::gui::CGUIFileOpenDialog_Modified(L"Please select File to Load", GUIEnvironment, GUIEnvironment->getRootGUIElement(), EGuiId::FILEOPENDIALOG_LOAD);
	fileOpenDialog->drop();
}

void Editor::CGUI::CreateFileSaveDialog()
{
	irr::gui::IGUIFileOpenDialog* fileOpenDialog =
		new irr::gui::CGUIFileOpenDialog_Modified(L"Please select File to Save", GUIEnvironment, GUIEnvironment->getRootGUIElement(), EGuiId::FILEOPENDIALOG_SAVE);
	fileOpenDialog->drop();
}
