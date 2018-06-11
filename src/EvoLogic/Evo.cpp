#include <iostream>

#include "EvoLogic/Evo.h"


Evo::Evo(int popSize, int minX, int maxX, int minY, int maxY) : popSize(popSize), pop(popSize, minX, maxX, minY, maxY)/*, costs(new std::vector<std::pair<float, float>>)*/ {}

void Evo::NextGeneration() {
	
	Points();
	Costs();
	
	int oneFunSelect =		0x1;
	int twoFunSelect =		0x2;
	int dominatedSelect =	0x4;
	
	int noElite =			0x8;
	int yesElite =			0x10;
	
	int recombineStandard =	0x20;
	int recombineCenter =	0x40;
	
	const int choice = twoFunSelect | yesElite | recombineCenter;
	
	pop.Mutate(mutationMagnitude, mutationChance);
	
	// Select type
	if (choice & oneFunSelect)
		pop.Select(populationNumber % 2 == 0 ? fun1 : fun2);
	else if (choice & twoFunSelect)
		pop.Select(fun1, fun2);
	else if (choice & dominatedSelect)
		pop.DominatedSelect(fun1, fun2);
	
	// No or elite
	if (choice & noElite) {
		// No elite, recombination
		if (choice & recombineStandard)
			pop.Recombine();
		if (choice & recombineCenter)
			pop.RecombineCenter();
	}
	else if (choice & yesElite) {
		// Elite, recombination
		if (choice & recombineStandard)
			pop.RecombineWithElite();
		if (choice & recombineCenter)
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