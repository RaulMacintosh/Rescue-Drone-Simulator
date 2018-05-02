#include <stdio.h>
#include <thread>
#include <unistd.h>
#include "map.cpp"

using namespace std;

Map map;
bool foundObject = false;

void search(int index){
	while(true){
		usleep(1000000);
		Object* drone = map.getDrone(index);

		if(drone->getX() < 14){
			Object object = map.getObject(drone->getX(), drone->getY()+1);
			
			if(object.getTagName() == ' '){
				drone->moveRight();
			}
		}else{
			break;
		}
	}
}

void printMap(){
	while(!foundObject){
		map.printGrid();
	}
}

int main(int argc, char* argv[]){

	Map newMap(argv[1]);
	map = newMap;

	thread threadMap(printMap);
	thread threadDrone1(search, 0);
	thread threadDrone2(search, 1);
	thread threadDrone3(search, 2);

	threadDrone1.join();
	threadDrone2.join();
	threadDrone3.join();

	foundObject = true;

	threadMap.join();

	return 0;
}