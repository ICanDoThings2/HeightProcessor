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

	std::vector<uint16_t> rawData;
	std::vector< std::vector<uint16_t> > sortedElevation;

	int startX = 0, startY = 0;
	int endX = 0, endY = 0;

	void sortElevation();

	void updateCorners();

	void parseNodes(pugi::xml_node thisNode);
	void parseAllNodes(pugi::xml_document* parsing);
	void loadTuples(pugi::xml_node fromNode);

	bool eastMoving = true; bool southMoving = true;

	void setDirections(std::string fromStr);

	void setLow(std::string fromStr);
	void setEnd(std::string fromStr);

	heightmapTile *southTile, *northTile, *westTile, *eastTile; // Neighboring non diagonal tiles. 


	/*
	* These two coordinate pairs are for the Southewest and Northest corners of a map, respectively.
	*/

	double northEastLat, northEastLong;
	double southWestLat, southWestLong;


public:

	heightmapTile(pugi::xml_document *fromDoc);

	bool sameFormat(heightmapTile toCompare);

	void setNECorner(double asLat, double asLong);

	static void addNeighbors(std::vector<heightmapTile> allTiles);

	double longitudeLength();
	double latitudeLength();

	bool equalLongitude(heightmapTile* otherTile);
	bool equalLatitude(heightmapTile* otherTile);

	bool southNeighbor(heightmapTile* otherTile);
	bool northNeighbor(heightmapTile* otherTile);
	bool westNeighbor(heightmapTile* otherTile);
	bool eastNeighbor(heightmapTile* otherTile);

	enum borderDir { invalid, north, south, east, west };
	borderDir neighborDir(heightmapTile* ourNeighbor);

	double westLong(), eastLong(), northLat(), southLat();


};

