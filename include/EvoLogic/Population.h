#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <array>
#include "Dot.h"

class Population {
private:
	float *constraints;
	std::vector<Dot> population;
	
	std::vector<Dot> RecombineAndReturn(int howMany);
	void SelectInPlace(std::vector<Dot> &vec, float (*fun)(float x, float y), float howMany);
	
public:
	int size;
	
	Population(int popSize, float minX, float maxX, float minY, float maxY);
	
	void PrintElements();
	void PrintElements(float (*fun)(float x, float y));
	
	std::vector<std::pair<float, float>> Points() const;
	
	// Yes, hardcoded to use 1 or 2 functions. Using a vector would be too much effort for this project. Also, nobody is ever gonna use it.
	// Destroys all elements that weren't selected
	void Select(float (*fun)(float x, float y), float howMany = 0.5f);
	void Select(float (*f1)(float x, float u), float (*f2)(float x, float y), float howMany = 0.5f);
	
	// Mutates existing Dots
	void Mutate(float mutationMagnitude, float mutationChance);
	
	// Recombines the population to the internal size
	void Recombine();
	void RecombineWithElite();
};

#endif