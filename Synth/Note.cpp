#include "Note.h"

Note::Note(int amplitude, int measure, float measureLength, float offset, float length, NOTE note)
{
	this->amplitude = amplitude;
	this->measure = measure;
	this->measureLength = measureLength;
	this->offset = offset;
	this->length = length;
	this->note = note;
}

Note::~Note()
{
}

void Note::play(std::vector<sf::Int16>* samples, unsigned int sampleRate, float bpm)
{
	float speed = 60.f / bpm;
	float start = (measure*measureLength + offset)*speed;
	float length = this->length*speed;
	if (samples->size() < (start + length)*sampleRate)
		samples->resize((start + length)*sampleRate);
	double increment = getNote(note) / sampleRate;
	double x = 0;
	float amplitudeOld = amplitude;
	int time = 0;
	for (float i = start*sampleRate; i < (int)((start + length)*sampleRate); i++) {
		amplitude = amplitudeOld*__max(0.7, 1.0 *(1 - (float)time / ((float)sampleRate)));
		(*samples)[(int)i] += amplitude*sin(x*M_PI * 2);
		x += increment;
		time++;
	}
}
