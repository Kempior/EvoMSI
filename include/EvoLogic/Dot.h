#ifndef DOT_H
#define DOT_H

class Dot {
public:
	Dot() = default;
	Dot(float x, float y);
	
	Dot &operator+ (const Dot &other);
	Dot &operator- (const Dot &other);
	Dot &operator* (const float &number);
	Dot &operator/ (const float &number);
	
	float x = 0;
	float y = 0;
};

class DotComparator {
public:
	DotComparator() = delete;
	DotComparator(float (*fun) (float x, float y));
	
	float operator() (const Dot &first, const Dot &second) const;
	
	// Please explain why the one in the comment doesnt't compile
	float (*fun) (float x, float y) = [] (float x, float y) { return 0.0f; };
};

#endif