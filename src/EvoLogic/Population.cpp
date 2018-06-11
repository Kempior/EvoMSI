#include <random>
#include <ctime>
#include <iostream>
#include <utility>
#include <algorithm>
#include <cmath>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include "EvoLogic/Population.h"
#include "EvoLogic/Dot.h"
#include "EvoLogic/Enums.h"

//#define DEBUG_POPULATION_CPP

const float PI_F = 3.14159265358979f;

Population::Population(int popSize, float minX, float maxX, float minY, float maxY) : constraints{{minX, maxX, minY, maxY}}, points(new std::vector<std::pair<float, float>>), costs(new std::vector<std::pair<float, float>>), size(popSize) {
	//constraints = new float[4] {minX, maxX, minY, maxY};
	
	std::mt19937 gen(time(NULL));	//Standard mersenne_twister_engine
	std::uniform_real_distribution<> randX(minX, maxX);
	std::uniform_real_distribution<> randY(minY, maxY);
	
	for(int i = 0; i < popSize; i++) {
		population.push_back(Dot(randX(gen), randY(gen)));
	}
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

const std::vector<std::pair<float, float>> *Population::Points() const {
	
	points->clear();
	
	for (auto &it : population) {
		points->push_back(std::make_pair(it.x, it.y));
	}
	
	return points.get();
}

const std::vector<std::pair<float, float>> *Population::Costs(float (*fun1)(float x, float u), float (*fun2)(float x, float y)) const {
	
	costs->clear();
	
	//const std::vector<std::pair<float, float>> *points = Points();
	
	for (auto &it : *points) {
		costs->push_back(std::make_pair(fun1(it.first, it.second), fun2(it.first, it.second)));
	}
	
	return costs.get();
}

void Population::SelectInPlace(std::vector<Dot> &vec, float (*fun)(float x, float y), float howMany) {
	
	DotComparator comparator(fun);
	std::sort(vec.begin(), vec.end(), comparator);
	
	int keptElements = (int)(vec.size() * howMany);
	vec.erase(vec.begin() + keptElements, vec.end());
}

void Population::Select(float (*fun)(float x, float y), float howMany) {
	
	SelectInPlace(population, fun, howMany);
	
#ifdef DEBUG_POPULATION_CPP
	std::cout << "\n\nSelecting (one function)\n";
	PrintElements(fun);
#endif
}

void Population::Select(float (*f1)(float x, float y), float (*f2)(float x, float y), float howMany) {
	
	shuffle(population.begin(), population.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
	
	std::vector<Dot> firstHalf(population.begin(), population.begin() + population.size() / 2);
	std::vector<Dot> secondHalf(population.begin() + population.size() / 2, population.end());
	
	SelectInPlace(firstHalf, f1, howMany);
	SelectInPlace(secondHalf, f2, howMany);
	
	population.erase(population.begin(), population.end());
	
	population.insert(population.end(), firstHalf.begin(), firstHalf.end());
	population.insert(population.end(), secondHalf.begin(), secondHalf.end());
}

void Population::DominatedSelect(float (*f1)(float x, float u), float (*f2)(float x, float y)) {
	Costs(f1, f2);
	
	for(unsigned int i = 0; i < population.size(); i++) {
		for (unsigned int j = 0; j < population.size() && i != j; j++) {
			// If j-th point is worse under both criteria
			if ((*costs)[i].first < (*costs)[j].first && (*costs)[i].second < (*costs)[j].second) {
				// Probably heavily unoptimized
				population.erase(population.begin() + j);
			}
		}
	}
	
	std::cout << population.size() << '\n';
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
	//std::uniform_real_distribution<> randMagnitude(0, 1);
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
			newDot.x = parentsCenter.x + distToP2.y;
			newDot.y = parentsCenter.y - distToP2.x;
		}
		else {
			// Rotation to the left from P2
			newDot.x = parentsCenter.x - distToP2.y;
			newDot.y = parentsCenter.y + distToP2.x;
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

std::vector<Dot> Population::RecombineAndReturnCenter(int howMany) {
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
		
		Dot parentsCenter = population[index1] + (population[index2] - population[index1]) * randMagnitude(gen);
		
		if (parentsCenter.x < constraints[(int)Constraint::minX] ||
			constraints[(int)Constraint::maxX] < parentsCenter.x ||
			parentsCenter.y < constraints[(int)Constraint::minY] ||
			constraints[(int)Constraint::maxY] < parentsCenter.y) {
			i--;
			continue;
		}
		else {
			returnVec.push_back(parentsCenter);
		}
	}
	
	return returnVec;
}

void Population::Recombine() {
	population = RecombineAndReturn(size);
}

void Population::RecombineWithElite() {
	std::vector<Dot> newDots = RecombineAndReturn(size - population.size());
	
	population.reserve(population.size() + newDots.size());
	population.insert(population.end(), newDots.begin(), newDots.end());
}

void Population::RecombineCenter() {
	population = RecombineAndReturnCenter(size);
}

void Population::RecombineWithEliteCenter() {
	std::vector<Dot> newDots = RecombineAndReturnCenter(size - population.size());
	
	population.reserve(population.size() + newDots.size());
	population.insert(population.end(), newDots.begin(), newDots.end());
}