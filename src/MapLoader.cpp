#include "MapLoader.h"

MapLoader::MapLoader()
{
}

MapLoader::MapLoader(MapLoader* mapLoader)
{
}

MapLoader::~MapLoader()
{
}

void MapLoader::operator=(MapLoader MapLoader)
{
}

ostream& operator<<(std::ostream& strm, MapLoader& mapLoader)
{
	return strm << "This is a map loader. It can be used to load .map and .txt files";
}

shared_ptr<Map> MapLoader::createMap(string fileName)
{
	//Obtain map name from filename by removing extensions .map or .txt
	string mapName = fileName.substr(0, fileName.length() - 4);

	//debug
	//cout << mapName << endl;

	shared_ptr<Map> map(new Map);

	//file stream
	ifstream inputFileStream;
	inputFileStream.open(fileName);

	if (!inputFileStream.is_open())
	{
		cerr << "Error reading file " << fileName << ", please verify it is present." << endl;
		return nullptr;
	}

	//indicate to the loader about the current section of the file it's in
	bool inContinents = false;
	bool inTerritories = false;
	bool inBorders = false;

	int continentID = 1;
	string nextWord;

	while (inputFileStream >> nextWord) 
	{
		//debug
		//cout << "~~~~~~~~~~~~~~~~~~~ " + nextWord + "~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		
		if (nextWord == "[continents]")
		{
			inContinents = true;
			inTerritories = false;
			inBorders = false;
			inputFileStream >> nextWord;
		}

		if (nextWord == "[countries]") 
		{
			inContinents = false;
			inTerritories = true;
			inBorders = false;
			inputFileStream >> nextWord;
		}

		if (nextWord == "[borders]")
		{
			inContinents = false;
			inTerritories = false;
			inBorders = true;
			inputFileStream >> nextWord;
		}

		//if in a continents line,  create new continent with name and bonus and add to map		
		//Take Current word and next two
		if (inContinents)
		{
			string continentName, bonus, temp;
			
			continentName = nextWord;
			inputFileStream >> bonus;
			inputFileStream >> temp;

			//creating continent and adding to map
			//stoi transforms string to int
			map->add(Continent(continentID, continentName, stoi(bonus)));
			
			//debug
			//cout << "New Continent: " << continentName << " Bonus of: " << bonus << " Garbage value of: " + temp << endl;
			continentID++;
		}

		//If in territories, create new territory and add to map with ID, name, contitnent ID, and discard two garbage values.
		//Add to map once created. 
		if (inTerritories)
		{
			string territoryID, territoryName, continentID, temp1, temp2;
			territoryID = nextWord;
			inputFileStream >> territoryName;
			inputFileStream >> continentID;
			inputFileStream >> temp1;
			inputFileStream >> temp2;

			map->add(Territory(stoi(territoryID), territoryName, stoi(continentID)));

			//debug
			//cout << "New territory with ID: " << territoryID << " Name: " << territoryName << " ContinentID: " << continentID << " Garbage values: " << temp1 << " | " << temp2 << endl;
		}

		//if in borders, link first id with id's that follow. Treat line by line.
		if (inBorders)
		{
			string territoryID = nextWord;
			string territoryToLink;

			//seperate border data by lines
			while (inputFileStream.peek() != '\n') {
				inputFileStream >> territoryToLink;
				map->link(map->getTerritory(stoi(territoryID)), map->getTerritory(stoi(territoryToLink)));
				
				//debug
				cout << "Linking " + nextWord << " and " << territoryToLink << endl;
			}
			
			//debug
			//cout << "end of border" << endl;
		}
	}

	//validating created map.
	if (!map->validate())
	{
		cerr << "Map " << mapName << " is invalid and was rejected by the mapLoader" << endl;
		return nullptr;
	}


	inputFileStream.close();
	std::cout << "Creation of map " << mapName << " succesful!\n" << endl;
	return map;
}
