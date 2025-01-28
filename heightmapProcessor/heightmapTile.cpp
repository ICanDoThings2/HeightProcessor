#include "heightmapTile.h"
#include "heightmapIsland.h"
#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <cstdint>

#include <cctype>

#include <clocale>
#include <cwctype>
#include <chrono>



heightmapTile::heightmapTile( pugi::xml_document *fromDoc)
{

	parseAllNodes(fromDoc);
}

bool operator==(heightmapTile A, heightmapTile B)
{
	return A.southLat() == B.southLat() &&
		A.northLat() == B.northLat() &&
		A.westLong() == B.westLong() &&
		A.eastLong() == B.eastLong();
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
	int raw = 0;

	if (fromStr.size() == 0)
	{
		std::cout << "Nothing!?";
	}

	raw = std::round ( std::clamp(std::stod(fromStr), 0.0, 65535.0) );

	if (raw > 0)
	{
		raw += 200; // To simulate above the continental shelf.
	}

	return raw;
}

void heightmapTile::setLow(std::string fromStr)
{
	if (!fromStr.empty())
	{
		startX = std::stoi( fromStr.substr(0, fromStr.find(" ")) );
		startY = std::stoi(fromStr.substr(fromStr.find(" "), fromStr.length()));
	}
}

void heightmapTile::setEnd(std::string fromStr)
{
	if (!fromStr.empty())
	{
		endX = std::stoi( fromStr.substr(0, fromStr.find(" ")) );
		endY = std::stoi(fromStr.substr(fromStr.find(" "), fromStr.length()));
	}
}

bool isCharNumberUsed(char numberChar)
{
	return std::iswdigit(numberChar ) || numberChar == '-' || numberChar == '.';
}

void heightmapTile::sortElevation()
{
	int perRow = endX - startX + 1;

	std::cout << rawData.size() / perRow;
}

void heightmapTile::loadTuples(pugi::xml_node fromNode)
{

	std::string reading = fromNode.first_child().value();

	std::string thisNum = "";

	int maxNumber = rawData.max_size();

	int done = 0;

	for (char c : reading)
	{
		if (isCharNumberUsed(c))
		{
			thisNum += c;
		}
		else
		{
			if (thisNum.length() > 0)
			{
				rawData.push_back(cleaned_char(thisNum));
			}
			
			thisNum = "";
		}
	}

	sortElevation();
}

void heightmapTile::setDirections(std::string fromStr) 
{
	// eastMoving = true; bool southMoving;

	char signA = fromStr.at(0);
	char signB = fromStr.at(2);

	if (signA == '+')
	{
		eastMoving = true;
	}
	if (signA == '-')
	{
		eastMoving = false;
	}

	if (signA != '-' || signA != '+')
	{
		std::cout << "Alarm! Unknown direction setting! Char is " + signA; std::cout << "\n";
	}

	if (signB == '+')
	{
		eastMoving = southMoving = false;
	}
	if (signB == '-')
	{
		southMoving = true;
	}

	if ( (signB != '-') || ( signB != '+' ) )
	{
		std::cout << "Alarm! Unknown direction setting! Char is " + signB; std::cout << "\n";
	}
}


bool heightmapTile::inVector(std::vector<heightmapTile> list)
{

	auto found = std::find(list.begin(), list.end(), *this);

	bool end ( found != list.end() );

	return end;
}

heightmapTile::borderDir heightmapTile::dirInt(int asNum)
{
	return static_cast<heightmapTile::borderDir>(asNum);
}

std::vector<heightmapTile> heightmapTile::allConnected(std::vector<heightmapTile> currentKnown)
{

	std::vector<heightmapTile> uncheckedTiles;

	if ( !inVector(currentKnown) ) // We're not within the list of connected tiles.
	{
		currentKnown.emplace_back(*this);
		// std::cout << "We weren't in";
	}

	uncheckedTiles.emplace_back( *this);

	while (uncheckedTiles.size() > 0)
	{
		for (int s = 1; s < 5; s++)
		{
			heightmapTile thisNeighbor = uncheckedTiles.at(0).currentNeighbor(dirInt(s));

			if (thisNeighbor == nullptr) { continue;  }

			if ( !thisNeighbor.inVector(currentKnown) )
			{
				uncheckedTiles.emplace_back( thisNeighbor );
				currentKnown.emplace_back(thisNeighbor);
			}
		}

		uncheckedTiles.erase(uncheckedTiles.begin());
	}


	return currentKnown;
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
					setLow( thisNode.first_child().value() );
					break;
				case 3:
					setEnd( thisNode.first_child().value());
					break;
				case 5:
					loadTuples(thisNode);
					break;
				case 6:
					setDirections(thisNode.first_attribute().value());
					break;

				case 7: // We've reached the last point needed. We can adjust the rest of the file now.
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
		parseNodes(tChild);
	}

}

