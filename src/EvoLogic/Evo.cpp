#include "EvoLogic/Evo.h"


Evo::Evo(int popSize, int minX, int maxX, int minY, int maxY) : popSize(popSize), pop(popSize, minX, maxX, minY, maxY), costs(new std::vector<std::pair<float, float>>) {}

void Evo::NextGeneration() {
	
	Points();
	Costs();
	
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

const std::vector<std::pair<float, float>> *Evo::Points() const {
	return pop.Points();
}

const std::vector<std::pair<float, float>> *Evo::Costs() const {
	
	costs->clear();
	
	const std::vector<std::pair<float, float>> *points = Points();
	
	for (auto &it : *points) {
		costs->push_back(std::make_pair(fun1(it.first, it.second), fun2(it.first, it.second)));
	}
	
	return costs.get();
}