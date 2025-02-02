#pragma once
#include "heightmapTile.h"
#include <list>



// using namespace cv;

class heightmapIsland
{

protected:
	int maxX = 0, maxY = 0;

	int tileWidth, tileHeight;

	double westLong, eastLong;
	double northLat, southLat;

	std::list<heightmapTile> tiles;

public:
	heightmapIsland(std::list<heightmapTile> usedList);
	// heightmapIsland();

	void generateImage();

	static void generateTest();

	static uint8_t greenFromHeight(uint16_t heightValue);
	static uint8_t redFromHeight(uint16_t heightValue);

};