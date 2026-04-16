// Copyright (C) 2011-2014 by Maximilian Hönig

#include "EventReceiver.h"
#include "Gui.h"
#include "Options.h"

CEventReceiver::CEventReceiver()
	: ExeNameToStart()
{
}

CEventReceiver::~CEventReceiver()
{
}

void CEventReceiver::Intialize(Launcher::COptions* options_, Launcher::CGUI* gui_,  irr::gui::IGUIEnvironment* guiEnvironment_)
{
	Options = options_;
	Gui = gui_;
	GuiEnvironment = guiEnvironment_;
	Gui->CreateMenue();
	Options->Load();
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
		// button clicked
		case irr::gui::EGET_BUTTON_CLICKED:
			switch(id)
			{
			// buttons
			case Launcher::CGUI::EGuiId::BUTTON_DEFAULTS:
				Options->Defaults();
				Options->Save();
				break;
			case Launcher::CGUI::EGuiId::BUTTON_START_NIGHTORK:
				Options->Save();
				ExeNameToStart = std::wstring(L"Nightork3.exe");
				break;
			}
			break;
		}
	}
	return false;
}

std::wstring CEventReceiver::GetExeNameToStart() const
{
	return ExeNameToStart;
}
