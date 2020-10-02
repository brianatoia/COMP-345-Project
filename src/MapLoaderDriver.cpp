#include "MapLoader.h"


int main()
{

	shared_ptr<MapLoader> mapLoader( new MapLoader);
	
	//implement while loop to ask for file names
	
	//implement vector of maps, player can choose one

	shared_ptr<Map> loadedMap = mapLoader->createMap("solar.map");

	//cout << "success" << endl;
	//cout << loadedMap->to_string() << endl;
	return 0;
}