// Copyright (C) 2011-2026 by Maximilian Hönig

#include <iostream>
#include "Credits.h"

Nightork::CCredits::CCredits(irr::IrrlichtDevice* irrDevice_)
	: CreditsText()
{
	std::cout << "[Credits:] Reading credits";

	// get read access to file
	irr::io::IReadFile* file = irrDevice_->getFileSystem()->createAndOpenFile("data/Credits");

	if (!file)
	{
		std::cout << " failed." << std::endl;
		return;
	}

	irr::io::IXMLReader* xmlReader = irrDevice_->getFileSystem()->createXMLReader(file);

	// process file content
	while(xmlReader->read())
	{
		if (xmlReader->getNodeType() == irr::io::EXN_ELEMENT)
		{
			if (irr::core::stringw("Text") == xmlReader->getNodeName())
			{
				CreditsText = xmlReader->getAttributeValue(L"value");
			}
		}
	}

	// close file
	xmlReader->drop();
	file->drop();

	std::cout << " succeeded." << std::endl;
}

Nightork::CCredits::~CCredits()
{
}

std::wstring Nightork::CCredits::GetCredits() const
{
	return CreditsText;
}
