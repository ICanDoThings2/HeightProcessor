#include "heightmapIsland.h"
#include <vector>
#include "heightmapTile.h"
#include <iostream>

heightmapIsland::heightmapIsland(std::vector<heightmapTile> tileGroup)
{

}

std::vector<heightmapIsland> heightmapIsland::islandsFromTiles(std::vector<heightmapTile> usedTiles)
{
	std::vector<std::vector<heightmapTile>> islands;
	std::vector<heightmapIsland> sorted;

	if (islands.size() == 0) // If it's nothing we'll go ahead and make our first island.
	{
		islands.emplace_back ( usedTiles.at(0).allConnected( std::vector<heightmapTile> {usedTiles.at(0) }) );
	}

	for (heightmapTile checkingTile : usedTiles )
	{

		for (std::vector<heightmapTile> thisIsland : islands)
		{
			/*if (heightmapTile::vectorContains(thisIsland, checkingTile)) // This checks current islands to see if it is in any. 
			{
				break; continue; // We found it among the islands so we can get back to checking other tiles.
			}*/
		}

		islands.emplace_back(checkingTile.allConnected(std::vector<heightmapTile> {checkingTile}));

	}

	std::cout << "\n there are "; std::cout << islands.size(); std::cout << " islands.\n";

	return sorted;
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

