#include "heightmapIsland.h"
#include <list>
#include <stdint.h>
#include <iostream>
#include "heightmapTile.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"



heightmapIsland::heightmapIsland()
{

}

heightmapIsland::heightmapIsland(std::list<heightmapTile> usedList)
{
	std::list<heightmapIsland> pendingIslands;

	heightmapTile firstTile = usedList.front();
	std::list<heightmapTile> firstTileList = std::list<heightmapTile>{ firstTile };
	std::list<heightmapTile> firstChain = firstTile.allConnected( firstTileList, std::pair<int, int>(0, 0));

	heightmapIsland firstIsland = heightmapIsland();
	firstIsland.tiles = firstChain;
	pendingIslands.emplace_back( firstIsland );

	usedList = heightmapTile::trimmedList( usedList, firstIsland.tiles);

	while (usedList.size() > 0)
	{
		heightmapIsland newIsland = heightmapIsland();
		heightmapTile nextTile = usedList.front();
		std::list<heightmapTile> nextGroup = std::list<heightmapTile>{ nextTile };
		newIsland.tiles = nextTile.allConnected( nextGroup, {0,0} );
		pendingIslands.emplace_back( newIsland );
		usedList = heightmapTile::trimmedList(usedList, newIsland.tiles);
	}

	for (heightmapIsland island : pendingIslands)
	{		
		island.generateImage();
	}

	std::cout << "Islands generated\n";

}

int imageWidth( int tilesWide)
{
	return tilesWide * 1125;
}

int imageHeight( int tilesHeight )
{
	return tilesHeight * 750;
}

int pixelPos(std::pair<int, int> imagePos, std::pair<int,int> imageRes)
{
	return (imagePos.first * 3) + ( 3 * imagePos.second * imageRes.first );
}

int tilePixelStartOffset( std::pair<int,int> tilePos )
{
	return 0;
}

std::pair<int, int> adjustedTilePosition( std::pair<int, int> directPosition )
{
	return std::pair<int, int> {0, 0};
}

std::string heightmapIsland::imageName()
{
	std::string center;

	double centerLat, centerLong;

	centerLong = islandSpecs.east - islandSpecs.west;
	centerLat = islandSpecs.north - islandSpecs.south;

	centerLong += islandSpecs.west;
	centerLat += islandSpecs.east;

	center = std::to_string(centerLat); center.append("N_"); center.append(std::to_string(centerLong)); center.append("E");
	return center.append(".png");
}

void heightmapIsland::updateSpecs()
{
	int tileWidth, tileHeight;

	int minX = 0, maxX = 0;
	int minY = 0, maxY = 0;

	int tileX, tileY;

	if (tiles.size() == 0)
	{
		std::cout << "Critical failure! Island without tiles.";
		return;
	}

	heightmapTile firstTile = tiles.front();


	islandSpecs.west = firstTile.westLong();
	islandSpecs.east = firstTile.eastLong();
	islandSpecs.north = firstTile.northLat();
	islandSpecs.south = firstTile.southLat();

	for (heightmapTile tile : tiles) // Data calculations.
	{
		std::pair<int, int> thisPos = tile.getPosition();
		tileX = thisPos.first; tileY = thisPos.second;

		if (tileX < minX)
		{
			islandSpecs.west = tile.westLong();
			minX = tileX;
		}

		if (tileX > maxX)
		{
			islandSpecs.east = tile.eastLong();
			maxX = tileX;
		}

		if (tileY < minY)
		{
			islandSpecs.south = tile.southLat();
			minY = tileY;
		}

		if (tileY > maxY)
		{
			islandSpecs.north = tile.northLat();
			maxY = tileY;
		}
	}

	islandSpecs.lowest = { minX, minY };
	islandSpecs.highest = { maxX, maxY };
	islandSpecs.resolution = { (maxX - minX) + 1, (maxY - minY) + 1 };

}

void heightmapIsland::generateImage()
{
	updateSpecs();

	int width = islandSpecs.resolution.first, height = islandSpecs.resolution.second;
	width *= 1125; height *= 750;

	uint8_t* islandImage = new uint8_t[ width * height * 3];
	for (int pixel = 0; pixel < width * height * 3; pixel++)
	{
		islandImage[pixel] = 0; // Ensures it is otherwise blank.
	}

	std::pair<int, int> position;

	for (heightmapTile tile : tiles) // Much I hate to repeat ourselves we did need to figure out how big the pic needed to be before, so now we know
	{
		for (uint16_t point : tile.getRaw())
		{

		}
	}

	stbi_write_png( imageName().c_str(), width, height, 3, islandImage, width * 3);
}

uint8_t heightmapIsland::greenFromHeight(uint16_t heightValue)
{
	return ( (heightValue >> 8) & 0xFF );

}


uint8_t heightmapIsland::redFromHeight(uint16_t heightValue)
{
	return (heightValue & 0xFF);
}

void heightmapIsland::generateTest()
{
	int width = 128; int height = 128; int channels = 3;

	uint8_t* imgData = new uint8_t[width * height * channels];

	for (int n = 0; n < width * height * channels; n++)
	{
		imgData[n] = 0;
	}

	for (int x = 0; x < 127; x++)
	{
		int offset = x * 3;
		imgData[offset] = 100;
	}

	stbi_write_png("test.png", 128, 128, 3, imgData, 128 * 3);
}