// Copyright (C) 2011-2026 by Maximilian Hönig

#include "Helper.h"

irr::core::stringw Nightork::GetTime(unsigned int ms_, bool showMs_)
{
	const unsigned int seconds = (ms_ / 1000) % 60;
	const unsigned int minutes = (ms_ / 60000) % 60;
	const unsigned int hours = ms_ / 3600000;

	irr::core::stringw result = irr::core::stringw(hours) + irr::core::stringw(":");
	if (10 > minutes)
		result += irr::core::stringw("0");
	result += irr::core::stringw(minutes) + irr::core::stringw(":");
	if (10 > seconds)
		result += irr::core::stringw("0");
	result += irr::core::stringw(seconds);

	if (showMs_)
	{
		const unsigned int ms = ms_ % 1000;
		result += irr::core::stringw("-") + irr::core::stringw(ms);
	}
	return result;
}
