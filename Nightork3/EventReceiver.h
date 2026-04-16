// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include "Pilot.h"
#include <irrlicht.h>

namespace Nightork
{

class CGameStates;

class CEventReceiver : public irr::IEventReceiver
{
public:
	/// Constructor.
	CEventReceiver();

	/// Destructor.
	~CEventReceiver();

	/// Initialize CEventreceiver.
	void Initialize(irr::IrrlichtDevice* irrDevice_, CGameStates* gameStates_);

	/// Is a specific key pressed?
	bool IsKeyPressed(irr::EKEY_CODE keyCode_) const;

	/// Is certain maneuver active?
	bool IsManeuver(CPilot::SControls::Enum maneuver_);

	/// Read relative mouse position to screen center and reset it to screen center.
	irr::core::position2df CEventReceiver::GetRelativeMousePosition();

	/// Is the cancel game overlay active?
	bool IsCancelGameOverlayActive();

	/// Shall the program be ended?
	bool GetEndProgram();

	/// What to do on an event?
	virtual bool OnEvent(const irr::SEvent& event_);

	/// Clear all key pressed and wasPressed.
	void ClearKeys();

private:
	irr::gui::ICursorControl* CursorControl;		///< The mouse cursor control.
	irr::video::IVideoDriver* VideoDriver;			///< The video driver.
	CGameStates* GameStates;						///< The game states.

	bool KeyPressed[irr::KEY_KEY_CODES_COUNT+3];	///< Which keys are pressed currently? Last three: Left, Right, Middle mouse button
	bool KeyWasPressed[irr::KEY_KEY_CODES_COUNT+3];	///< Which keys were pressed since last request? Last three: Left, Right, Middle mouse button
	CPilot::SControls::Enum ActiveControlButton;	///< The currently active control button.
	bool CancelGameOverlayActive;					///< Is the cancel game overlay active?
	bool EndProgram;								///< Shall the program be ended?
};

}

#endif
