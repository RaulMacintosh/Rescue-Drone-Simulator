#include <stdio.h>
#include "map.cpp"

int main(){
	Map x("../maps/01.dat");

	x.printGrid();

	return 0;
}