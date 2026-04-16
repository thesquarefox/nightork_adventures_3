// Copyright (C) 2011-2014 by Maximilian Hönig

#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht.h>

namespace Editor
{
	class CGUI;
	class COptions;
}

class CEventReceiver : public irr::IEventReceiver
{
public:
	/// Constructor.
	CEventReceiver();

	/// Destructor.
	~CEventReceiver();

	/// Initialize the object.
	void Intialize(Editor::COptions* options_, Editor::CGUI* gui_, irr::gui::IGUIEnvironment* guiEnvironment_);

	/// What to do on an event?
	virtual bool OnEvent(const irr::SEvent& event_);

private:
	irr::gui::IGUIEnvironment* GuiEnvironment;	///< The irrlicht gui environment.
	Editor::CGUI* Gui;							///< The GUI for creating menu elements.
	Editor::COptions* Options;					///< The options to interact.

};

#endif