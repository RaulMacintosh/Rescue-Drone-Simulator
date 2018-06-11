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
		
		// Probability to move to the up direction
		for(int i = 1; i <= 5; i++){
			if(drone->getX()-i < 0){
				break;
			}
			
			Object* cell = map.getObject(drone->getX()-i, drone->getY());
			
			if(cell->getTagName() != ' '){
				break; // Colocar pra rodar o classificador aqui? Ou Sempre?
			}
		
			double dice = (rand() % 10 + 1)/10.0;

			if(dice <= cell->getProbability()){
				upProbability++;
			}
		}
		
		// Probability to move to the down direction
		for(int i = 1; i <= 5; i++){
			if(drone->getX()+i >= map.getM()){
				break;
			}
			
			Object* cell = map.getObject(drone->getX()+i, drone->getY());
			
			if(cell->getTagName() != ' '){
				break; // Colocar pra rodar o classificador aqui? Ou Sempre?
			}
		
			double dice = (rand() % 10 + 1)/10.0;

			if(dice <= cell->getProbability()){
				downProbability++;
			}
		}

		if (leftProbability >= rightProbability && leftProbability >= upProbability && leftProbability >= downProbability){
			if(map.getObject(drone->getX(), drone->getY()-1)->getTagName() == ' '){
				drone->moveLeft();
				map.getObject(drone->getX(), drone->getY()+1)->setProbability(0.1);
				map.getObject(drone->getX(), drone->getY()+1)->setTagName(' ');
				map.getObject(drone->getX(), drone->getY()+1)->setFeature((rand() % 10 + 20));
				map.getObject(drone->getX(), drone->getY())->setProbability(map.getDrone(index)->getProbability());
				map.getObject(drone->getX(), drone->getY())->setFeature(map.getDrone(index)->getFeature());

			}
		}else if(rightProbability >= leftProbability && rightProbability >= upProbability && rightProbability >= downProbability){
			if(map.getObject(drone->getX(), drone->getY()+1)->getTagName() == ' '){
				drone->moveRight();
				map.getObject(drone->getX(), drone->getY()-1)->setProbability(0.1);
				map.getObject(drone->getX(), drone->getY()-1)->setTagName(' ');
				map.getObject(drone->getX(), drone->getY()-1)->setFeature((rand() % 10 + 20));
				map.getObject(drone->getX(), drone->getY())->setProbability(map.getDrone(index)->getProbability());
				map.getObject(drone->getX(), drone->getY())->setFeature(map.getDrone(index)->getFeature());
			}
		} else if (upProbability >= rightProbability && upProbability >= leftProbability && upProbability >= downProbability){
			if(map.getObject(drone->getX()-1, drone->getY())->getTagName() == ' '){
				drone->moveUp();
				map.getObject(drone->getX()+1, drone->getY())->setProbability(0.1);
				map.getObject(drone->getX()+1, drone->getY())->setTagName(' ');
				map.getObject(drone->getX()+1, drone->getY())->setFeature((rand() % 10 + 20));
				map.getObject(drone->getX(), drone->getY())->setProbability(map.getDrone(index)->getProbability());
				map.getObject(drone->getX(), drone->getY())->setFeature(map.getDrone(index)->getFeature());
			}
		} else if (downProbability >= rightProbability && downProbability >= leftProbability && downProbability >= upProbability){
			if(map.getObject(drone->getX()+1, drone->getY())->getTagName() == ' '){
				drone->moveDown();
				map.getObject(drone->getX()-1, drone->getY())->setProbability(0.1);
				map.getObject(drone->getX()-1, drone->getY())->setTagName(' ');
				map.getObject(drone->getX()-1, drone->getY())->setFeature((rand() % 10 + 20));
				map.getObject(drone->getX(), drone->getY())->setProbability(map.getDrone(index)->getProbability());
				map.getObject(drone->getX(), drone->getY())->setFeature(map.getDrone(index)->getFeature());
			}
		}
	}
}

void printMap(){
	int memoryTime = 5;
	int currentMemoryTime = memoryTime;
	while(!foundObject){
		map.printGrid();
		if(currentMemoryTime == 0){
			currentMemoryTime = memoryTime;
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
