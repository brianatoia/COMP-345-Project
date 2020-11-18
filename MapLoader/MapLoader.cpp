#include "MapLoader.h"

MapLoader::MapLoader()
{
}

MapLoader::MapLoader(const MapLoader&)
{
}

MapLoader::~MapLoader()
{
}

MapLoader& MapLoader::operator=(const MapLoader&)
{
	return *this;
}

ostream& operator<<(std::ostream& strm, MapLoader& mapLoader)
{
	return strm << "This is a map loader. It can be used to load .map and .txt files";
}

shared_ptr<Map> MapLoader::createMap(string fileName, string pathToDirectory)
{
	//Obtain map name from filename by removing extensions .map or .txt
	string mapName = fileName.substr(0, fileName.length() - 4);

	shared_ptr<Map> map(new Map);

	//file stream
	ifstream inputFileStream;
	inputFileStream.open(pathToDirectory+fileName);

	if (!inputFileStream.is_open())
	{
		cout << "Error reading file " << fileName << ", please verify it is present.\n" << endl;
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
			}
		}
	}

	//validating created map.
	if (!map->validate())
	{
		cout << "Map " << mapName << " is invalid and was rejected by the mapLoader\n" << endl;
		return nullptr;
	}

	inputFileStream.close();
	std::cout << "Creation of map " << mapName << " succesful!\n" << endl;
	return map;
}
