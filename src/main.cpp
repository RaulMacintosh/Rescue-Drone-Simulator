#include <stdio.h>
#include <thread>
#include "map.cpp"
#include "object.cpp"

using namespace std;

Map map;
bool foundObject = false;

void search(int drone){
	
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
	thread threadDrone1(0);
	thread threadDrone2(1);
	thread threadDrone3(2);

	threadDrone1.join();
	threadDrone2.join();
	threadDrone3.join();

	foundObject = true;

	threadMap.join();

	return 0;
}