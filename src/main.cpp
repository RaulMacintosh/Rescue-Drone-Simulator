#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <thread>
#include "map.cpp"

using namespace std;

Map map;
bool foundObject = false;

void search(int index){
	Object* drone = map.getDrone(index);
	srand(time(NULL));
	while(true){
		usleep(500000);
		
		double leftProbability = 0;
		double rightProbability = 0;
		double upProbability = 0;
		double downProbability = 0;
		
		// Probability to move to the left
		for(int i = 1; i <= 5; i++){
			if(drone->getY()-i < 0){
				break;
			}
			
			Object* cell = map.getObject(drone->getX(), drone->getY()-i);
			
			if(cell->getTagName() != ' '){
				break; // Colocar pra rodar o classificador aqui? Ou Sempre?
			}
		
			double dice = (rand() % 10 + 1)/10.0;
			
			if(dice <= cell->getProbability()){
				leftProbability++;
			}
		}

		// Probability to move to the right
		for(int i = 1; i <= 5; i++){
			if(drone->getY()+i >= map.getN()){
				break;
			}
			
			Object* cell = map.getObject(drone->getX(), drone->getY()+i);
			
			if(cell->getTagName() != ' '){
				break; // Colocar pra rodar o classificador aqui? Ou Sempre?
			}
		
			double dice = (rand() % 10 + 1)/10.0;

			if(dice <= cell->getProbability()){
				rightProbability++;
			}
		}
		
		if (leftProbability > rightProbability && leftProbability > upProbability && leftProbability > downProbability){
			drone->moveLeft();
			map.getObject(drone->getX(), drone->getY()+1)->setProbability(0.1);
			map.getObject(drone->getX(), drone->getY()+1)->setTagName(' ');
			map.getObject(drone->getX(), drone->getY())->setProbability(0.0);
		}else if(rightProbability > leftProbability && rightProbability > upProbability && rightProbability > downProbability){
			drone->moveRight();
			map.getObject(drone->getX(), drone->getY()-1)->setProbability(0.1);
			map.getObject(drone->getX(), drone->getY()-1)->setTagName(' ');
			map.getObject(drone->getX(), drone->getY())->setProbability(0.0);
		}
		
//		Object object = map.getObject(drone->getX()+1, drone->getY());
		
//		if(object.getTagName() == ' '){
//			drone->moveDown();
//		}
	}
}

void printMap(){
	int memoryTime = 1;
	int currentMemoryTime = memoryTime;
	while(!foundObject){
		map.printGrid();
		
		if(memoryTime == 0){
			currentMemoryTime == memoryTime;
			map.updateProbabilities();
		}
		
		currentMemoryTime--;
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
