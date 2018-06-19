#include <thread>
#include <climits>
#include <math.h>
#include "map.cpp"

using namespace std;

Map map;
bool foundObject = false;
Object aim;
int memoryTime = 5;
int iterations = 100;
int currentMemoryTime = memoryTime;

void classifier(Object *obj){
	vector<Object> kCells;

//	Object emptyCell(' ', 1.0, 15);
//	Object aim('*', 1.0, 25);
//	Object wall(L'\u25AE', 0.0, 35);
//	Object dontCare('&', 1.0, 45);

//	kCells.push_back(&emptyCell);
//	kCells.push_back(&aim);
//	kCells.push_back(&wall);
//	kCells.push_back(&dontCare);

	for(int i = 1; i <= 50; i++){
		Object newObj(' ', 1.0, i);
		kCells.push_back(newObj);
	}
	
	for(int i = 0; i < map.visitedCells.size(); i++){
		double distanceToVisited = sqrt(pow((map.visitedCells[i]->getFeature() - obj->getFeature()), 2));
		for(int j = 0; j < kCells.size(); j++){
			double distanceToK = sqrt(pow((kCells[j].getFeature() - obj->getFeature()), 2));
			if(distanceToVisited < distanceToK){
				kCells[j] = *map.visitedCells[i];
			}
		}
	}
	
	int wallsCounter = 0;
	int objectsCounter = 0;
	int emptyCellsCounter = 0;
	int aimCounter = 0;
	
	for(int i = 0; i < kCells.size(); i++){
		if(kCells[i].getFeature() > 10 && kCells[i].getFeature() < 21){
			emptyCellsCounter++;
		}else if(kCells[i].getFeature() > 20 && kCells[i].getFeature() < 31){
			aimCounter++;
		}else if(kCells[i].getFeature() > 30 && kCells[i].getFeature() < 41){
			wallsCounter++;
		}else if(kCells[i].getFeature() > 40 && kCells[i].getFeature() < 51){
			objectsCounter++;
		}
	}

	map.visitedCells.push_back(obj);
	
	if(wallsCounter > objectsCounter &&  wallsCounter > emptyCellsCounter && wallsCounter > aimCounter){
		map.knownWalls.push_back(obj);
	}else if(objectsCounter > wallsCounter && objectsCounter > emptyCellsCounter && objectsCounter > aimCounter){
		map.knownObjects.push_back(obj);
	}else if(emptyCellsCounter > wallsCounter && emptyCellsCounter > objectsCounter && emptyCellsCounter > aimCounter){
		map.knownEmptyCells.push_back(obj);
	}else if(aimCounter > wallsCounter && aimCounter > objectsCounter && aimCounter > emptyCellsCounter){
		foundObject = true;
	}
}

bool objectClassified(Object* obj){
	for(int i = 0; i < map.knownWalls.size(); i++){
		if(obj == map.knownWalls[i]){
			return true;
		}
	}
	
	for(int i = 0; i < map.knownObjects.size(); i++){
		if(obj == map.knownObjects[i]){
			return true;
		}
	}
	
	if(obj->getFeature() == map.getDrone(0)->getFeature() || obj->getFeature() == map.getDrone(1)->getFeature() || obj->getFeature() == map.getDrone(2)->getFeature()){
		return true;
	}
	
	return false;
}

