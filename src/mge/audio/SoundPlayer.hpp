#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <SFML\Audio.hpp>
#include <list> 

#include "ResourceIdentifiers.h"
#include "SFMLResourceHolder.h"

class SoundPlayer : private sf::NonCopyable {
public:
	SoundPlayer();
	void		Play(SoundEffect::ID pEffect, float pVolume = 100.0f);
	void		RemoveStoppedSound();

private:
	SoundBufferHolder		_soundBuffers;
	std::list<sf::Sound>	_sounds;
};
#endif // !SOUNDPLAYER_H
