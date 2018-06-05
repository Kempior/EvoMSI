#ifndef EVO_H
#define EVO_H

#include <vector>
#include "Population.h"

//class Population;

class Evo {
private:
	int popSize;
	Population pop;
	int populationNumber = 1;
	
public:
	Evo(int popSize, int minX, int maxX, int minY, int maxY);
	
	void NextGeneration();
	
	float (*fun1) (float x, float y) = [] (float x, float y) { return x; };
	float (*fun2) (float x, float y) = [] (float x, float y) { return (1 + y) / x; };
	float mutationMagnitude = 0.25;
	float mutationChance = 0.5;
};

#endif