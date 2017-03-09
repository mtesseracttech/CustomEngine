#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <SFML/Audio.hpp>
#include <map>

#include"ResourceIdentifiers.h"

class MusicPlayer : private sf::NonCopyable {
public:
	MusicPlayer();
	void	Play(Music::ID pTheme, bool pRepeat = true);
	void	Stop();

	void	SetPaused(bool pPaused);
	void	SetVolume(float pVolume);
private:

	sf::Music							_music;
	std::map<Music::ID, std::string>	_fileNames;
	float								_volume;
};
#endif // !MUSICPLAYER_H

