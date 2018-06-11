#include <iostream>

#include "EvoLogic/Evo.h"
#include "EvoLogic/Enums.h"


Evo::Evo(int popSize, int minX, int maxX, int minY, int maxY) : popSize(popSize), pop(popSize, minX, maxX, minY, maxY) {}

void Evo::NextGeneration() {
	
	Points();
	Costs();
	
	int choice = EvoTypes::ONE_FUN_SELECT | EvoTypes::YES_ELITE | EvoTypes::RECOMBINE_STANDARD;
	
	pop.Mutate(mutationMagnitude, mutationChance);
	
	// Select type
	if (evoType & EvoTypes::ONE_FUN_SELECT)
		pop.Select(populationNumber % 2 == 0 ? fun1 : fun2);
	else if (evoType & EvoTypes::TWO_FUN_SELECT)
		pop.Select(fun1, fun2);
	else if (evoType & EvoTypes::DOMINATED_SELECT)
		pop.DominatedSelect(fun1, fun2);
	
	// No or elite
	if (evoType & EvoTypes::NO_ELITE) {
		// No elite, recombination
		if (evoType & EvoTypes::RECOMBINE_STANDARD)
			pop.Recombine();
		if (evoType & EvoTypes::RECOMBINE_CENTER)
			pop.RecombineCenter();
	}
	else if (evoType & EvoTypes::YES_ELITE) {
		// Elite, recombination
		if (evoType & EvoTypes::RECOMBINE_STANDARD)
			pop.RecombineWithElite();
		if (choice & EvoTypes::RECOMBINE_CENTER)
			pop.RecombineWithEliteCenter();
	}
	
	populationNumber++;
}

const std::vector<std::pair<float, float>> *Evo::Points() const {
	return pop.Points();
}

const std::vector<std::pair<float, float>> *Evo::Costs() const {
	return pop.Costs(fun1, fun2);
}

unsigned int Evo::PopulationNumber() const {
	return populationNumber;
}