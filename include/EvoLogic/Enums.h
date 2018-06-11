#ifndef ENUMS_H
#define ENUMS_H

enum class Constraint {
	minX = 0,
	maxX = 1,
	minY = 2,
	maxY = 3,
};

enum EvoTypes {
	
	ONE_FUN_SELECT =		1 << 0,
	TWO_FUN_SELECT =		1 << 1,
	DOMINATED_SELECT =		1 << 2,
	
	NO_ELITE =				1 << 3,
	YES_ELITE =				1 << 4,
	
	RECOMBINE_STANDARD =	1 << 5,
	RECOMBINE_CENTER =		1 << 6,
};

#endif