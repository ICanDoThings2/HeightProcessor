#include "heightmapIsland.h"
#include <vector>
#include "heightmapTile.h"

heightmapIsland::heightmapIsland(heightmapTile fromTile)
{
	linkedTiles.emplace_back(fromTile);
}



void heightmapIsland::outputHeightmapIslandImage()
{
	double westmostLong, eastmostLong;
	double southmostLat, northmostLat;

	int xRes, yRes;

	westmostLong = linkedTiles.at(0).westLong();
	eastmostLong = linkedTiles.at(0).eastLong();

	northmostLat = linkedTiles.at(0).northLat();
	southmostLat = linkedTiles.at(0).southLat();

	for (heightmapTile hTile : linkedTiles)
	{
		if (hTile.westLong() < westmostLong)
		{
			westmostLong = hTile.westLong();
		}

		if (hTile.eastLong() > eastmostLong)
		{
			eastmostLong = hTile.eastLong();
		}

		if (hTile.southLat() < southmostLat)
		{
			southmostLat = hTile.southLat();
		}

		if (hTile.northLat() > northmostLat)
		{
			northmostLat = hTile.northLat();
		}
	}

	double longCenter, latCenter;
	longCenter = westmostLong + ((eastmostLong - westmostLong) / 2);
	latCenter = southmostLat + ((northmostLat - southmostLat) / 2);

	std::string fileName = std::to_string(latCenter) + "N " + std::to_string(longCenter) + "E";

}

