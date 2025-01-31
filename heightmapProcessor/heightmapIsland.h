#pragma once
#include "heightmapTile.h"
#include <list>

class heightmapIsland
{

protected:
	int maxX = 0, maxY = 0;

	int tileWidth, tileHeight;

	double westLong, eastLong;
	double northLat, southLat;

	std::list<heightmapTile> insideTiles;

public:
	heightmapIsland(std::list<heightmapTile> usedList);
	heightmapIsland();


};