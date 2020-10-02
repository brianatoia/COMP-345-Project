#pragma once

#include "Map.h"
#include <fstream>

using namespace std;

struct MapLoader 
{

	MapLoader();
	MapLoader(MapLoader* mapLoader);
	~MapLoader();


	shared_ptr<Map> createMap(string fileName);

	//needed here?
	ifstream inputFileStream;

	



};