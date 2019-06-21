#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "Music.h"

class Note
{
public:
	Note(int amplitude, int measure, float measureLength, float offset, float length, NOTE note);
	~Note();

	void play(std::vector<sf::Int16> *samples, unsigned int sampleRate, float bpm);

protected:
	int amplitude;
	float offset;
	float length;
	int measure;
	float measureLength;
	NOTE note;

};

