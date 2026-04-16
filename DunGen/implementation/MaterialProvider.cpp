// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#include "interface/MaterialDunGen.h"
#include "MaterialCaveMultiColor.h"
#include "MaterialCaveSingleColor.h"
#include "MaterialProvider.h"

DunGen::CMaterialProvider::CMaterialProvider(irr::IrrlichtDevice* irrDevice_)
: MaterialCaveMultiColor(new CMaterialCaveMultiColor(irrDevice_))
, MaterialCaveSingleColor(new CMaterialCaveSingleColor(irrDevice_))
, MaterialCaveCustom(NULL)
{
}

DunGen::CMaterialProvider::~CMaterialProvider()
{
	delete MaterialCaveSingleColor;
	delete MaterialCaveMultiColor;
}

irr::video::E_MATERIAL_TYPE DunGen::CMaterialProvider::GetMaterial(EMaterialType::Enum type_)
{
	switch (type_)
	{
	case EMaterialType::CAVE_SINGLECOLOR:
		return MaterialCaveSingleColor->GetMaterial();
	case EMaterialType::CAVE_CUSTOM:
		return MaterialCaveCustom->GetMaterial();
	default:
		return MaterialCaveMultiColor->GetMaterial();
	}
}

void DunGen::CMaterialProvider::SetCaveColor(const irr::video::SColorf& color_)
{
	MaterialCaveSingleColor->SetColor(color_);
}

void DunGen::CMaterialProvider::SetMaterialCustom(CMaterialDunGen* material_)
{
	MaterialCaveCustom = material_;
}

irr::video::SColorf DunGen::CMaterialProvider::GetCaveColor()
{
	return MaterialCaveSingleColor->GetColor();
}
