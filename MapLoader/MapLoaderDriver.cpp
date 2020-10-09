#include "MapLoader.h"

/*
Different files for testing:
solar.map and europe.map are valid
solarError.map features an empty continent
europeError.map features a territory with no borders either way
*/

/*
int main()
{
	//declaring map loader 
	shared_ptr<MapLoader> mapLoader( new MapLoader());
	
	//implement vector of maps, player can choose one
	vector <shared_ptr<Map>> listOfMaps;

	while (true)
	{
		string userInput;
		cout << "Please enter name of the map (with .map or .txt) you would like to load and hit enter." << endl
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
				//cout << "pushing back map " << userInput << "\n"<< endl;
			}
		}
	}

	if (listOfMaps.size() > 0)
	{
		cout << "\nPrinting Maps:\n" << endl;
		for (int i = 0; i < listOfMaps.size(); i++)
		{
			cout << listOfMaps[i]->to_string() << "\n" << endl;
		}
	}

	mapLoader.reset();
	mapLoader = nullptr; //necessary?

	return 0;
}
*/