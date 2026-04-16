// Copyright (C) 2011-2014 by Maximilian Hönig

#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht.h>
#include <string>

namespace Launcher
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
	void Intialize(Launcher::COptions* options_, Launcher::CGUI* gui_, irr::gui::IGUIEnvironment* guiEnvironment_);

	/// What to do on an event?
	virtual bool OnEvent(const irr::SEvent& event_);

	/// Which executable shall be launched? Empty ... none.
	std::wstring GetExeNameToStart() const;

private:
	irr::gui::IGUIEnvironment* GuiEnvironment;	///< The irrlicht gui environment.
	Launcher::CGUI* Gui;							///< The GUI for creating menu elements.
	Launcher::COptions* Options;					///< The options to interact.
	std::wstring ExeNameToStart;				///< Which executable shall be launched? Empty ... none.

};

#endif