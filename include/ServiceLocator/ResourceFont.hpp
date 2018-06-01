#ifndef RESOURCEFONT_HPP
#define RESOURCEFONT_HPP

#include "SFML/Graphics.hpp"

class ResourceFont
{
public:
	virtual ~ResourceFont() {}

	virtual sf::Font& getFont() = 0;
};

#endif //RESOURCEFONT_HPP