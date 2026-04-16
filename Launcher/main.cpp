// Copyright (C) 2011-2014 by Maximilian Hönig

#include <iostream>
#include <irrlicht.h>
#include <string>
#include <Windows.h>
#include "EventReceiver.h"
#include "Gui.h"
#include "Options.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#ifdef _IRR_WINDOWS_
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

namespace
{
	void Execute(std::wstring NameOfExe)
	{ 
		// process API
		STARTUPINFOW siStartupInfo; 
		PROCESS_INFORMATION piProcessInfo; 
		memset(&siStartupInfo, 0, sizeof(siStartupInfo)); 
		memset(&piProcessInfo, 0, sizeof(piProcessInfo)); 
		siStartupInfo.cb = sizeof(siStartupInfo); 

		CreateProcessW(const_cast<LPCWSTR>(NameOfExe.c_str()), 
			0, 0, 0, false, CREATE_DEFAULT_ERROR_MODE, 0, 0, 
			&siStartupInfo, &piProcessInfo);
		
		// release handles
		CloseHandle(piProcessInfo.hProcess); 
		CloseHandle(piProcessInfo.hThread); 
	}
}

int main(int argc, char* argv[])
{
	std::cout << "Launcher is started." << std::endl;

	// create Irrlicht device
	CEventReceiver eventReceiver;
	irr::SIrrlichtCreationParameters irrlichtParameter;
	irrlichtParameter.DriverType = irr::video::EDT_OPENGL;
	irrlichtParameter.WindowSize = irr::core::dimension2du(1024, 768);
	irrlichtParameter.Bits = 32;
	irrlichtParameter.Fullscreen = false;
	irrlichtParameter.Stencilbuffer = false;
	irrlichtParameter.Vsync = false;
	irrlichtParameter.AntiAlias = false;
	irrlichtParameter.EventReceiver = &eventReceiver;

	irr::IrrlichtDevice* irrDevice = irr::createDeviceEx(irrlichtParameter);
	if(!irrDevice)
	{
		std::cout << "Could not create OpenGL Irrlicht device. Switching to DirectX 9." << std::endl;
		irrlichtParameter.DriverType = irr::video::EDT_DIRECT3D9;
		irrDevice = irr::createDeviceEx(irrlichtParameter);
	}
	if(!irrDevice)
	{
		std::cout << "Irrlicht device could not be created, program is terminated." << std::endl;
		return 1;
	}
	irr::scene::ISceneManager* sceneManager = irrDevice->getSceneManager();
	irr::video::IVideoDriver* videoDriver = irrDevice->getVideoDriver();
	irr::gui::IGUIEnvironment* guiEnvironment = irrDevice->getGUIEnvironment();

	Launcher::CGUI* gui = new Launcher::CGUI(guiEnvironment, videoDriver);
	Launcher::COptions* options = new Launcher::COptions(irrDevice);
	eventReceiver.Intialize(options, gui, guiEnvironment);

	irrDevice->setWindowCaption(L"Nightork 3 Launcher");

	// as long as the engine is active:
	while(irrDevice->run() && videoDriver && eventReceiver.GetExeNameToStart().empty())
	{
		if (irrDevice->isWindowActive())
		{
			// draw everything
			videoDriver->beginScene(true, true, irr::video::SColor(255,200,215,200));
			sceneManager->drawAll();
			guiEnvironment->drawAll();
			videoDriver->endScene();
		}
		else
			irrDevice->yield();
	}

	// store name of exe to be launched
	std::wstring exeToStart = eventReceiver.GetExeNameToStart();

	// destroy the engine
	delete options;
	delete gui;
	irrDevice->drop();

	// launch exe
	if (!eventReceiver.GetExeNameToStart().empty())
	{
		std::wcout << L"Launcher starts " << exeToStart << std::endl;
		Execute(exeToStart);
	}

	return 0;
}