#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <array>
#include <memory>
#include "Dot.h"

class Population {
private:
	std::array<float, 4> constraints;
	std::vector<Dot> population;
	
	std::vector<Dot> RecombineAndReturn(int howMany);
	std::vector<Dot> RecombineAndReturnCenter(int howMany);
	void SelectInPlace(std::vector<Dot> &vec, float (*fun)(float x, float y), float howMany);
	
	std::unique_ptr<std::vector<std::pair<float, float>>> points;
	mutable std::unique_ptr<std::vector<std::pair<float, float>>> costs;
	
public:
	int size;
	
	Population(int popSize, float minX, float maxX, float minY, float maxY);
	
	void PrintElements();
	void PrintElements(float (*fun)(float x, float y));
	
	const std::vector<std::pair<float, float>> *Points() const;
	const std::vector<std::pair<float, float>> *Costs(float (*fun1)(float x, float u), float (*fun2)(float x, float y)) const;
	
	// Yes, hardcoded to use 1 or 2 functions. Using a vector would be too much effort for this project. Also, nobody is ever gonna use it.
	// Destroys all elements that weren't selected
	void Select(float (*fun)(float x, float y), float howMany = 0.5f);
	void Select(float (*f1)(float x, float u), float (*f2)(float x, float y), float howMany = 0.5f);
	void DominatedSelect(float (*f1)(float x, float u), float (*f2)(float x, float y));
	
	// Mutates existing Dots
	void Mutate(float mutationMagnitude, float mutationChance);
	
	// Recombines the population to the internal size
	void Recombine();
	void RecombineWithElite();
	void RecombineCenter();
	void RecombineWithEliteCenter();
};

#endif