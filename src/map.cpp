#include <vector>
#include <stdio.h>
#include <string>
#include "object.cpp"

using namespace std;

class Map{
	private:
		vector<Object> drones;
		vector< vector<Object> > grid;
		int m;
		int n;
	public:
		Map(int m_, int n_){
			m = m_;
			n = n_;

			for (int i = 0; i < m; i++){
				vector<Object> row;
				for (int j = 0; j < n; j++){
					Object empty(" ", i, j);
					row.push_back(empty);
				}
				grid.push_back(row);
			}
		}

		void addDrone(Object drone){
			drones.push_back(drone);
		}

		void printGrid(){
			printf("_______________________________________________________________\n");
			for (int i = 0; i < m; i++){
				printf("| ");
				for (int j = 0; j < n; j++){
					printf("%s ", grid[i][j].getTagName().c_str());
				}
				printf("|\n");
			}
			printf("|_____________________________________________________________|\n");
		}
};