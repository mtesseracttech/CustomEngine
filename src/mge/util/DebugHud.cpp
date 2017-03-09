#include <cassert>
#include <iostream>
#include <memory>
using namespace std;

#include <GL/glew.h>
#include <SFML/Graphics/Text.hpp>
#include "mge/util/DebugHud.hpp"
#include "mge/config.hpp"

DebugHud::DebugHud(sf::RenderWindow * window): _debugInfo(), _font(), _image(),_sprite(),_debugText(),_window(window)
{
	assert ( _window != NULL );

	if (!_font.loadFromFile(config::MGE_FONT_PATH+ "TESLAFONT.ttf")) {
		cout << "Could not load font, exiting..." << endl;
		return;
	}
	_createDebugHud();
}

DebugHud::~DebugHud()
{
	//dtor
}


void DebugHud::setWindow(sf::RenderWindow* aWindow)
{
	_window = aWindow;
}

void DebugHud::_createDebugHud() 
{
	_debugText.setString("");
	setUpFontSize(50);
	_debugText.setFont(_font);
	setUpFontColor(sf::Color::Green);
	setUpPosition(0, 0);
}

void DebugHud::CreateAmmo()
{
	string ammoCounter = "";
	ammoCounter += to_string(_ammo);
	setUpFontColor(sf::Color::White);
	setUpPosition(200, 1000);
	setDebugInfo(ammoCounter);
}

void DebugHud::setTexture(string filename,int x,int y)
{
	if(!_image.loadFromFile(config::MGE_SPRITE_PATH + filename) )
	{
		std::cout << "fuck you learn how to load shit" << std::endl;
		return;
	}
	_image.setSmooth(true);
	_image.loadFromFile(config::MGE_SPRITE_PATH + filename);
	_sprite.setTexture(_image);	
	_sprite.setPosition(x, y);
	drawImage();
}

void DebugHud::CreateFeedBack(std::string filename)
{
	setTexture(filename,500,500);
}


void DebugHud::setDebugInfo(std::string pInfo) {
    _debugText.setString(pInfo);
	
}

void DebugHud::setUpFontColor(sf::Color pColor)
{
	_debugText.setFillColor(pColor);
}

void DebugHud::setUpPosition(float x, float y)
{
	_debugText.setPosition(x, y);
}

void DebugHud::setUpFontSize(float size)
{
	_debugText.setCharacterSize(size);
}

void DebugHud::SetupInstance(sf::RenderWindow * window)
{
	if(!_instance) _instance = new DebugHud(window);
	else std::cout << "Debugwindow is already created!" << std::endl;
}

DebugHud * DebugHud::GetInstance()
{
	if (!_instance) std::cout << "Call SetupInstance before calling GetInstance!" << std::endl;
	return _instance;
}

void DebugHud::draw()
{
	//glDisable( GL_CULL_FACE );
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	_window->draw(_debugText);
	_window->popGLStates();
}

void DebugHud::drawImage()
{
	glDisable( GL_CULL_FACE );
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	_window->draw(_debugText);
	_window->popGLStates();
}

DebugHud * DebugHud::_instance;
