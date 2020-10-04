#pragma once

#include "Map.h"
#include <fstream>

using namespace std;

struct MapLoader 
{


	MapLoader();
	MapLoader(MapLoader* mapLoader);
	~MapLoader();
	void operator = (MapLoader MapLoader);
	friend ostream& operator<<(std::ostream& strm, MapLoader& mapLoader);

	shared_ptr<Map> createMap(string fileName);
};