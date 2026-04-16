// Copyright (C) 2011-2014 by Maximilian Hönig

#include <iostream>
#include <irrlicht.h>
#include <fstream>
#include <steam_api.h>
#include "EventReceiver.h"
#include "Gui.h"
#include "Options.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "DunGen.lib")
#pragma comment(lib, "steam_api64.lib")
#endif

int main(int argc, char* argv[])
{
	bool steamAvailable = SteamAPI_Init();
	std::cout << "Steam status: " << steamAvailable << std::endl;

	unsigned int resolution_x = 1024;
	unsigned int resolution_y = 768;
	unsigned int antialiasing = 0;
	bool fullscreen = false;
	bool vsync = false;
	bool directX = true;

	std::ifstream infile("Editor.cfg", std::ifstream::in & std::ifstream::binary);
	if (infile)
	{
		infile >> resolution_x;
		infile >> resolution_y;
		infile >> fullscreen;
		infile >> vsync;
		infile >> antialiasing;
		infile >> directX;
	}

	std::cout << "Leveleditor is started." << std::endl;
	std::cout << "Resolution is: " << resolution_x << " * " << resolution_y << " , FullScreen: " << fullscreen
		<< " , AntiAliasing: " <<  antialiasing << " , VSync: " << vsync << " , " << (directX ? "DirectX9" : "OpenGL") << std::endl;

	// create Irrlicht device
	CEventReceiver eventReceiver;
	irr::SIrrlichtCreationParameters irrlichtParameter;
	irrlichtParameter.DriverType = directX ? irr::video::EDT_DIRECT3D9 : irr::video::EDT_OPENGL;
	irrlichtParameter.WindowSize = irr::core::dimension2du(resolution_x,resolution_y);
	irrlichtParameter.Bits = 32;
	irrlichtParameter.Fullscreen = fullscreen;
	irrlichtParameter.Stencilbuffer = false;
	irrlichtParameter.Vsync = vsync;
	irrlichtParameter.AntiAlias = 255 < antialiasing ? 255 : antialiasing;
	irrlichtParameter.EventReceiver = &eventReceiver;

	irr::IrrlichtDevice* irrDevice = irr::createDeviceEx(irrlichtParameter);
	if(!irrDevice)
	{
		if (directX)
		{
			std::cout << "Could not create DirectX 9 Irrlicht device. Switching to OpenGL." << std::endl;
			irrlichtParameter.DriverType = irr::video::EDT_OPENGL;
			irrDevice = irr::createDeviceEx(irrlichtParameter);
		}
		else
		{
			std::cout << "Could not create OpenGL Irrlicht device. Switching to DirectX 9." << std::endl;
			irrlichtParameter.DriverType = irr::video::EDT_DIRECT3D9;
			irrDevice = irr::createDeviceEx(irrlichtParameter);
		}
	}
	if(!irrDevice)
	{
		std::cout << "Irrlicht device could not be created, program is terminated." << std::endl;
		return 1;
	}
	irr::scene::ISceneManager* sceneManager = irrDevice->getSceneManager();
	irr::video::IVideoDriver* videoDriver = irrDevice->getVideoDriver();
	irr::gui::IGUIEnvironment* guiEnvironment = irrDevice->getGUIEnvironment();

	Editor::CGUI* gui = new Editor::CGUI(guiEnvironment);
	Editor::COptions* options = new Editor::COptions(irrDevice);
	eventReceiver.Intialize(options, gui, guiEnvironment);

	irrDevice->setWindowCaption(L"Leveleditor");

	irr::s32 lastFPS(0), actFPS(0);
	irr::core::vector3d<int> lastCamPos(-1,-1,-1);
	irr::core::vector3d<int> actCamPos(0,0,0);
	irr::core::vector3df tmp;

	// as long as the engine is active:
	while(irrDevice->run() && videoDriver)
	{
		if (irrDevice->isWindowActive())
		{
			// draw everything
			videoDriver->beginScene(true, true, irr::video::SColor(255,100,101,140));
			sceneManager->drawAll();
			guiEnvironment->drawAll();
			videoDriver->endScene();

			actFPS = videoDriver->getFPS();

			tmp = options->GetCamera()->getAbsolutePosition();
			actCamPos.X = static_cast<int>(tmp.X + 0.5f);
			actCamPos.Y = static_cast<int>(tmp.Y + 0.5f);
			actCamPos.Z = static_cast<int>(tmp.Z + 0.5f);

			if (actFPS != lastFPS || lastCamPos != actCamPos)
			{
				// update FPS and camera position
				// and show them as window name
				lastFPS = actFPS;
				lastCamPos = actCamPos;

				irr::core::stringw str = L"Leveleditor - FPS: ";
				str += actFPS;
				str += " - Position: (";
				str += actCamPos.X;
				str += ",";
				str += actCamPos.Y;
				str += ",";
				str += actCamPos.Z;
				str += ")";

				irrDevice->setWindowCaption(str.c_str());
			}
		}
		else
			irrDevice->yield();
	}

	// destroy the engine
	irrDevice->drop();
	delete options;
	delete gui;

	return 0;
}