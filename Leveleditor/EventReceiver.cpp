// Copyright (C) 2011-2014 by Maximilian Hönig

#include "EventReceiver.h"
#include "Gui.h"
#include "Options.h"

CEventReceiver::CEventReceiver()
{
}

CEventReceiver::~CEventReceiver()
{
}

void CEventReceiver::Intialize(Editor::COptions* options_, Editor::CGUI* gui_,  irr::gui::IGUIEnvironment* guiEnvironment_)
{
	Options = options_;
	Gui = gui_;
	GuiEnvironment = guiEnvironment_;
	Gui->CreateMenue();
}

bool CEventReceiver::OnEvent(const irr::SEvent& event_)
{
	if (event_.EventType == irr::EET_GUI_EVENT)
	{		
		// get id of element
		irr::s32 id = event_.GUIEvent.Caller->getID();

		// type of element
		switch(event_.GUIEvent.EventType)
		{
		// closing windows
		case irr::gui::EGDT_WINDOW_CLOSE:
			GuiEnvironment->setFocus(NULL);
		case irr::gui::EGET_LISTBOX_CHANGED:
			Options->WindowsStore();
			Options->WindowsApply();
			break;

		// button clicked
		case irr::gui::EGET_BUTTON_CLICKED:
			Options->WindowsStore();
			switch(id)
			{
			// toolbar buttons
			case Editor::CGUI::EGuiId::BUTTON_SAVE:
				Gui->CreateFileSaveDialog();
				break;
			case Editor::CGUI::EGuiId::BUTTON_LOAD:
				Gui->CreateFileLoadDialog();
				break;
			case Editor::CGUI::EGuiId::BUTTON_RANDOMCAVE:
				Gui->CreateWindow_RandomCave();
				break;
			case Editor::CGUI::EGuiId::BUTTON_CAVE:
				Gui->CreateWindow_Cave();
				break;
			case Editor::CGUI::EGuiId::BUTTON_OBJECTPLACEMENT:
				Gui->CreateWindow_ObjectPlacement();
				break;
			case Editor::CGUI::EGuiId::BUTTON_RANDOMOBJECTS:
				Gui->CreateWindow_RandomObjects();
				break;
			case Editor::CGUI::EGuiId::BUTTON_GAMEPLAY:
				Gui->CreateWindow_Gameplay();
				break;
			case Editor::CGUI::EGuiId::BUTTON_DESCRIPTION:
				Gui->CreateWindow_Description();
				break;
			// other buttons
			case Editor::CGUI::EGuiId::BUTTON_RULE_ADD:
				Options->AddRule();
				break;
			case Editor::CGUI::EGuiId::BUTTON_RULE_DELETEALL:
				Options->DeleteRules();
				break;
			case Editor::CGUI::EGuiId::BUTTON_APPLY_START:
				Options->PortalRotationApply();
				break;
			case Editor::CGUI::EGuiId::BUTTON_DELETE_START:
				Options->DeleteObject(0);
				break;
			case Editor::CGUI::EGuiId::BUTTON_DELETE_BOSS01:
				Options->DeleteObject(1);
				break;
			case Editor::CGUI::EGuiId::BUTTON_DELETE_BOSS02:
				Options->DeleteObject(2);
				break;
			case Editor::CGUI::EGuiId::BUTTON_DELETE_BOSS03:
				Options->DeleteObject(3);
				break;
			case Editor::CGUI::EGuiId::BUTTON_DELETE_BOSS04:
				Options->DeleteObject(4);
				break;
			case Editor::CGUI::EGuiId::BUTTON_DELETE_BOSS05:
				Options->DeleteObject(5);
				break;
			case Editor::CGUI::EGuiId::BUTTON_DELETE_BOSS06:
				Options->DeleteObject(6);
				break;
			}
			Options->WindowsApply();
			break;

		case irr::gui::EGET_FILE_SELECTED:
			Options->WindowsStore();
			irr::gui::IGUIFileOpenDialog* dialog =  (irr::gui::IGUIFileOpenDialog*)event_.GUIEvent.Caller;
			switch(id)
			{
			case Editor::CGUI::EGuiId::FILEOPENDIALOG_SAVE:
				Options->SaveFile(dialog->getFileName());
				break;
			case Editor::CGUI::EGuiId::FILEOPENDIALOG_LOAD:
				Options->LoadFile(dialog->getFileName());
				break;
			}
			Options->WindowsApply();
			break;
		}
	}
	else if (event_.EventType == irr::EET_KEY_INPUT_EVENT && false == event_.KeyInput.PressedDown
		&& 0 == GuiEnvironment->getFocus()) // no window focussed
	{
		switch(event_.KeyInput.Key)
		{
		case irr::KEY_F1:
			Options->WindowsStore();
			Options->SetParametersRandom();
			Options->AssembleCave();
			Options->WindowsApply();
			break;
		case irr::KEY_F2:
			Options->WindowsStore();
			Options->AssembleCave();
			Options->WindowsApply();
			break;
		case irr::KEY_F3:
			Options->WindowsStore();
			Options->IterationMinus();
			Options->AssembleCave();
			Options->WindowsApply();
			break;
		case irr::KEY_F4:
			Options->WindowsStore();
			Options->IterationPlus();
			Options->AssembleCave();
			Options->WindowsApply();
			break;
		case irr::KEY_F5:
			Options->WindowsStore();
			Options->PlaceObjects();
			Options->WindowsApply();
			break;
		case irr::KEY_F7:
			Options->WindowsStore();
			Options->GetCamera()->setInputReceiverEnabled(true);
			Options->WindowsApply();
			break;
		case irr::KEY_F8:
			Options->WindowsStore();
			Options->GetCamera()->setInputReceiverEnabled(false);
			Options->WindowsApply();
			break;
		case irr::KEY_PRINT:
		case irr::KEY_SNAPSHOT:
			Options->TakeScreenshot();
			break;

		case irr::KEY_KEY_7:
			Options->WindowsStore();
			Options->PlaceObject(0);
			Options->WindowsApply();
			break;
		case irr::KEY_KEY_1:
			Options->WindowsStore();
			Options->PlaceObject(1);
			Options->WindowsApply();
			break;
		case irr::KEY_KEY_2:
			Options->WindowsStore();
			Options->PlaceObject(2);
			Options->WindowsApply();
			break;
		case irr::KEY_KEY_3:
			Options->WindowsStore();
			Options->PlaceObject(3);
			Options->WindowsApply();
			break;
		case irr::KEY_KEY_4:
			Options->WindowsStore();
			Options->PlaceObject(4);
			Options->WindowsApply();
			break;
		case irr::KEY_KEY_5:
			Options->WindowsStore();
			Options->PlaceObject(5);
			Options->WindowsApply();
			break;
		case irr::KEY_KEY_6:
			Options->WindowsStore();
			Options->PlaceObject(6);
			Options->WindowsApply();
			break;

		}
	}
	return false;
}

