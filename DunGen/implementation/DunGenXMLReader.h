// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#ifndef DUNGENXMLREADER_H
#define DUNGENXMLREADER_H

#include <vector>
#include <irrlicht.h>

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	class CDunGen;

	/// Class for reading and processing DunGen XML files.
	class CDunGenXMLReader
	{
	public:
		/// Constructor.
		CDunGenXMLReader(irr::IrrlichtDevice* irrDevice_, CDunGen* dunGen_);

		/// Destructor.
		~CDunGenXMLReader();

		/// process a DunGen XML file
		bool Read(const irr::io::path& filename_);

	private:
		/// process 'Materials' block
		void ReadMaterials();

		/// process 'RandomGenerator' block
		void ReadRandomGenerator();

		/// process 'VoxelCave' block
		void ReadVoxelCave();

		/// process 'Erode' block
		void ReadErode();

		/// process 'Filter' block
		void ReadFilter();

		/// process 'MeshCave' block
		void ReadMeshCave();

		/// process 'Gameplay' block
		void ReadGameplay();

		/// process 'Description' block
		void ReadDescription();

		/// read 'Portal Objects' block
		void ReadPortalObject();

		/// read 'Boss Objects' block
		void ReadBossObjects();

		/// read 'Objects' block
		void ReadObjects();

	private:
		/// reference to the DunGen interface
		CDunGen* DunGenInterface;

		/// reference to the the file system
		irr::io::IFileSystem* FileSystem;

		/// the actual file
		irr::io::IReadFile* File;

		/// the XML reader for the actual file
		irr::io::IXMLReader* XmlReader;
	};
}

#endif
