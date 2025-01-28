#pragma once

#include <vector>
#include "heightmapTile.h"

class heightmapIsland
{

protected:
	std::vector<heightmapTile> linkedTiles;

public:

	heightmapIsland(std::vector<heightmapTile> tileGroup);

	static std::vector<heightmapIsland> islandsFromTiles( std::vector<heightmapTile> );

	std::vector<heightmapTile> getLinkedTiles();
	void outputHeightmapIslandImage();

};

