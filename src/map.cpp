#include <vector>
#include <stdio.h>
#include <stdlib.h> 
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
		Map(){}

		Map(char* path){
			setlocale(LC_CTYPE, "");
			ifstream file;
			file.open(path);
			string output;

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

						Object drone(('A' + i), x, y, 0.0);
						drones.push_back(drone);
					}

					file >> output;
					file >> output;
					Object aim_(output.at(0), 0.0);
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
								Object cell(drones[dronesQtd-1].getTagName(), i, j, drones[dronesQtd-1].getProbability());
								row.push_back(cell);
								dronesQtd -= 1;
							}else if(output == "0"){ // If is a empty cell
								Object cell(' ', i, j, 1.0);
								row.push_back(cell);
							}else if(output == "1"){ // If is a wall
								Object cell(L'\u25AE', i, j, 0.0);
								row.push_back(cell);
							}else if(output == string(1, aim.getTagName())){ // If is the aim
								Object cell(aim.getTagName(), i, j, aim.getProbability());
								row.push_back(cell);
							}else{ // If is other objects
								Object cell(output.at(0), i, j, 0.0);
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
			updateMap();
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

			usleep(250000);
		}
		
		void updateProbabilities(){
			for(int i = 0; i < m; i++){
				for(int j = 0; j < n; j++){
					double currentProbability = grid[i][j].getProbability();
					if(currentProbability > 0.0 && currentProbability < 1.0){
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
};
