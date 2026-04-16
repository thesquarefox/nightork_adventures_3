// Copyright (C) 2011-2014 by Maximilian Hönig

#ifndef GUI_H
#define GUI_H

#include <irrlicht.h>

namespace Launcher
{
	// GUI class
	class CGUI
	{
	public:
		struct EGuiId
		{
			enum Enum
			{
				CHECKBOX_NIGHTORK_FULLSCREEN,
				CHECKBOX_NIGHTORK_VSYNC,
				CHECKBOX_NIGHTORK_ANTIALIASING,
				EDITBOX_NIGHTORK_RESOLUTION_X,
				EDITBOX_NIGHTORK_RESOLUTION_Y,
				COMBOBOX_NIGHTORK_RENDERER,

				BUTTON_DEFAULTS,
				BUTTON_START_NIGHTORK
			};
		};

	public:
		CGUI(irr::gui::IGUIEnvironment* guiEnvironment_, irr::video::IVideoDriver* videoDriver_);

		void CreateMenue();

	private:
		// GUI Environment
		irr::gui::IGUIEnvironment* GUIEnvironment;
		irr::video::ITexture* TextureBackground;
		irr::video::ITexture* TextureLogo;
	};
	
} // END NAMESPACE DunGen

#endif