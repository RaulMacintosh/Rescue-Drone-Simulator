#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <iostream>
#include <locale>
#include <unistd.h>
#include "object.cpp"

using namespace std;

class Map{
	private:
		vector<Object> drones;
		vector< vector<Object> > grid;
		Object aim;
		int m;
		int n;
	public:
		vector<Object*> visitedCells;
		
		// Classes
		vector<Object*> knownWalls;
		vector<Object*> knownObjects;
		vector<Object*> knownEmptyCells;
		
		Map(){}

		Map(char* path){
			setlocale(LC_CTYPE, "");
			ifstream file;
			file.open(path);
			string output;
			srand(time(NULL));

			while(!file.eof()){
				file >> output;
				if (output == "Drones:"){
					int dronesQtd;
					file >> output;
					dronesQtd = atoi(output.c_str());
					
					for (int i = 0; i < dronesQtd; i++){
						file >> output;
						file >> output;
						
						file >> output;
						int x = atoi(output.c_str());

						file >> output;
						int y = atoi(output.c_str());
						int droneFeature = (rand() % 10 + 1); // Ranges from 1 to 10
						Object drone(('A' + i), x, y, 0.0, droneFeature);
						drones.push_back(drone);
					}

					file >> output;
					file >> output;
					int aimFeature = (rand() % 10 + 21); // Ranges from 21 to 30
					Object aim_(output.at(0), 1.0, aimFeature);
					aim = aim_;

					file >> output;
					m = atoi(output.c_str());

					file >> output;
					n = atoi(output.c_str());

					for (int i = 0; i < m; i++){
						vector<Object> row;
						for (int j = 0; j < n; j++){
							file >> output;
							if (output == "D"){ // If is a Drone
								Object cell(drones[dronesQtd-1].getTagName(), i, j, drones[dronesQtd-1].getProbability(), drones[dronesQtd-1].getFeature());
								row.push_back(cell);
								dronesQtd -= 1;
							}else if(output == "0"){ // If is a empty cell
								int emptyFeature = (rand() % 10 + 11); // Ranges from 11 to 20
								Object cell(' ', i, j, 1.0, emptyFeature);
								row.push_back(cell);
							}else if(output == "1"){ // If is a wall
								int wallFeature = (rand() % 10 + 31); // Ranges from 31 to 40
								Object cell(L'\u25AE', i, j, 0.0, wallFeature);
								row.push_back(cell);
							}else if(output == string(1, aim.getTagName())){ // If is the aim
								Object cell(aim.getTagName(), i, j, aim.getProbability(), aim.getFeature());
								row.push_back(cell);
							}else{ // If is other objects
								int objectFeature = (rand() % 10 + 41); // Ranges from 41 to 50
								Object cell(output.at(0), i, j, 1.0, objectFeature);
								row.push_back(cell);
							}
						}
						grid.push_back(row);
					}
				}
			}

			file.close();
		}

		Object* getDrone(int index){
			return &drones[index];
		}

		void setObject(Object* obj){
			int x_ = obj->getX();
			int y_ = obj->getY();
			grid[x_][y_] = *obj;
		}
		
		Object* getObject(int x_, int y_){
			return &grid[x_][y_];
		}

		void updateMap(){
			for (int i = 0; i < drones.size(); i++){
				Object d = drones[i];
				grid[d.getX()][d.getY()] = d;
			}
		}

		void printGrid(){
			wcout << " _______________" << endl;
			for (int i = 0; i < m; i++){
				wcout << "|";
				for (int j = 0; j < n; j++){
					wcout << grid[i][j].getTagName();
				}
				wcout << "|" << endl;
			}
			wcout << "|_______________|" << endl;

			for (int i = 0; i < m+2; i++){
				wcout << "\e[A";
			}
		}
		
		void updateProbabilities(){
			for(int i = 0; i < m; i++){
				for(int j = 0; j < n; j++){
					double currentProbability = grid[i][j].getProbability();
					if(currentProbability > 0.0 && currentProbability < 0.9){
						grid[i][j].setProbability(currentProbability + 0.1);
					}
				}
			}
		}
		
		int getM(){
			return m;
		}
		
		int getN(){
			return n;
		}
		
		void printOutput(string fileName, string text, int iterationsRemaining, int memoryTime){
			ofstream file;
			file.open(fileName);
			file << text;
			file << iterationsRemaining;
			file << "\nMemory time: ";
			file << memoryTime;
			file.close();
		}
};
