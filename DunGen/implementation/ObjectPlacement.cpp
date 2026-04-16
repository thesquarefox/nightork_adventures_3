// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#include "ObjectPlacement.h"
#include <iostream>
#include <xutility>

DunGen::CObjectPlacement::CObjectPlacement()
{
	DeleteStaticObjects();
}

DunGen::CObjectPlacement::~CObjectPlacement()
{
}

void DunGen::CObjectPlacement::DeleteObjects()
{
	PlacedObjects.clear();
}

void DunGen::CObjectPlacement::DeleteStaticObjects()
{
	for (unsigned int i=0; i<EBossObject::NUMBER_OF_OBJECTS; ++i)
	{
		BossObjects[i].Position = irr::core::vector3d<double>(0.0,0.0,0.0);
		BossObjects[i].IsPlaced = false;
	}

	PortalObject.Position = irr::core::vector3d<double>(0.0,0.0,0.0);
	PortalObject.Rotation = irr::core::vector3d<double>(0.0,0.0,0.0);
	PortalObject.IsPlaced = false;
}

std::vector<DunGen::SPlacedObject> DunGen::CObjectPlacement::GetPlacedObjects() const
{
	return PlacedObjects;
}

void DunGen::CObjectPlacement::SetBossObject(EBossObject::Enum type_, const SBossObject& object_)
{
	BossObjects[type_] = object_;
}

DunGen::SBossObject DunGen::CObjectPlacement::GetBossObject(EBossObject::Enum type_) const
{
	return BossObjects[type_];
}

void DunGen::CObjectPlacement::SetPortalObject(const SPortalObject& object_)
{
	PortalObject = object_;
}

DunGen::SPortalObject DunGen::CObjectPlacement::GetPortalObject() const
{
	return PortalObject;
}

void DunGen::CObjectPlacement::AddObject(const SPlacedObject& object_)
{
	PlacedObjects.push_back(object_);
}
