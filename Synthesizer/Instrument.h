#pragma once

#include "Music.h"
#include "Note.h"
#include "AudioPlayer.h"

#include <vector>


typedef struct {
	int startMeasure;
	float bpm;
	float measureLength;
	unsigned int sampleRate;
	std::vector<Note*> notes;
	AudioPlayer* audioPlayer;
	ALuint source;
	Instrument* instrument;
}Music;

ref class MusicThread {
public:
	
	MusicThread(Music* music) {
		this->music = music;
		playing = false;
		shouldStop = false;
		prepComplete = false;

		thread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &MusicThread::prepMusic));
	};

	void prepMusic() {
		std::sort(this->music->notes.begin(), this->music->notes.end(), Note::SortNotes);
		samples = new std::vector<AudioPlayer::Sample_t>();
		nextSamples = new std::vector<AudioPlayer::Sample_t>();
		samples->resize(music->measureLength*(60.f / music->bpm)*music->sampleRate);
		nextSamples->resize(music->measureLength*(60.f / music->bpm)*music->sampleRate);
		floatSamples = new std::vector<Note::SampleFloat_t>();
		floatSamples->resize(music->measureLength*(60.f / music->bpm)*music->sampleRate);

		currentMeasure = music->startMeasure;

		for (note = 0; note < music->notes.size(); ++note) {
			if (music->notes[note]->getMeasure() >= currentMeasure)
				break;
		}
		for (;note < music->notes.size(); ++note) {
			if (music->notes[note]->getMeasure() >= currentMeasure + 1)
				break;
			music->notes[note]->play(floatSamples, music->sampleRate, music->bpm, music->instrument);
		}

		if (samples->size() != floatSamples->size())
			samples->resize(floatSamples->size());

		for (int i = 0; i < floatSamples->size(); ++i) {
			(*samples)[i].left = __min((int16_t)(*floatSamples)[i].left, 0x7fff);
			(*samples)[i].right = __min((int16_t)(*floatSamples)[i].right, 0x7fff);
			(*floatSamples)[i].left = 0.f;
			(*floatSamples)[i].right = 0.f;
		}
			   		
		currentMeasure++;

		music->audioPlayer->QueueSound(music->source, (*samples), false);

		std::vector<AudioPlayer::Sample_t>* v = nextSamples;
		nextSamples = samples;
		samples = v;

		prepComplete = true;

		playing = false;

		while (!playing);

		music->audioPlayer->PlaySource(music->source);

		while (!shouldStop) {
			if (note < music->notes.size()) {
				while (music->notes[note]->getMeasure() < currentMeasure + 1) {
					music->notes[note]->play(floatSamples, music->sampleRate, music->bpm, music->instrument);
					note++;
					if (note == music->notes.size())
						break;
				}					
				
				currentMeasure++;

				if (nextSamples->size() != floatSamples->size())
					nextSamples->resize(floatSamples->size());
				
				for (int i = 0; i < floatSamples->size(); ++i) {
					(*nextSamples)[i].left = __min((int16_t)(*floatSamples)[i].left, 0x7fff);
					(*nextSamples)[i].right = __min((int16_t)(*floatSamples)[i].right, 0x7fff);
					(*floatSamples)[i].left = 0.f;
					(*floatSamples)[i].right = 0.f;
				}

				music->audioPlayer->QueueSound(music->source, (*nextSamples), true);

				std::vector<AudioPlayer::Sample_t>* v = nextSamples;
				nextSamples = samples;
				samples = v;

				while (music->audioPlayer->SourceBuffersQueuedCount(music->source) > 3 && !shouldStop);
				
			}
			else if (music->audioPlayer->SourceFinished(music->source)) {
				shouldStop = true;
			}
		}

		music->audioPlayer->StopSource(music->source);

		delete music;
		delete samples;
		delete nextSamples;
		delete floatSamples;
		return;
	};

	void playMusic() {
		playing = true;
	};

	void stop() {
		shouldStop = true;
		thread->Join();
	}

	System::Threading::Thread^ getThread() {
		return thread;
	}

	bool playing;
	bool shouldStop;
	bool prepComplete;

protected:
	Music* music;
	std::vector<AudioPlayer::Sample_t>* samples;
	std::vector<AudioPlayer::Sample_t>* nextSamples;
	std::vector<Note::SampleFloat_t>* floatSamples;
	size_t note;
	int currentMeasure;

	System::Threading::Thread^ thread;
};

ref class ThreadManager
{
public:
	ThreadManager() {

	};

	MusicThread^ GetMusicThread(size_t threadID) {
		if (musicThreads.ContainsKey(threadID)) {
			return musicThreads[threadID];
		}
	}

	bool MusicThreadExists(size_t threadID) {
		return musicThreads.ContainsKey(threadID);
	}

	size_t AddMusicThread(MusicThread^ thread) {
		currentKey++;
		size_t key = currentKey;
		musicThreads.Add(key, thread);
		return key;
	}

	void ReplaceMusicThread(size_t key, MusicThread^ thread) {
		if (MusicThreadExists(key)) {
			musicThreads[key]->stop();
			musicThreads[key] = thread;
		}
	}

	void CloseAll() {
		for each (System::Collections::Generic::KeyValuePair<size_t, MusicThread^>^ pair in musicThreads)
		{
			pair->Value->stop();
		}
		musicThreads.Clear();
	}

	//System::

protected:
	System::Collections::Generic::Dictionary<size_t, MusicThread^> musicThreads;

	size_t currentKey = 1;
};

class Instrument
{
public:

	struct InstrumentData {
		WAVE_TYPE wave;
		int noteCount;
		int additionalData[30];
	};

	Instrument();
	Instrument(InstrumentData data);
	~Instrument();
	
	Note* addNote(int amplitude, int measure, float measureLength, float offset, float length, NOTE note);
	Note* addNote(Note* note);

	void prepPlay(AudioPlayer* audioPlayer, ThreadManager^ threadManager, unsigned int sampleRate, float bpm, int start_measure = 0, float measure_length = 4.f);
	void play(ThreadManager^ threadManager);
	void stop(ThreadManager^ threadManager);
	
	Note* getNote(NOTE note, int measure, float time);

	void removeNote(Note* note);

	std::vector<Note*> getNotes() { return notes; };

	void setWaveType(WAVE_TYPE type) { this->wave = type; };
	WAVE_TYPE getWaveType() { return wave; };


	InstrumentData GetInstrumentData();

protected:
	WAVE_TYPE wave;
	std::vector<Note*> notes;
	AudioPlayer* audioPlayer;
	ALuint source;

	size_t threadID;
	size_t musicThreadObjectID;
};