void search(int index){
	Object* drone = map.getDrone(index);
	srand(time(NULL));
		
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

		if(!objectClassified(cell)){
			classifier(cell);
			if(foundObject){
				aim = *cell;
				return;
			}
		}else{
			break;
		}
	
		double dice = (rand() % 10 + 1)/10.0;

		if(dice <= cell->getProbability()){
			if(cell->getFeature() > 20){
				leftProbability++;
			}
			leftProbability++;
		}
	}

	// Probability to move to the right
	for(int i = 1; i <= 5; i++){
		if(drone->getY()+i >= map.getN()){
			break;
		}
		
		Object* cell = map.getObject(drone->getX(), drone->getY()+i);

		if(!objectClassified(cell)){
			classifier(cell);
			if(foundObject){
				aim = *cell;
				return;
			}
		}else{
			break;
		}
		
		double dice = (rand() % 10 + 1)/10.0;

		if(dice <= cell->getProbability()){
			if(cell->getFeature() > 20){
				rightProbability++;
			}
			rightProbability++;
		}
	}
	
	// Probability to move to the up direction
	for(int i = 1; i <= 5; i++){
		if(drone->getX()-i < 0){
			break;
		}
		
		Object* cell = map.getObject(drone->getX()-i, drone->getY());

		if(!objectClassified(cell)){
			classifier(cell);
			if(foundObject){
				aim = *cell;
				return;
			}
		}else{
			break;
		}
		
		double dice = (rand() % 10 + 1)/10.0;

		if(dice <= cell->getProbability()){
			if(cell->getFeature() > 20){
				upProbability++;
			}
			upProbability++;
		}
	}
	
	// Probability to move to the down direction
	for(int i = 1; i <= 5; i++){
		if(drone->getX()+i >= map.getM()){
			break;
		}
		
		Object* cell = map.getObject(drone->getX()+i, drone->getY());

		if(!objectClassified(cell)){
			classifier(cell);
			if(foundObject){
				aim = *cell;
				return;
			}
		}else{
			break;
		}
		
		double dice = (rand() % 10 + 1)/10.0;

		if(dice <= cell->getProbability()){
			if(cell->getFeature() > 20){
				downProbability++;
			}
			downProbability++;
		}
	}

	if (leftProbability >= rightProbability && leftProbability >= upProbability && leftProbability >= downProbability){
		if(map.getObject(drone->getX(), drone->getY()-1)->getTagName() == ' '){
			double oldFeature = map.getObject(drone->getX(), drone->getY()-1)->getFeature();
			drone->moveLeft();

			map.getObject(drone->getX(), drone->getY()+1)->setProbability(0.1);
			map.getObject(drone->getX(), drone->getY()+1)->setTagName(' ');
			map.getObject(drone->getX(), drone->getY()+1)->setFeature(oldFeature);
			
			map.getObject(drone->getX(), drone->getY())->setProbability(map.getDrone(index)->getProbability());
			map.getObject(drone->getX(), drone->getY())->setFeature(map.getDrone(index)->getFeature());
		}
	}else if(rightProbability >= leftProbability && rightProbability >= upProbability && rightProbability >= downProbability){
		if(map.getObject(drone->getX(), drone->getY()+1)->getTagName() == ' '){
			double oldFeature = map.getObject(drone->getX(), drone->getY()+1)->getFeature();
			drone->moveRight();

			map.getObject(drone->getX(), drone->getY()-1)->setProbability(0.1);
			map.getObject(drone->getX(), drone->getY()-1)->setTagName(' ');
			map.getObject(drone->getX(), drone->getY()-1)->setFeature(oldFeature);
			
			map.getObject(drone->getX(), drone->getY())->setProbability(map.getDrone(index)->getProbability());
			map.getObject(drone->getX(), drone->getY())->setFeature(map.getDrone(index)->getFeature());
		}
	} else if (upProbability >= rightProbability && upProbability >= leftProbability && upProbability >= downProbability){
		if(map.getObject(drone->getX()-1, drone->getY())->getTagName() == ' '){
			double oldFeature = map.getObject(drone->getX()-1, drone->getY())->getFeature();
			drone->moveUp();

			map.getObject(drone->getX()+1, drone->getY())->setProbability(0.1);
			map.getObject(drone->getX()+1, drone->getY())->setTagName(' ');
			map.getObject(drone->getX()+1, drone->getY())->setFeature(oldFeature);
			
			map.getObject(drone->getX(), drone->getY())->setProbability(map.getDrone(index)->getProbability());
			map.getObject(drone->getX(), drone->getY())->setFeature(map.getDrone(index)->getFeature());
		}
	} else if (downProbability >= rightProbability && downProbability >= leftProbability && downProbability >= upProbability){
		if(map.getObject(drone->getX()+1, drone->getY())->getTagName() == ' '){
			double oldFeature = map.getObject(drone->getX()+1, drone->getY())->getFeature();
			drone->moveDown();

			map.getObject(drone->getX()-1, drone->getY())->setProbability(0.1);
			map.getObject(drone->getX()-1, drone->getY())->setTagName(' ');
			map.getObject(drone->getX()-1, drone->getY())->setFeature(oldFeature);
			
			map.getObject(drone->getX(), drone->getY())->setProbability(map.getDrone(index)->getProbability());
			map.getObject(drone->getX(), drone->getY())->setFeature(map.getDrone(index)->getFeature());
		}
	}
	
	map.updateMap();
}

