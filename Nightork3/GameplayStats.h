// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef GAMEPLAYSTATS_H
#define GAMEPLAYSTATS_H

#include <time.h>

namespace Nightork
{

class CGameplayStats
{
public:
	/// Constructor.
	CGameplayStats();

	/// Destructor.
	~CGameplayStats();

	/// Count one death.
	void CountDeath();

	/// Returns death count.
	unsigned int GetDeathCount();

	/// Returns played time
	unsigned int GetPlayTime(clock_t actTime_);

private:
	unsigned int DeathCount;	///< The current death count.
	clock_t GameStartTime;		///< The time where the game started.
};

}

#endif
