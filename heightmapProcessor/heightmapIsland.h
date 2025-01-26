#pragma once

#include <vector>
#include "heightmapTile.h"

class heightmapIsland
{

protected:
	std::vector<heightmapTile> linkedTiles;

public:
	heightmapIsland(heightmapTile);
	heightmapIsland(std::vector<heightmapTile> tileGroup);

	std::vector<heightmapTile> getLinkedTiles();
	void outputHeightmapIslandImage();

};