bool flyToAim(int index){

	Object* drone = map.getDrone(index);
	
	int distanceToLeft = INT_MAX;
	int distanceToRight = INT_MAX;
	int distanceToUp = INT_MAX;
	int distanceToDown = INT_MAX;
	
	if(drone->getY() > 0){
		if(map.getObject(drone->getX(), drone->getY()-1)->getTagName() == aim.getTagName()){
		   	aim = *drone;
			return true;
		}
		
		distanceToLeft = map.getObject(drone->getX(), drone->getY()-1)->getDistance();
	}
	
	if(drone->getY() < map.getN()-1){
		if(map.getObject(drone->getX(), drone->getY()+1)->getTagName() == aim.getTagName()){
   		   	aim = *drone;
			return true;
		}
		distanceToRight = map.getObject(drone->getX(), drone->getY()+1)->getDistance();
	}
	
	if(drone->getX() > 0){
		if(map.getObject(drone->getX()-1, drone->getY())->getTagName() == aim.getTagName()){
		   	aim = *drone;
			return true;
		}
		distanceToUp = map.getObject(drone->getX()-1, drone->getY())->getDistance();
	}
	
	if(drone->getX() < map.getM()-1){
		if(map.getObject(drone->getX()+1, drone->getY())->getTagName() == aim.getTagName()){
		   	aim = *drone;
			return true;
		}
		distanceToDown = map.getObject(drone->getX()+1, drone->getY())->getDistance();
	}
	
	if(distanceToLeft <= distanceToRight && distanceToLeft <= distanceToUp && distanceToLeft <= distanceToDown){
		if(map.getObject(drone->getX(), drone->getY()-1)->getTagName() == ' '){
			drone->moveLeft();
			Object* emptyCell = map.getObject(drone->getX(), drone->getY()+1);
			
			emptyCell->setTagName(' ');
			emptyCell->setDistance(emptyCell->getDistance()+1);
		}
	}else if(distanceToRight <= distanceToLeft && distanceToRight <= distanceToUp && distanceToRight <= distanceToDown){
		if(map.getObject(drone->getX(), drone->getY()+1)->getTagName() == ' '){
			drone->moveRight();
			Object* emptyCell = map.getObject(drone->getX(), drone->getY()-1);
			
			emptyCell->setTagName(' ');
			emptyCell->setDistance(emptyCell->getDistance()+1);
		}
	}else if(distanceToUp <= distanceToLeft && distanceToUp <= distanceToRight && distanceToUp <= distanceToDown){
		if(map.getObject(drone->getX()-1, drone->getY())->getTagName() == ' '){
			drone->moveUp();
			Object* emptyCell = map.getObject(drone->getX()+1, drone->getY());
			
			emptyCell->setTagName(' ');
			emptyCell->setDistance(emptyCell->getDistance()+1);
		}
	}else if(distanceToDown <= distanceToLeft && distanceToDown <= distanceToRight && distanceToDown <= distanceToUp){
		if(map.getObject(drone->getX()+1, drone->getY())->getTagName() == ' '){
			drone->moveDown();
			Object* emptyCell = map.getObject(drone->getX()-1, drone->getY());
			
			emptyCell->setTagName(' ');
			emptyCell->setDistance(emptyCell->getDistance()+1);
		}
	}
	
	map.updateMap();
	
	return false;
}

void printMap(){
//	map.printGrid();
	if(currentMemoryTime == 0){
		currentMemoryTime = memoryTime;
		map.updateProbabilities();
	}

	currentMemoryTime--;
}

// Manhattan Distance
void fillDistances(){
	for(int i = 0; i < map.visitedCells.size(); i++){
		if(map.visitedCells[i]->getTagName() != aim.getTagName()){
			map.visitedCells[i]->setDistance(abs(aim.getX() - map.visitedCells[i]->getX()) + abs(aim.getY() - map.visitedCells[i]->getY()));
		}
	}
	
	for(int i = 0; i < map.knownWalls.size(); i++){
		map.knownWalls[i]->setDistance(INT_MAX);
	}
	
	for(int i = 0; i < map.knownObjects.size(); i++){
		map.knownObjects[i]->setDistance(INT_MAX);
	}
}

int main(int argc, char* argv[]){

	Map newMap(argv[1]);
	map = newMap;
	memoryTime = atoi(argv[2]);
	iterations = atoi(argv[3]);
	int iterationsPassed = iterations;
	
	string outputFile = argv[4];
	
	while(!foundObject && iterations > 0){
		printMap();
		search(0);
		if(!foundObject){
			search(1);
		}
		
		if(!foundObject){
			search(2);
		}
		
		iterations--;
//		usleep(250000);
	}
	
	iterationsPassed -= iterations;
	
	for(int i = 0; i < map.knownWalls.size(); i++){
		if(map.knownWalls[i]->getTagName() == '*'){
			map.printOutput(outputFile, "Aim found\nClassified as: Wall\nIterations passed: ", iterationsPassed, memoryTime);
			return 0;
		}
	}
	
	for(int i = 0; i < map.knownObjects.size(); i++){
		if(map.knownObjects[i]->getTagName() == '*'){
			map.printOutput(outputFile, "Aim found\nClassified as: Other object\nIterations passed: ", iterationsPassed, memoryTime);
			return 0;
		}
	}
	
	if(iterations == 0){
		map.printOutput(outputFile, "Aim not found\nAim not classified\nIterations passed: ", iterationsPassed, memoryTime);
	}else{
		map.printOutput(outputFile, "Aim found\nClassified as: Aim\nIterations passed: ", iterationsPassed, memoryTime);
	}
	
//	aim.setDistance(INT_MAX);
	
//	fillDistances();

//	bool fstDroneReach = false;
//	bool sndDroneReach = false;
//	bool thdDroneReach = false;
	
//	while(!fstDroneReach || !sndDroneReach || !thdDroneReach){
//		printMap();
		
//		if(!fstDroneReach){
//			if(flyToAim(0)){
//				fstDroneReach = true;
//				fillDistances();
//			}
//		}
		
//		if(!sndDroneReach){
//			if(flyToAim(1)){
//				sndDroneReach = true;
//				fillDistances();
//			}
//		}
		
//		if(!thdDroneReach){
//			if(flyToAim(2)){
//				thdDroneReach = true;
//				fillDistances();
//			}
//		}
		
//		usleep(250000);
//	}

	return 0;
}
