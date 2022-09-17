#include "heightmapTile.h"

/*
* This sets the upper right, North East corner of a tile.
*/

void heightmapTile::setNECorner(double asLat, double asLong)
{
	northEastLat = std::clamp(asLat, -90.0, 90.0); // Latitude only goes to 90 degrees. Negative is meant to be South.
	northEastLong = std::clamp(asLong, -180.0, 180.0); // Longitude is 180 either way, negative is west.
}

double heightmapTile::longitudeLength()
{
	return std::abs(northEastLong - southWestLong);
}

double heightmapTile::latitudeLength()
{
	return std::abs(northEastLat - southWestLat);
}

/*
* This returns true/false for if two heightmap tiles have equal latitude
*/

bool heightmapTile::equalLatitude(heightmapTile* altTile)
{
	return (this->latitudeLength() == altTile->latitudeLength());
}

/*
* This returns true/false if two heightmap tiles have equal longitude.
*/

bool heightmapTile::equalLongitude(heightmapTile* altTile)
{
	return ( this->longitudeLength() == altTile->longitudeLength() );
}

/*
* This function checks if a heightmapTile is adjacent to another, by latitude.
* If they have the same latitudes for both corners, then they would be the same distance.
*/

bool heightmapTile::isWestOrEastBorder( heightmapTile* asNeighbor )
{
	return ( this->northEastLat == asNeighbor->northEastLat &&
		this->southWestLat == asNeighbor->southWestLat &&
		this->southWestLong == asNeighbor->northEastLong ||
		this->northEastLong == asNeighbor->southWestLong);
}

/*
* This function checks if a heightmapTile is adjacent to another, by longitude.
* 
*/

bool heightmapTile::isNorthOrSouthBorder(heightmapTile* asNeighbor)
{
	return ( this->northEastLong == asNeighbor->northEastLong );
}

void heightmapTile::setEastCell(heightmapTile* asEastCell)
{
	if (asEastCell != nullptr) // Make sure it's real.
	{
		if ( this->isWestOrEastBorder( asEastCell ) ) // Make sure their borders are equal.
		{
			this->eastCell = asEastCell;
			asEastCell->westCell = this; // Set us to be their neighbor too.
		}

		return; 
	}
}

void heightmapTile::setWestCell(heightmapTile* asWestCell)
{
	if (asWestCell != nullptr) // Make sure it's real.
	{
		westCell = asWestCell;
	}
}