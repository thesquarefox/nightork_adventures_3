// Copyright (C) 2011-2026 by Maximilian Hönig

#include "GameplayStats.h"

Nightork::CGameplayStats::CGameplayStats()
	: DeathCount(0)
	, GameStartTime(0)
{
}

Nightork::CGameplayStats::~CGameplayStats()
{
}

void Nightork::CGameplayStats::CountDeath()
{
	++DeathCount;
}

unsigned int Nightork::CGameplayStats::GetDeathCount()
{
	return DeathCount;
}

unsigned int Nightork::CGameplayStats::GetPlayTime(clock_t actTime_)
{
	return actTime_ - GameStartTime;
}