void heightmapTile::parseAllNodes(pugi::xml_document* parsing)
{
	if (parsing == nullptr)
	{
		return;
	}

	for (pugi::xml_node child : parsing->children())
	{
		parseNodes(child);
	}
}

bool heightmapTile::sameFormat(heightmapTile toCompare)
{
	std::vector<bool> comparisons; 
	comparisons.emplace_back(eastMoving == toCompare.eastMoving);
	comparisons.emplace_back(southMoving == toCompare.southMoving);
	comparisons.emplace_back(startX == toCompare.startX && startY == toCompare.startY);
	comparisons.emplace_back(endX == toCompare.endX && endY == toCompare.endY);

	for (bool checking : comparisons)
	{
		if (checking == false)
		{
			return false;
		}
	}

	return true;
}

heightmapTile heightmapTile::currentNeighbor(heightmapTile::borderDir toDir)
{
	switch (toDir)
	{
		case north:
			if (northTile != nullptr)
			{
				return *northTile;
			}
			break;

		case south:
			if (southTile != nullptr)
			{
				return *southTile;
			}
			break;

		case east:
			if (eastTile != nullptr)
			{
				return *eastTile;
			}
			break;

		case west:
			if (westTile != nullptr)
			{
				return *westTile;
			}
			break;
	}

	return nullptr;
}

/*
This is a function that is meant to iterate a tile with a given set of unconnected tiles and find connections, and return eventually a list
of totally unconnected to the first tile for the purpose of generating islands. 
*/

void heightmapTile::addNeighbors(std::vector<heightmapTile>& allTiles)
{
	int runs = 0;

	for (heightmapTile altTile : allTiles)
	{
		if ( &altTile == this) // Same guy. No point.
		{
			std::cout << "Skipping!\n";
			continue;
		}

		switch ( neighborDir( &altTile ) )
		{

			case north:
				northTile = &altTile; std::cout << "N Neighbor";
				continue;

			case south:
				southTile = &altTile; std::cout << "S neighbor";
				continue;

			case east:
				eastTile = &altTile; std::cout << "E neighbor";
				continue;

			case west:
				westTile = &altTile; std::cout << "W neighbor";
				continue;

		}
	}
	
}

heightmapTile::borderDir heightmapTile::neighborDir(heightmapTile* ourNeighbor)
{
	if (southNeighbor(ourNeighbor) )
	{
		return heightmapTile::south;
	}

	if (northNeighbor(ourNeighbor))
	{
		return heightmapTile::north;
	}

	if (eastNeighbor(ourNeighbor))
	{
		return heightmapTile::east;
	}

	if (westNeighbor(ourNeighbor))
	{
		return heightmapTile::west;
	}

	return heightmapTile::invalid;
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

bool heightmapTile::southNeighbor(heightmapTile* neighbor)
{
	return southLat() == neighbor->northLat() &&
		northEastLong == neighbor->northEastLong &&
		southWestLong == neighbor->southWestLong;

}

bool heightmapTile::northNeighbor(heightmapTile* neighbor)
{
	return northLat() == neighbor->southLat() &&
		northEastLong == neighbor->northEastLong &&
		southWestLong == neighbor->southWestLong;

}

bool heightmapTile::eastNeighbor(heightmapTile* neighbor)
{
	return eastLong() == neighbor->westLong() &&
		northEastLat == neighbor->northEastLat &&
		southWestLat == neighbor->southWestLat;

}

bool heightmapTile::westNeighbor(heightmapTile* neighbor)
{
	return westLong() == neighbor->eastLong() &&
		northEastLat == neighbor->northEastLat &&
		southWestLat == neighbor->southWestLat;

}


double heightmapTile::westLong()
{
	return southWestLong;
}

double heightmapTile::eastLong()
{
	return northEastLong;
}

double heightmapTile::northLat()
{
	return northEastLat;
}

double heightmapTile::southLat()
{
	return southWestLat;
}
