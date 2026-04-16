// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef CREDITS_H
#define CREDITS_H

#include <irrlicht.h>
#include <string>

namespace Nightork
{

class CCredits
{
public:
	/// Constructor.
	CCredits(irr::IrrlichtDevice* irrDevice_);
	/// Destructor.
	~CCredits();

	/// Returns the credits text.
	std::wstring GetCredits() const;

private:
	std::wstring CreditsText;	///< The credits text.
};

}

#endif
