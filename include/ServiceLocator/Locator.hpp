#ifndef LOCATOR_HPP
#define LOCATOR_HPP

#include "ResourceFont.hpp"

class Locator
{
public:
	static void terminate();

	static ResourceFont* getResourceFont();
	static void provideFont(ResourceFont* fontService);
	
private:
	static ResourceFont* fontService;
};

#endif //LOCATOR_HPP