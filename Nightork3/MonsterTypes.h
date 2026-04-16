// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef MONSTERTYPES_H
#define MONSTERTYPES_H

namespace Nightork
{

/// The type of the object
struct EMonsterType
{
	enum Enum
	{
		SHIP_RED = 0,
		SHIP_GREEN,
		SHIP_BLUE,
		SHIP_YELLOW,
		SHIP_TEAL,
		SHIP_PURPLE,
		BOSS_01,
		BOSS_02,
		BOSS_03,
		BOSS_04,
		BOSS_05,
		BOSS_06,
		NUMBER_OF_MONSTERTYPES
	};
};

}

#endif
