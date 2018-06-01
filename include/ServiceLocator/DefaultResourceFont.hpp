#ifndef DEFAULTRESOURCEFONT_HPP
#define DEFAULTRESOURCEFONT_HPP

#include "ServiceLocator/ResourceFont.hpp"

class DefaultResourceFont : public ResourceFont
{
public:
	DefaultResourceFont();
	
	sf::Font& getFont();
	
private:
	sf::Font font;
};

#endif //DEFAULTRESOURCEFONT_HPP