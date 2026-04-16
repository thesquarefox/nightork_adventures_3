// Copyright (C) 2011-2026 by Maximilian Hönig

#include <iostream>
#include <irrlicht.h>
#include <irrKlang.h>
#include <fstream>
#include <steam_api.h>
#include "EventReceiver.h"
#include "GameStates.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrKlang.lib")
#pragma comment(lib, "DunGen.lib")
#pragma comment(lib, "steam_api64.lib")
#endif

#ifdef _IRR_WINDOWS_
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main(int argc, char* argv[])
{
#if 0
	std::ofstream logfile("Nightork.log");
	std::cout.rdbuf(logfile.rdbuf());
#endif

	bool steamAvailable = SteamAPI_Init();
	std::cout << "Steam status: " << steamAvailable << std::endl;

	unsigned int resolution_x = 1024;
	unsigned int resolution_y = 768;
	unsigned int antialiasing = 0;
	bool fullscreen = false;
	bool vsync = false;
	bool directX = true;

	std::ifstream infile("Nightork.cfg", std::ifstream::in & std::ifstream::binary);
	if (infile)
	{
		infile >> resolution_x;
		infile >> resolution_y;
		infile >> fullscreen;
		infile >> vsync;
		infile >> antialiasing;
		infile >> directX;
		infile.close();
	}

	std::cout << "Nightork Adventures is started." << std::endl;
	std::cout << "Resolution is: " << resolution_x << " * " << resolution_y << " , FullScreen: " << fullscreen
		<< " , AntiAliasing: " <<  antialiasing << " , VSync: " << vsync << " , " << (directX ? "DirectX9" : "OpenGL") << std::endl;

	// create Irrlicht device
	Nightork::CEventReceiver eventReceiver;
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

	irrklang::ISoundEngine* irrKlangDevice = irrklang::createIrrKlangDevice();
	if (!irrKlangDevice)
	{
		std::cout << "IrrKlang device could not be created, program is terminated." << std::endl;
		return 1;
	}

	irrDevice->setWindowCaption(L"Nightork Adventures");

	Nightork::CGameStates* gameStates = new Nightork::CGameStates(irrDevice,irrKlangDevice,&eventReceiver);
	eventReceiver.Initialize(irrDevice, gameStates);

	int actFPS = 0;
	int lastFPS = 0;
	irr::core::vector3df tmp;
	irr::core::vector3d<int> actCamPos(0,0,0);
	irr::core::vector3d<int> lastCamPos(0,0,0);

	// preload textures to prevent loading delays
	videoDriver->getTexture("data/particle_white.png");
	videoDriver->getTexture("data/projectile_green.png");
	videoDriver->getTexture("data/projectile_orange.png");
	videoDriver->getTexture("data/Crosshair_white.png");
	videoDriver->getTexture("data/Crosshair_blue.png");
	videoDriver->getTexture("data/Aiminghair.png");
	videoDriver->getTexture("data/Highlighting.png");

	// as long as the engine is active:
	while(irrDevice->run() && videoDriver && !eventReceiver.GetEndProgram())
	{
		if (irrDevice->isWindowActive())
		{
			gameStates->Update();

			actFPS = videoDriver->getFPS();

			irr::scene::ICameraSceneNode* cam = sceneManager->getActiveCamera();
			tmp = irr::core::vector3df(0.0f,0.0f,0.0f);
			if (cam)
				tmp = cam->getAbsolutePosition();

			actCamPos.X = static_cast<int>(tmp.X + 0.5f);
			actCamPos.Y = static_cast<int>(tmp.Y + 0.5f);
			actCamPos.Z = static_cast<int>(tmp.Z + 0.5f);

			if (actFPS != lastFPS || lastCamPos != actCamPos)
			{
				// update FPS and camera position
				// and show them as window name
				lastFPS = actFPS;
				lastCamPos = actCamPos;

				irr::core::stringw str = L"Nightork Adventures 3 - FPS: ";
				str += actFPS;
				/*str += " - Position: (";
				str += actCamPos.X;
				str += ",";
				str += actCamPos.Y;
				str += ",";
				str += actCamPos.Z;
				str += ")";
				*/

				irrDevice->setWindowCaption(str.c_str());
			}
		}
		else
			irrDevice->yield();
	}

	// delete objects
	irrDevice->drop();
	delete gameStates;
	irrKlangDevice->drop();

	return 0;
}