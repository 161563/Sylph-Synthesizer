#include "Song.h"



Song::Song()
{
}

Song::Song(std::string location)
{
}


Song::~Song()
{
}

void Song::save(std::string location)
{
}

void Song::play(unsigned int sampleRate)
{
	for (int i = 0; i < notes.size(); i++) {
		notes[i]->play(&samples, sampleRate, bpm);
	}

	sf::SoundBuffer buffer;
	buffer.loadFromSamples(samples.data(), samples.size(), 1, sampleRate);

	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.play();

}
