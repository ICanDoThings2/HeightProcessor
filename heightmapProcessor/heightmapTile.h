#pragma once

#include <algorithm>
#include <cmath>
#include "pugixml.hpp"
#include <vector>
#include <cstdint>

class heightmapTile
{

	
protected:

	std::vector<std::string> usedNames{
		"gml:lowerCorner",
		"gml:upperCorner",
		"gml:low",
		"gml:high",
		"gml:axisLabels",
		"gml:tupleList",
		"gml:sequenceRule",
		"gml:startPoint"
	};


	void parseNodes(pugi::xml_node thisNode);
	void parseAllNodes(pugi::xml_document* parsing);

	bool eastMoving = true; bool southMoving = true;


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

	heightmapTile(pugi::xml_document *fromDoc);

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

