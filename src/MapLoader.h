#pragma once

#include "Map.h"
#include <fstream>

using namespace std;

struct MapLoader 
{


	MapLoader();
	MapLoader(MapLoader* mapLoader);
	~MapLoader();
	//add copy, stream overloads

	shared_ptr<Map> createMap(string fileName);

	//needed here?
	ifstream inputFileStream;

	



};