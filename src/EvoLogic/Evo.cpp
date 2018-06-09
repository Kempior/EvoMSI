#include "EvoLogic/Evo.h"


Evo::Evo(int popSize, int minX, int maxX, int minY, int maxY) : popSize(popSize), pop(popSize, minX, maxX, minY, maxY) {}

void Evo::NextGeneration() {
	switch (2) {
	case 0:
		pop.Mutate(mutationMagnitude, mutationChance);
		pop.Select(fun1, fun2);
		pop.Recombine();
		break;
	case 1:
		pop.Mutate(mutationMagnitude, mutationChance);
		pop.Select(populationNumber % 2 == 0 ? fun1 : fun2);
		pop.Recombine();
		break;
	case 2:
		pop.Mutate(mutationMagnitude, mutationChance);
		pop.Select(fun1, fun2);
		pop.RecombineWithElite();
		break;
	case 3:
		pop.Mutate(mutationMagnitude, mutationChance);
		pop.Select(populationNumber % 2 == 0 ? fun1 : fun2);
		pop.RecombineWithElite();
		break;
	}
	
	populationNumber++;
}

std::vector<std::pair<float, float>> Evo::Points() const {
	return pop.Points();
}

std::vector<std::pair<float, float>> Evo::Costs() const {
	std::vector<std::pair<float, float>> returnVal;
	std::vector<std::pair<float, float>> points = Points();
	
	for (auto &it : points) {
		returnVal.push_back(std::make_pair(fun1(it.first, it.second), fun2(it.first, it.second)));
	}
	
	return pop.Points();
}