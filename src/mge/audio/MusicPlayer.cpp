#include "MusicPlayer.hpp"
#include <iostream>
#include "mge/config.hpp"

MusicPlayer::MusicPlayer() : _music(), _fileNames(), _volume(2.0f)
{
	_fileNames[Music::MenuTheme] = config::MGE_MUSIC_PATH + "1.wav";
	_fileNames[Music::Level1] = config::MGE_MUSIC_PATH + "level.wav";
}

void MusicPlayer::Play(Music::ID pTheme, bool pRepeat)
{
	std::string filename = _fileNames[pTheme];
	if (!_music.openFromFile(filename))
		std::cout << ("Music " + filename + " could not be loaded.") << std::endl;

	_music.setVolume(_volume);
	_music.setLoop(pRepeat);
	_music.play();
}

void MusicPlayer::Stop()
{
	_music.stop();
}

void MusicPlayer::SetPaused(bool pPaused)
{
	if (pPaused) {
		_music.pause();
	}
	else {
		_music.play();
	}
}

void MusicPlayer::SetVolume(float pVolume)
{
	_music.setVolume(pVolume);
}