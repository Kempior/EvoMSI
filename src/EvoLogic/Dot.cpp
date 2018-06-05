#include "EvoLogic/Dot.h"
#include <iostream>

Dot::Dot(float x, float y) : x(x), y(y) {}

Dot &Dot::operator+ (const Dot &other) {
	return *new Dot(this->x + other.x, this->y + other.y);
}

Dot &Dot::operator- (const Dot &other) {
	return *new Dot(this->x - other.x, this->y - other.y);
}

Dot &Dot::operator* (const float &number) {
	return *new Dot(this->x / number, this->y / number);
}

Dot &Dot::operator/ (const float &number) {
	return *new Dot(this->x / number, this->y / number);
}

float DotComparator::operator() (const Dot &first, const Dot &second) const {
	return fun(first.x, first.y) < fun(second.x, second.y);
}

DotComparator::DotComparator(float (*fun) (float x, float y)) : fun(fun) {}