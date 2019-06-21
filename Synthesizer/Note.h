#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <algorithm>
#include <string>
#include "Music.h"
#include "WaveTables.h"

#define INVALID_DATA_ERROR 1

class Instrument;

class Note
{
public:

	struct NoteData {
		int amplitude;
		float offset;
		float length;
		int measure;
		float measureLength;
		NOTE note;
		bool followUp;
		int additionalData[1];
	};

	Note(int amplitude, int measure, float measureLength, float offset, float length, NOTE note, Instrument* instrument);
	Note(std::string data, Instrument* instrument, int* error=0);
	Note(NoteData data, Instrument* instrument);
	~Note();

	std::string toString();

	typedef struct {
		float left;
		float right;
	}SampleFloat_t;

	void play(std::vector<SampleFloat_t> *samples, unsigned int sampleRate, float bpm, Instrument* instrument);

	void edit(int amplitude, int measure, float measureLength, float offset, float length, NOTE note);

	int getMeasure() { return measure; };
	float getOffset() { return offset; };
	float getLength() { return length; };
	int getAmplitude() { return amplitude; };
	NOTE getNote() { return note; };
	float getMeasureLength() { return measureLength; };

	void select(bool b) { selected = b; };

	bool isSelected() { return selected; };

	void setInstrument(Instrument* instrument);
	Instrument* getInstrument();

	void setPredecessor(Note* note);
	bool hasPredecessor() { return this->followUp; };
	Note* getPredecessor() { return this->predecessor; };

	void setExtension(Note* note);
	bool isExtended() { return this->extended; };
	Note* getExtension() { return this->extension; };

	NoteData getNoteData();

	static bool SortNotes(Note* a, Note* b) { return (a->getMeasure() < b->getMeasure()); };

	struct PlayInfo {
		float speed;
		float start;
		float length;
		float extendedLength;
		double increment;
		double frequency;
		double harmonic_2;
		double harmonic_3;
		double x;
		double y;
		double z;
		double w;
		float amplitude;
		float amplitudeOld;
		int time;
		int sign;
		float endPoint;

		WAVE_TYPE wave;
	};

	PlayInfo getPlayInfo();

protected:
	int amplitude;
	float offset;
	float length;
	int measure;
	float measureLength;
	bool selected;
	bool extended;
	Note* extension;
	bool followUp;
	Note* predecessor;
	NOTE note;
	Instrument* instrument;
	PlayInfo playInfo;

};

