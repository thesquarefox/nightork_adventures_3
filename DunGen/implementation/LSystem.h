// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#ifndef LSYSTEM_H
#define LSYSTEM_H

#include "interface/LSystemCommon.h"
#include <string>
#include <vector>

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	/// generator for Lindenmayer systems (L-systems)
	class CLSystem
	{
public:
		/// constructor
		CLSystem();
		/// destructor
		~CLSystem();

		/// generate L-system derivations
		void GenerateDerivations();
		
		/// get the highest created derivation
		std::string GetHighestDerivation();

		/// sets the iteration
		void SetIteration(unsigned int iteration_);

		/// gets the iteration
		unsigned int GetIteration() const;

		/// set parameter
		void SetParameter(ELSystemParameter::Enum parameter_, double value_);

		/// get parameter
		double GetParameter(ELSystemParameter::Enum parameter_);

		/// set start string (axiom)
		void SetStart(const std::string& start_);

		/// set start string (axiom)
		std::string GetStart();

		/// add or actualize a substitution rule
		void AddRule(const SSubstitutionRule& rule_);
		
		/// delete all substitution rules
		void DeleteRules();

		/// gets the substitution rules
		std::vector<SSubstitutionRule> GetRules();

	private:
		/// replace by substitution rules
		inline std::string Replace(const std::string& baseString_);

	private:
		/// maximum string size for generated derivations
		static const unsigned int MAX_SIZE = 1000000;

		// parameters:

		/// derivation (0 = start)
		std::vector<std::string> Derivations;
		/// substitution rules
		std::vector<SSubstitutionRule> SubstitutionRules;
		/// the yaw angle
		double AngleYaw;
		/// the pitch angle
		double AnglePitch;
		/// the roll angle
		double AngleRoll;
		/// the start radius
		double RadiusStart;
		/// the radius factor
		double RadiusFactor;
		/// the radius decrement
		double RadiusDecrement;
		/// the maximum iteration
		unsigned int Iteration;
	};

} // END NAMESPACE DunGen

#endif