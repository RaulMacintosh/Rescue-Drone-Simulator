#include <stdio.h>
#include "map.cpp"

int main(int argc, char* argv[]){
	Map x(argv[1]);

	x.printGrid();

	return 0;
}