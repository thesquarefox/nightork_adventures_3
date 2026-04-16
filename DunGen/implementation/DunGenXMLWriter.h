// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#ifndef DUNGENXMLWRITER_H
#define DUNGENXMLWRITER_H

#include <vector>
#include <irrlicht.h>

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	class CDunGen;

	/// Class for writing DunGen XML files.
	class CDunGenXMLWriter
	{
	public:
		/// Constructor.
		CDunGenXMLWriter(irr::IrrlichtDevice* irrDevice_, CDunGen* dunGen_);

		/// Destructor.
		~CDunGenXMLWriter();

		/// write a DunGen XML file
		bool Write(const irr::io::path& filename_);

	private:
		/// write 'Materials' block
		void WriteMaterials();

		/// write 'RandomGenerator' block
		void WriteRandomGenerator();

		/// write 'VoxelCave' block
		void WriteVoxelCave();

		/// write 'Erode' block
		void WriteErode();

		/// write 'Filter' block
		void WriteFilter();

		/// write 'MeshCave' block
		void WriteMeshCave();

		/// write 'Gameplay' block
		void WriteGameplay();

		/// write 'Description' block
		void WriteDescription();

		/// write 'Portal Objects' block
		void WritePortalObject();

		/// write 'Boss Objects' block
		void WriteBossObjects();

		/// write 'Objects' block
		void WriteObjects();

	private:
		/// reference to the DunGen interface
		CDunGen* DunGenInterface;

		/// reference to the the file system
		irr::io::IFileSystem* FileSystem;

		/// the actual file
		irr::io::IWriteFile* File;

		/// the XML reader for the actual file
		irr::io::IXMLWriter* XmlWriter;
	};
}

#endif
