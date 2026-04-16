// Copyright (C) 2011-2014 by Maximilian Hönig

#ifndef OPTIONS_H
#define OPTIONS_H

#include <irrlicht.h>

namespace Launcher
{
	class COptions
	{
	public:
		/// Constructor.
		COptions(irr::IrrlichtDevice* irrDevice_);

		/// Destructor.
		~COptions();

		/// Store settings.
		void Save();

		/// Load Settings.
		void Load();

		/// Load default setting.
		void Defaults();

	private:
		irr::IrrlichtDevice* IrrDevice;
		irr::gui::IGUIEnvironment* GUIEnvironment;
	};
};

#endif