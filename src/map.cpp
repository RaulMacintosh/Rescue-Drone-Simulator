#include <vector>
#include <stdio.h>
#include <stdlib.h> 
#include <string>
#include <fstream>
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
		Map(char* path){
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

						Object drone(('A' + i), x, y);
						drones.push_back(drone);
					}

					file >> output;
					file >> output;
					Object aim_(output.at(0));
					aim = aim_;

					file >> output;
					m = atoi(output.c_str());

					file >> output;
					n = atoi(output.c_str());

					for (int i = 0; i < m; i++){
						vector<Object> row;
						for (int j = 0; j < n; j++){
							file >> output;
							if (output == "D"){
								Object cell(drones[dronesQtd-1].getTagName(), i, j);
								row.push_back(cell);
								dronesQtd -= 1;
							}else if(output == "0"){
								Object cell(' ', i, j);
								row.push_back(cell);
							}else if(output == "1"){
								Object cell(219, i, j);
								row.push_back(cell);
							}else if(output == string(1, aim.getTagName())){
								Object cell(aim.getTagName(), i, j);
								row.push_back(cell);
							}
						}
						grid.push_back(row);
					}
				}
			}

			file.close();
		}

		void addDrone(Object drone){
			drones.push_back(drone);
		}

		void printGrid(){
			printf("________________________________\n");
			for (int i = 0; i < m; i++){
				printf("|");
				for (int j = 0; j < n; j++){
					printf("%c", grid[i][j].getTagName());
				}
				printf("|\n");
			}
			printf("|_______________________________|\n");
		}
};