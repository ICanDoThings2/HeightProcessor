#include "heightmapTile.h"
#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <cstdint>
#include <algorithm>
#include <cctype>


heightmapTile::heightmapTile( pugi::xml_document *fromDoc)
{
	parseAllNodes(fromDoc);
}

double longitudeFromString(std::string ofString)
{
	if (!ofString.empty())
	{
		return std::stod(ofString.substr(ofString.find(" "), ofString.length()));
	}

	return 0;
}

double latitudeFromString( std::string ofString )
{
	
	if (!ofString.empty())
	{
		return std::stod( ofString.substr(0, ofString.find(" ") ) );
	}


	return 0;
}

uint16_t cleaned_char(std::string fromStr)
{
	std::stoul(fromStr.substr(fromStr.find(","), fromStr.length() ) );

	return 0;
}

void loadTuples(pugi::xml_node fromNode)
{
	for (pugi::xml_node tPoint : fromNode.children())
	{

		std::cout << tPoint.value();
	}
}

void heightmapTile::parseNodes(pugi::xml_node thisNode)
{
	int nameInd = 0;


	for (std::string thisName : usedNames)
	{
		if (thisNode.name() == thisName)
		{
			/*std::cout << "Found " + thisName + "\n";
			std::cout << "Index is " + std::to_string(nameInd);*/

			switch (nameInd)
			{
				case 0: // The lower corner.
					southWestLat = latitudeFromString(thisNode.first_child().value());
					southWestLong = longitudeFromString(thisNode.first_child().value());
					break;
				case 1:
					northEastLat = latitudeFromString(thisNode.first_child().value());
					northEastLong = longitudeFromString(thisNode.first_child().value());
					break;
				case 2:
					break;
				case 5:
					loadTuples(thisNode);
					break;
				default:
					break;
			}

			break;
		}

		nameInd++;
	}

	if (thisNode.children().empty())
	{
		return;
	}

	for (pugi::xml_node tChild : thisNode.children())
	{
		// std::cout << tChild.name(); std::cout << "\n";
		parseNodes(tChild);
	}

}

void heightmapTile::parseAllNodes(pugi::xml_document* parsing)
{
	for (pugi::xml_node child : parsing->children())
	{
		parseNodes(child);
	}
}

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