#include <random>
#include <ctime>
#include <iostream>
#include <utility>
#include <algorithm>
#include <cmath>
#include "EvoLogic/Population.h"
#include "EvoLogic/Dot.h"
#include "EvoLogic/Enums.h"

#define DEBUG_POPULATION_CPP

const float PI_F = 3.14159265358979f;

Population::Population(int popSize, float minX, float maxX, float minY, float maxY) : size(popSize) {
	constraints = new float[4] {minX, maxX, minY, maxY};
	
	std::mt19937 gen(time(NULL));	//Standard mersenne_twister_engine
	std::uniform_real_distribution<> randX(minX, maxX);
	std::uniform_real_distribution<> randY(minY, maxY);
	
	for(int i = 0; i < popSize; i++) {
		population.push_back(Dot(randX(gen), randY(gen)));
	}
	
#ifdef DEBUG_POPULATION_CPP
	std::cout << "Generating starting population\n";
	PrintElements();
#endif
}

void Population::PrintElements() {
	for(auto &it : population) {
		std::cout << it.x << "  \t" << it.y << std::endl;
	}
}

void Population::PrintElements(float (*fun)(float x, float y)) {
	for(auto &it : population) {
		std::cout << it.x << "  \t" << it.y << "  \t" << fun(it.x, it.y) << std::endl;
	}
}

void Population::Select(float (*fun)(float x, float y), float howMany) {
	
	DotComparator comparator(fun);
	std::sort(population.begin(), population.end(), comparator);
	
	int keptElements = (int)(population.size() * howMany);
	population.erase(population.begin() + keptElements, population.end());
	
#ifdef DEBUG_POPULATION_CPP
	std::cout << "\n\nSelecting (one function)\n";
	PrintElements(fun);
#endif
}

void Population::Select(float (*f1)(float x, float y), float (*f2)(float x, float y), float howMany) {
	
}

void Population::Mutate(float mutationMagnitude, float mutationChance) {
	
	std::mt19937 gen(time(NULL));	//Standard mersenne_twister_engine
	
	// Names should be self-explpaining
	std::uniform_real_distribution<> randMagnitude(0, mutationMagnitude);
	std::uniform_real_distribution<> randAngle(0, 2 * PI_F);
	std::bernoulli_distribution mutationProbability(mutationChance);
	
	for (auto &it : population) {
		if (!mutationProbability(gen))
			continue;
		
		// Yes, the distribution will be clustered around the center. I'm very okay with this.
		// For uniform distribution i'd use x = sqrt(r) * cos(angle) y = sqrt(r) * sin(angle)
		for(int i = 0; i < 1; i++) {
			float newX = it.x + randMagnitude(gen) * cos(randAngle(gen));
			float newY = it.y + randMagnitude(gen) * sin(randAngle(gen));
			
			if (newX < constraints[(int)Constraint::minX] ||
				constraints[(int)Constraint::maxX] < newX ||
				newY < constraints[(int)Constraint::minY] ||
				constraints[(int)Constraint::maxY] < newY) {
					i--;
					continue;
				}
			else {
				it.x = newX;
				it.y = newY;
			}
		}
	}
	
#ifdef DEBUG_POPULATION_CPP
	std::cout << "\n\nMutating\n";
	PrintElements();
#endif
}

std::vector<Dot> Population::RecombineAndReturn(int howMany) {
	std::vector<Dot> returnVec;
	
	std::mt19937 gen(time(NULL));	//Standard mersenne_twister_engine
	
	// Used to calculate the magnitude of change from the center of parents
	std::uniform_real_distribution<> randMagnitude(0, 1);
	// Indexes of parents
	std::uniform_int_distribution<> randIndex(0, population.size() - 1);
	// For chance of rotation to right or left
	std::bernoulli_distribution randBool(0.5);
	
	for (int i = 0; i < howMany; i++) {
		int index1 = randIndex(gen);
		int index2 = randIndex(gen);
		
		if (index1 == index2 || population.size() <= 1) {
			i--;
			continue;
		}
		
		Dot parentsCenter = (population[index1] + population[index2]) / 2;
		Dot distToP2 = population[index2] - parentsCenter;
		
		Dot newDot;
		
		if(randBool(gen)) {
			// Rotation to the right from P2
			float magnitude = randMagnitude(gen);
			newDot.x = parentsCenter.x + magnitude * distToP2.y;
			newDot.y = parentsCenter.y - magnitude * distToP2.x;
		}
		else {
			// Rotation to the left from P2
			float magnitude = randMagnitude(gen);
			newDot.x = parentsCenter.x - magnitude * distToP2.y;
			newDot.y = parentsCenter.y + magnitude * distToP2.x;
		}
		
		if (newDot.x < constraints[(int)Constraint::minX] ||
			constraints[(int)Constraint::maxX] < newDot.x ||
			newDot.y < constraints[(int)Constraint::minY] ||
			constraints[(int)Constraint::maxY] < newDot.y) {
			i--;
			continue;
		}
		else {
			returnVec.push_back(newDot);
		}
	}
	
	return returnVec;
}

void Population::Recombine() {
	std::cout << "\n\nRecombination\n";
	
	population = RecombineAndReturn(size);
	
#ifdef DEBUG_POPULATION_CPP
	std::cout << "\n\nRecombination\n";
	PrintElements();
#endif
}

void Population::RecombineWithElite() {
	std::vector<Dot> newDots = RecombineAndReturn(size - population.size());
	
	population.reserve(population.size() + newDots.size());
	population.insert(population.end(), newDots.begin(), newDots.end());
	
#ifdef DEBUG_POPULATION_CPP
	std::cout << "\n\nRecombination (elite)\n";
	PrintElements();
#endif
}