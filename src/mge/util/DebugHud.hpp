#ifndef DEBUGHUD_H
#define DEBUGHUD_H

#include <SFML/Graphics.hpp>
#include <iostream>

class DebugHud
{
	public:
		static void SetupInstance(sf::RenderWindow* window);
		static DebugHud * GetInstance();
		void draw();
		void drawImage();
		void setDebugInfo (std::string pInfo);
		void setUpPosition(float x, float y);
		void setUpFontSize(float size);
		void setUpFontColor(sf::Color pColor);
		void CreateAmmo();
		void CreateTimer();
		void CreateFeedBack(std::string filename);
		bool ShowFeedback(bool show);
		void setTexture(std::string filename,int x,int y);
		void setWindow(sf::RenderWindow* aWindow);
	private:
		DebugHud();
		DebugHud(sf::RenderWindow * window);
		virtual ~DebugHud();
		static DebugHud * _instance;
		bool _showFeedback = false;
		sf::RenderWindow * _window;
		std::string _debugInfo;
		sf::Font _font;
		sf::Texture _image;
		sf::Sprite _sprite;
		sf::Text _debugText;
		void _createDebugHud();
		int _ammo = 10;
		DebugHud(const DebugHud&);
		DebugHud& operator=(const DebugHud&);

};

#endif // DEBUGHUD_H
