// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <irrlicht.h>

class CMaterialCaveMultiColor;
class CMaterialCaveSingleColor;
class CMaterialCorridor;
class CMaterialDunGen;

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	class CMaterialProvider
	{
	public:
		/// constructor
		CMaterialProvider(irr::IrrlichtDevice* irrDevice_);

		/// destructor
		~CMaterialProvider();

		/// returns the material of the specified type
		irr::video::E_MATERIAL_TYPE GetMaterial(EMaterialType::Enum type_);

		/// set the cave color for the single color material
		void SetCaveColor(const irr::video::SColorf& color_);

		/// get the cave color for the single color material
		irr::video::SColorf GetCaveColor();

		/// set material
		void SetMaterialCustom(CMaterialDunGen* material_);

	private:
		CMaterialCaveMultiColor* MaterialCaveMultiColor;				///< the cave multicolor material
		CMaterialCaveSingleColor* MaterialCaveSingleColor;				///< the cave single color material
		CMaterialDunGen* MaterialCaveCustom;							///< custom cave material
	};

} // END NAMESPACE DunGen

#endif