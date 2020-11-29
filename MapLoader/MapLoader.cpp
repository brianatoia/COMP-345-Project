#include "MapLoader.h"

MapLoader::MapLoader() {}

MapLoader::MapLoader(const MapLoader&) {}

MapLoader::~MapLoader() {}

MapLoader& MapLoader::operator=(const MapLoader&) { return *this; }

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
	inputFileStream.open(pathToDirectory + fileName);

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
			cout << nextWord;
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

//===========================Conquest File Reader Section (A3) ==================================
ConquestFileReader::ConquestFileReader() {}

ConquestFileReader::ConquestFileReader(const ConquestFileReader&) {}

ConquestFileReader::~ConquestFileReader() {}

ConquestFileReader& ConquestFileReader::operator=(const ConquestFileReader&) { return *this; }

ostream& operator<<(std::ostream& strm, ConquestFileReader& mapLoader)
{
	return strm << "This is the Conquest file reader. It can be used to load .map and .txt files of Conquest maps";
}

shared_ptr<Map> ConquestFileReader::createMap(string fileName, string pathToDirectory)
{
	//Obtain map name from filename by removing extensions .map or .txt
	string mapName = fileName.substr(0, fileName.length() - 4);

	shared_ptr<Map> map(new Map);

	//file stream
	ifstream inputFileStream;
	inputFileStream.open(pathToDirectory + fileName);

	if (!inputFileStream.is_open())
	{
		cout << "Error reading file " << fileName << ", please verify it is present.\n" << endl;
		return nullptr;
	}

	//indicate to the loader about the current section of the file it's in
	bool inContinents = false;
	bool inTerritories = false;

	int continentID = 1;
	int territoryID = 1;
	string line;

	//Used for linking after territories are created
	vector<vector<string>> territories;

	vector <string> filler; //Filling index 0 since we will use territory IDs as the index for simplicity
	territories.push_back(filler);

	while (getline(inputFileStream, line))
	{
		bool endOfFile = false;

		//Get rid of empty lines
		while (line == "")
		{
			getline(inputFileStream, line);
			if (inputFileStream.eof())
			{
				endOfFile = true;
				break;
			}
		}

		if (endOfFile) break;

		if (line == "[Continents]")
		{
			inContinents = true;
			inTerritories = false;
			getline(inputFileStream, line);
		}

		if (line == "[Territories]")
		{
			inContinents = false;
			inTerritories = true;
			getline(inputFileStream, line);
		}

		//if in a continents line,  create new continent with name and bonus and add to map		
		if (inContinents)
		{
			string continentName, bonus;

			//Split line, before '=' is the name and everything after is bonus
			continentName = line.substr(0, line.find('='));
			bonus = line.substr(line.find('=') + 1, line.length());

			//creating continent and adding to map
			//stoi transforms string to int
			map->add(Continent(continentID, continentName, stoi(bonus)));

			continentID++;
		}

		//If in territories, create new territory and add to map with ID, name, contitnent ID, and discard two garbage values.
		//Add to map once created. 
		if (inTerritories)
		{
			vector<string> territory;
			//tracks the position of the latest 
			int lastCommaIndex = 0;
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == ',')
				{
					territory.push_back(line.substr(lastCommaIndex, i - lastCommaIndex));
					lastCommaIndex = i + 1;
				}

				//for the last border
				if (i == line.length() - 1)
				{
					territory.push_back(line.substr(lastCommaIndex, line.length()));
				}
			}

			/*Conquest maps add the borders at the same time, not in a seperate section like Warzone
			however this means that we would need to link with territories that are created after this one.
			To solve this the territory is created but all its linking info is saved and then executed after
			all territories are created. This is the reason for the territories 2D vector.
			*/
			territories.push_back(territory);

			//First word is name, next two are garbage values, next one is ContinentName it belongs to, all after are borders
			string territoryName = territories[territoryID][0];
			cout << territoryName << endl;
			int continentID = map->findContinent(territories[territoryID][3])->getID();

			//Adding the territory
			map->add(Territory(territoryID, territoryName, continentID));

			territoryID++;
		}
	}

	//Once all territories are created, link them together using info from before
	for (int i = 1; i != territoryID; i++) //Loop through territories
	{
		for (int j = 4; j < territories[i].size(); j++) //Loop through the territories that territory links to
		{
			map->link(map->findTerritory(territories[i][0]), map->findTerritory(territories[i][j]));
		}
	}

	//validating created map.
	if (!map->validate())
	{
		cout << "Conquest Map " << mapName << " is invalid and was rejected by the mapLoader\n" << endl;
		return nullptr;
	}

	inputFileStream.close();
	cout << "Creation of Conquest map " << mapName << " succesful!\n" << endl;
	return map;
}

