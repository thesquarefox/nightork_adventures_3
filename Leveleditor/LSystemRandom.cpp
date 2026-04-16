// Copyright (C) 2011-2014 by Maximilian Hönig

#include "LSystemRandom.h"
#include <time.h>

Editor::CLSystemRandom::CLSystemRandom(DunGen::CDunGen* dungeonGenerator_)
	: DungeonGenerator(dungeonGenerator_)
	, RandomCaveIteration(5)
	, RandomCaveAngleYawMin(0.0)
	, RandomCaveAngleYawMax(360.0)
	, RandomCaveAngleYawDigits(0)
	, RandomCaveAnglePitchMin(0.0)
	, RandomCaveAnglePitchMax(360.0)
	, RandomCaveAnglePitchDigits(0)
	, RandomCaveAngleRollMin(0.0)
	, RandomCaveAngleRollMax(360.0)
	, RandomCaveAngleRollDigits(0)
	, RandomCaveRadiusMin(10)
	, RandomCaveRadiusMax(30)
	, RandomCaveRadiusDigits(0)
	, RandomCaveStartAllowedSymbols("FXY")
	, RandomCaveStartMinSymbols(5)
	, RandomCaveStartMaxSymbols(10)
	, RandomCaveRuleAllowedSymbols("FFFFXY[]+-ouzg|$")
	, RandomCaveRuleMinSymbols(5)
	, RandomCaveRuleMaxSymbols(10)
{
}

Editor::CLSystemRandom::~CLSystemRandom()
{
}

std::string Editor::CLSystemRandom::GenerateRandomString(const std::string& allowedSymbols_, unsigned int minLength_, unsigned int maxLength_)
{
	std::string randomString = "";
	unsigned int length = rand()%(maxLength_-minLength_)+minLength_;

	// roll first symbol
	unsigned int index;
	index = rand()%(allowedSymbols_.length());
	randomString += allowedSymbols_[index];

	// catch senseless data input
	bool onlySymb1 = true;
	bool onlySymb2 = true;

	for (unsigned int i = 0; i<allowedSymbols_.size(); ++i)
	{
		if (allowedSymbols_.at(i) != '|')
			onlySymb1 = false;
		if (allowedSymbols_.at(i) != '$')
			onlySymb2 = false;
	}

	if (onlySymb1 || onlySymb2) 
		return randomString;

	// roll remaining symbols
	for (unsigned int i = 1; i<length; i++)
	{
		// prevent senseless symbol combination
		index = rand()%(allowedSymbols_.length());
		while ((randomString.at(i-1)=='[' && allowedSymbols_.at(index) ==']')
			|| (randomString.at(i-1)=='+' && allowedSymbols_.at(index) =='-')
			|| (randomString.at(i-1)=='-' && allowedSymbols_.at(index) =='+')
			|| (randomString.at(i-1)=='u' && allowedSymbols_.at(index) =='o')
			|| (randomString.at(i-1)=='o' && allowedSymbols_.at(index) =='u')
			|| (randomString.at(i-1)=='g' && allowedSymbols_.at(index) =='z')
			|| (randomString.at(i-1)=='z' && allowedSymbols_.at(index) =='g')
			|| (randomString.at(i-1)=='|' && allowedSymbols_.at(index) =='|')
			|| (randomString.at(i-1)=='$' && allowedSymbols_.at(index) =='$'))
			index = rand()%(allowedSymbols_.length());

		// add rolled symbol
		randomString += allowedSymbols_[index];
	}

	return randomString;
}

void Editor::CLSystemRandom::SetParametersRandom()
{
	srand(static_cast<unsigned int>(time(NULL)));
	DungeonGenerator->LSystemDeleteRules();
	DungeonGenerator->LSystemSetIteration(RandomCaveIteration);
	DungeonGenerator->LSystemSetStart(GenerateRandomString(RandomCaveStartAllowedSymbols,RandomCaveRuleMinSymbols,RandomCaveRuleMaxSymbols));
	SetParameterRandom(DunGen::ELSystemParameter::ANGLE_YAW, RandomCaveAngleYawMin, RandomCaveAngleYawMax, RandomCaveAngleYawDigits);
	SetParameterRandom(DunGen::ELSystemParameter::ANGLE_PITCH, RandomCaveAnglePitchMin, RandomCaveAnglePitchMax, RandomCaveAnglePitchDigits);
	SetParameterRandom(DunGen::ELSystemParameter::ANGLE_ROLL, RandomCaveAngleRollMin, RandomCaveAngleRollMax, RandomCaveAngleRollDigits);
	SetParameterRandom(DunGen::ELSystemParameter::RADIUS_START, RandomCaveRadiusMin, RandomCaveRadiusMax, RandomCaveRadiusDigits);

	// fetch replacement candidates
	std::string replacementCandidates = RandomCaveStartAllowedSymbols + RandomCaveRuleAllowedSymbols;
	std::string symbolsToReplace = "F";
	char curChar;
	bool alreadyThere;
	for (unsigned i = 0; i<replacementCandidates.size(); ++i)
	{
		curChar = replacementCandidates.at(i);

		if (curChar != '[' && curChar != ']' && curChar != '!'
			&& curChar != '+' && curChar != '-'
			&& curChar != 'u' && curChar != 'o' 
			&& curChar != 'g' && curChar != 'z' 
			&& curChar != '|' && curChar != '$')
		{
			alreadyThere = false;
			for (unsigned j = 0; j<symbolsToReplace.size(); ++j)
				if (curChar == symbolsToReplace.at(j))
					alreadyThere = true;

			if (!alreadyThere)
				symbolsToReplace.append(1,curChar);
		}
	}

	// add rules
	for (unsigned int i = 0; i < symbolsToReplace.length(); ++i)
		AddRuleRandom(symbolsToReplace[i], RandomCaveRuleAllowedSymbols, RandomCaveRuleMinSymbols, RandomCaveRuleMaxSymbols);
}

void Editor::CLSystemRandom::SetParameterRandom(DunGen::ELSystemParameter::Enum parameter_, double minValue_, double maxValue_, int digits_)
{
	// shift comma to high
	double conversionFactor = pow(10.0,digits_);
	int minValue =  static_cast<int>(minValue_ * conversionFactor);
	if (minValue_ * conversionFactor > static_cast<double>(minValue))
		++minValue;
	int maxValue = static_cast<int>(maxValue_ * conversionFactor);

	// roll result
	int tmp = maxValue;
	if (maxValue>minValue)
		tmp = (rand()%(maxValue-minValue))+minValue; 

	// shift comma back to low
	double result = static_cast<double>(tmp)/conversionFactor;

	// set parameter
	DungeonGenerator->LSystemSetParameter(parameter_, result);
}

void Editor::CLSystemRandom::AddRuleRandom(char symbol_, const std::string& allowedSymbols_, unsigned int minLength_, unsigned int maxLength_)
{
	// compute and add rule
	DunGen::SSubstitutionRule newRule;
	newRule.Symbol = symbol_;
	newRule.Substitution = GenerateRandomString(allowedSymbols_,minLength_,maxLength_);
	DungeonGenerator->LSystemAddRule(newRule);
}
