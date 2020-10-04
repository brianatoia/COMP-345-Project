#include "MapLoader.h"


int main()
{
	//declaring map loader 
	shared_ptr<MapLoader> mapLoader( new MapLoader());
	
	//implement while loop to ask for file names
	
	//implement vector of maps, player can choose one

	vector <shared_ptr<Map>> listOfMaps;

	while (true)
	{
		string userInput;
		cout << "Please enter name of the map (with .map) you would like to load and hit enter." << endl
			<< "If you are done selecting maps, enter 1\n" << endl;
		cin >> userInput;
		if (userInput == "1") {
			break;
		}
		else
		{
			shared_ptr<Map> loadedMap = mapLoader->createMap(userInput);
			if (loadedMap != nullptr)
			{
				listOfMaps.push_back(loadedMap);
				//debug
				cout << "pushing back map" << userInput << endl;
			}
		}
	}

	mapLoader.reset();
	mapLoader = nullptr; //necessary?

	return 0;
}