// Copyright (C) 2011-2014 by Maximilian Hönig

#ifndef OPTIONS_H
#define OPTIONS_H

#include <irrlicht.h>
#include <DunGen.h>

namespace Editor
{
	class CLSystemRandom;
	class CObjectPlacement;

	class COptions
	{
	public:
		/// Constructor.
		COptions(irr::IrrlichtDevice* irrDevice_);

		/// Destructor.
		~COptions();

		/// Gets the first person camera.
		irr::scene::ICameraSceneNode* GetCamera();

		/// Creates the cave.
		void AssembleCave();

		/// Decreases iteration of cave.
		void IterationMinus();

		/// Increases iteration of cave.
		void IterationPlus();

		/// Apply random parameters to DunGen.
		void SetParametersRandom();

		/// Add a rule.
		void AddRule();

		// Deletes all rules.
		void DeleteRules();

		/// Places random objects in the cave.
		void PlaceObjects();

		/// places portal or boss monster
		void PlaceObject(unsigned int i);

		/// delete portal or boss monster
		void DeleteObject(unsigned int i);

		/// applies the current portal rotation
		void PortalRotationApply();

		/// Take a screenshot.
		void TakeScreenshot();

		/// Save dungeon to file.
		void SaveFile(const irr::io::path& filename_);

		/// Load dungeon from file.
		void LoadFile(const irr::io::path& filename_);

		/// Store information of all windows.
		void WindowsStore();

		/// Load information of all windows.
		void WindowsApply();

	private:
		// Adds the cave to the scene.
		void CreateScene();

		/// Store information of random cave window.
		void WindowStore_RandomCave();

		/// Store information of cave window.
		void WindowStore_Cave();

		/// Store information of object placement window.
		void WindowStore_ObjectPlacement();

		/// Store information of random objects window.
		void WindowStore_RandomObjects();

		/// Store information of gameplay window.
		void WindowStore_Gameplay();

		/// Store information of description window.
		void WindowStore_Description();

		/// Load information to random cave window.
		void WindowApply_RandomCave();

		/// Load information to cave window.
		void WindowApply_Cave();

		/// Load information to object placement window.
		void WindowApply_ObjectPlacement();

		/// Load information to random objects window.
		void WindowApply_RandomObjects();

		/// Load information of gameplay window.
		void WindowApply_Gameplay();

		/// Load information of description window.
		void WindowApply_Description();

	private:
		DunGen::CDunGen* DungeonGenerator;
		irr::IrrlichtDevice* IrrDevice;
		irr::scene::ICameraSceneNode* Camera;
		irr::gui::IGUIEnvironment* GUIEnvironment;
		CLSystemRandom* LSystemRandom;
		CObjectPlacement* ObjectPlacement;
		const irr::io::path WorkingDirectory;
	};
};

#endif