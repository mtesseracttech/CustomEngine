#ifndef RESOURCEIDENTIFIERS_H
#define RESOURCEIDENTIFIERS_H

namespace sf
{
	class SoundBuffer;
}

namespace Music {
	enum ID {
		MenuTheme,
		Level1,
		Level2,
		WinTheme,
		GameOverTheme,
	};
}

namespace SoundEffect {
	enum ID {
		PickUp,
		Player_Walk,
		Player_Shoot,
		Trigger_Activated,
		Enemy_Hit
	};
}

//Forward declaration of the SFMLResourceHolder.
template <typename Resource, typename Identifier>
class SFMLResourceHolder;

typedef SFMLResourceHolder<sf::SoundBuffer, SoundEffect::ID>	SoundBufferHolder;

#endif // !RESOURCEIDENTIFIERS_H

