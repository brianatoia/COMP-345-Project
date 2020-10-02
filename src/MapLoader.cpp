#include "MapLoader.h"

//file stream
ifstream inputFileStream;


MapLoader::MapLoader()
{
}

MapLoader::MapLoader(MapLoader* mapLoader)
{
}

MapLoader::~MapLoader()
{
}

shared_ptr<Map> MapLoader::createMap(string fileName)
{

	//Obtain map name from filename by removing .map extension
	//-5 to make up for the index starting at 0
	//string mapName = fileName.substr(0, fileName.length() - 5);

	//debug
	//cout << mapName << endl;

	shared_ptr<Map> map(new Map);

	inputFileStream.open(fileName);

	//fix this?
	if (!inputFileStream.is_open())
	{
		//return;
	}

	//add checks for bad files

	//indicate to the loader about the current section of the file it's in
	bool inContinents = false;
	bool inTerritories = false;
	bool inBorders = false;
	int continentID = 1;
	string nextWord;

	while (inputFileStream >> nextWord) 
	{
		//string nextWord;
		//getline(inputFileStream, nextWord, ' ');
		cout << nextWord << endl;
		



		//if in a continents line,  create new continent with name and bonus and add to map
		
		//if in continents section, take current word and next two
		if (inContinents)
		{
			string continentName, bonus, temp;
			
			continentName = nextWord;
			inputFileStream >> bonus;
			inputFileStream >> temp;

			//creating continent and adding to map
			//c_str() transforms string to char[] and stoi transforms string to int
			map->add(Continent(continentID, continentName.c_str() , stoi(bonus)));
			
			//debug
			cout << " | " << continentName << " | " << bonus << " | " + temp << endl;
			continentID++;
		}

		if (inTerritories)
		{
			string territoryID, territoryName, continentID, temp1, temp2;
			territoryID = nextWord;
			inputFileStream >> territoryName;
			inputFileStream >> continentID;
			inputFileStream >> temp1;
			inputFileStream >> temp2;

			map->add(Territory(stoi(continentID), territoryName.c_str(), stoi(continentID)));

		}

		if (nextWord == "[continents]")
		{
			inContinents = true;
			inTerritories = false;
			inBorders = false;
		}

		if (nextWord == "[countries]") 
		{
			inContinents = false;
			inTerritories = true;
			inBorders = false;
		}

		if (nextWord == "[borders]")
		{
			inContinents = false;
			inTerritories = false;
			inBorders = true;
		}

		//make other loop for borders?
		//use mix of get line '\n'
	}

	inputFileStream.close();



	return map;
}
