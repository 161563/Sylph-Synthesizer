#pragma once
#include <string>
#include <vector>

#include "Note.h"
#include "Music.h"

class Song
{
public:
	Song();
	Song(std::string location);
	~Song();

	void save(std::string location);

	void play(unsigned int sampleRate);

	bool hasSaveLocation() { return hasLocation; };
	std::string getSaveLocation() { return saveLocation; };

private:
	std::string saveLocation;
	std::vector<Note*> notes;
	std::vector<sf::Int16> samples;
	bool hasLocation;
	float bpm;
};

