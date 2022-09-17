#pragma once

#include <algorithm>
#include <cmath>

class heightmapTile
{

	
protected:

	/*
	* These two coordinate pairs are for the Southewest and Northest corners of a map, respectively.
	*/

	double northEastLat, northEastLong;
	double southWestLat, southWestLong;

	/*
	* These are neighboring tiles, non-diagonally.
	*/

	heightmapTile *westCell, *eastCell, *southCell, *northCell;

public:

	void setNECorner(double asLat, double asLong);

	/*
	* This sets the lower left, Southwest Corner of a tile.
	*/

	void setSWCorner(double asLat, double asLong)
	{

	}

	void setEastCell(heightmapTile* asEastCell);
	void setWestCell(heightmapTile* asWestCell);

	double longitudeLength();
	double latitudeLength();

	bool equalLongitude(heightmapTile* otherTile);
	bool equalLatitude(heightmapTile* otherTile);

	bool isWestOrEastBorder(heightmapTile* asNeighbor );
	bool isNorthOrSouthBorder(heightmapTile* asNeighbor);


};

