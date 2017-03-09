#include "SoundPlayer.hpp"
#include "mge/config.hpp"

SoundPlayer::SoundPlayer() : _soundBuffers(), _sounds()
{
	_soundBuffers.load(SoundEffect::PickUp, config::MGE_SOUND_PATH + "gun-gunshot-01.wav");
	_soundBuffers.load(SoundEffect::Player_Shoot, config::MGE_SOUND_PATH + "gun-gunshot-01.wav");
	_soundBuffers.load(SoundEffect::Trigger_Activated, config::MGE_SOUND_PATH + "gun-gunshot-01.wav");
	_soundBuffers.load(SoundEffect::Enemy_Hit, config::MGE_SOUND_PATH + "gun-gunshot-01.wav");
}

void SoundPlayer::Play(SoundEffect::ID pEffect, float pVolume)
{
	_sounds.push_back(sf::Sound(_soundBuffers.get(pEffect)));
	_sounds.back().setVolume(pVolume);
	_sounds.back().play();
}

void SoundPlayer::RemoveStoppedSound()
{
	_sounds.remove_if([](const sf::Sound& s)
	{
		return s.getStatus() == sf::Sound::Stopped;
	});
}
