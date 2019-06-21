#pragma once
#include <string>
#include <vector>

#include "Note.h"
#include "Music.h"

#include "AudioPlayer.h"

#include "Instrument.h"


class Song
{
public:
	Song();
	Song(std::string location);
	Song(System::String ^ loc);
	~Song();

	void save(std::string location);

	void exportWav(std::string location, unsigned int sampleRate, float bpm, System::ComponentModel::BackgroundWorker^ worker);

	void play(AudioPlayer* player, ThreadManager^ threadManager, unsigned int sampleRate, float bpm, int start_measure = 0, float measure_length=4.f);

	void addNote(int amplitude, int measure, float measureLength, float offset, float length, NOTE note, Instrument* instrument);

	Instrument* createInstrument();
	void destroyInstrument(Instrument* instrument);

	bool hasSaveLocation() { return hasLocation; };
	bool isSaved() { return saved; };
	void setBPM(float bpm) { if (this->bpm != bpm) { this->bpm = bpm; saved = false; } };
	float getBPM() { return bpm; };

	void setSampleRate(int sampleRate) { if (this->sampleRate != sampleRate) { this->sampleRate = sampleRate; this->saved = false; } };
	int getSampleRate() { return this->sampleRate; };

	void setMeasureLength(float measureLength) { if (this->measureLength != measureLength) { this->measureLength = measureLength; this->saved = false; } };
	int getMeasureLength() { return this->measureLength; };

	std::vector<Instrument*> getInstruments() { return instruments; };

	void removeNote(Instrument* instrument, Note* note);

	void stopPlay(ThreadManager^ threadManager);

	std::string getSaveLocation() { return saveLocation; };


private:

	struct SongData {
		int magic;
		float version;
		int sampleRate;
		float bpm;
		int instrumentCount;
		float measureLength;
		int additionalData[26];
	};

	void load(std::string location);

	std::string saveLocation;
	std::vector<Instrument*> instruments;
	std::vector<float> samples;
	float bpm;
	int sampleRate;
	float measureLength;
	bool hasLocation, saved;
};

