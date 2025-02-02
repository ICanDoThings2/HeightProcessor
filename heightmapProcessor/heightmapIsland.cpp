#include "heightmapIsland.h"
#include <list>
#include <stdint.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"



heightmapIsland::heightmapIsland(std::list<heightmapTile> usedList)
{

	std::list<std::list<heightmapIsland>> newIslands;

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
	int totalPixels = 3 * imageRes.first * imageRes.second;


	int position = (imagePos.first * 3) + ( 3 * imagePos.second * (imageRes.second * imageRes.first * 3));

	return position;

}

void heightmapIsland::generateImage()
{
	int tileWidth, tileHeight;

	int minX = 0, maxX = 0;
	int minY = 0, maxY = 0;

	int tileX, tileY;
	for (heightmapTile tile : tiles) // Data calculations.
	{
		std::pair<int, int> thisPos = tile.getPosition();
		tileX = thisPos.first; tileY = thisPos.second;

		if (tileX < minX)
		{
			minX = tileX;
		}

		if (tileX > maxX)
		{
			maxX = tileX;
		}

		if (tileY < minY)
		{
			minY = tileY;
		}

		if (tileY > maxY)
		{
			maxY = tileY;
		}
	}

	int width, height;

	width = (maxX - minX) * 1125;
	height = (maxY - minY) * 750;

	uint8_t* islandImage = new uint8_t[width * height * 3];
	for (int pixel = 0; pixel < width * height * 3; pixel++)
	{
		islandImage[pixel] = 0; // Ensures there is no data.
	}

	/* As much I hate to repeat myself it was needed to figure out the island resolution, now we know
	we can actually go about setting the elevation points.*/

	for (heightmapTile tile : tiles) // Much I hate to repeat ourselves we did need to figure out how big the pic needed to be before, so now we know
	{
		int read = 0;
		int row = 0;

		for (uint16_t point : tile.getRaw())
		{

			read++;
			if (read == 1125)
			{
				read = 0;
				row++;
			}
		}
	}
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