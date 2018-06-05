#include "EvoLogic/Evo.h"


Evo::Evo(int popSize, int minX, int maxX, int minY, int maxY) : popSize(popSize), pop(popSize, minX, maxX, minY, maxY) {}

void Evo::NextGeneration() {
	switch (3) {
	case 0:
		pop.Select(fun1, fun2);
		pop.Mutate(mutationMagnitude, mutationChance);
		pop.Recombine();
		break;
	case 1:
		pop.Select(populationNumber % 2 == 0 ? fun1 : fun2);
		pop.Mutate(mutationMagnitude, mutationChance);
		pop.Recombine();
		break;
	case 2:
		pop.Select(fun1, fun2);
		pop.Mutate(mutationMagnitude, mutationChance);
		pop.RecombineWithElite();
		break;
	case 3:
		pop.Select(populationNumber % 2 == 0 ? fun1 : fun2);
		pop.Mutate(mutationMagnitude, mutationChance);
		pop.RecombineWithElite();
		break;
	}
	
	populationNumber++;
}