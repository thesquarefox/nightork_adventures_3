// Copyright (C) 2011-2014 by Maximilian Hönig

#ifndef LSYSTEMRANDOM_H
#define LSYSTEMRANDOM_H

#include <irrlicht.h>
#include <DunGen.h>

namespace Editor
{
	class CLSystemRandom
	{
	public:
		/// Constructor.
		CLSystemRandom(DunGen::CDunGen* dungeonGenerator_);

		/// Destructor.
		~CLSystemRandom();

		/// Apply random parameters to DunGen.
		void SetParametersRandom();

	private:
		/// Generates a random string based on given parameters.
		std::string GenerateRandomString(const std::string& allowedSymbols_, unsigned int minLength_, unsigned int maxLength_);

		/// Generates and adds a random rule.
		void AddRuleRandom(char symbol_, const std::string& allowedSymbols_, unsigned int minLength_, unsigned int maxLength_);

		/// Sets the specified parameter to a random value, based on parameters.
		void SetParameterRandom(DunGen::ELSystemParameter::Enum parameter_, double minValue_, double maxValue_, int digits_);

	public:
		unsigned int RandomCaveIteration;
		double RandomCaveAngleYawMin;
		double RandomCaveAngleYawMax;
		unsigned int RandomCaveAngleYawDigits;
		double RandomCaveAnglePitchMin;
		double RandomCaveAnglePitchMax;
		unsigned int RandomCaveAnglePitchDigits;
		double RandomCaveAngleRollMin;
		double RandomCaveAngleRollMax;
		unsigned int RandomCaveAngleRollDigits;
		double RandomCaveRadiusMin;
		double RandomCaveRadiusMax;
		unsigned int RandomCaveRadiusDigits;

		std::string RandomCaveStartAllowedSymbols;
		unsigned int RandomCaveStartMinSymbols;
		unsigned int RandomCaveStartMaxSymbols;
		std::string RandomCaveRuleAllowedSymbols;
		unsigned int RandomCaveRuleMinSymbols;
		unsigned int RandomCaveRuleMaxSymbols;

	private:
		DunGen::CDunGen* DungeonGenerator;
	};
};

#endif