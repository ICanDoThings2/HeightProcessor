#pragma once

#include <algorithm>
#include <cmath>
#include "pugixml.hpp"
#include <vector>
#include <memory>
#include <list>


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
	bool positiionSet = false;

	void setDirections(std::string fromStr);

	void setLow(std::string fromStr);
	void setEnd(std::string fromStr);

	heightmapTile *southTile = nullptr, *northTile = nullptr, *westTile = nullptr, *eastTile = nullptr; // Neighboring non diagonal tiles. 

	std::pair<int, int> islandPosition;

	/*
	* These two coordinate pairs are for the Southewest and Northest corners of a map, respectively.
	*/

	double northEastLat, northEastLong;
	double southWestLat, southWestLong;


public:

	
	std::vector<uint16_t> getRaw();

	heightmapTile(pugi::xml_document *fromDoc);

	bool sameFormat(heightmapTile toCompare);

	bool empty();

	void setNECorner(double asLat, double asLong);

	bool allNeighborsSet();
	void findNeighbors(std::list<heightmapTile>& fromAll);

	std::pair<int, int> getPosition();

	double longitudeLength();
	double latitudeLength();

	bool inList(std::list<heightmapTile>& list);

	bool equalLongitude(heightmapTile* otherTile);
	bool equalLatitude(heightmapTile* otherTile);

	bool southNeighbor(heightmapTile* otherTile);
	bool northNeighbor(heightmapTile* otherTile);
	bool westNeighbor(heightmapTile* otherTile);
	bool eastNeighbor(heightmapTile* otherTile);

	

	enum borderDir { invalid, north, south, east, west };
	borderDir neighborDir(heightmapTile* ourNeighbor);

	std::pair<int, int> newCoord(borderDir fromDir);
	static borderDir dirInt(int asNum);
	heightmapTile currentNeighbor(borderDir toDir);

	double westLong(), eastLong(), northLat(), southLat();

	std::list<heightmapTile> allConnected(std::list<heightmapTile>& allTiles, std::pair<int, int> newPos);

	void sortListToIslands( std::list<heightmapTile> &fromList );

};

