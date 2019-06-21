#pragma once

#include <al.h>
#include <alc.h>

#include <vector>
#include <queue>

#define SAMPLE_RATE 44100

class AudioPlayer
{
public:
	AudioPlayer();
	~AudioPlayer();

	ALuint CreateSource();

	typedef struct Sample {
		int16_t left;
		int16_t right;
	}Sample_t;
	void QueueSound(ALuint source, std::vector<Sample> samples, bool autoPlay = true);

	void PlaySource(ALuint source);
	void StopSource(ALuint source);

	bool SourceFinished(ALuint source);

	int SourceBuffersQueuedCount(ALuint source);

	void SourceSetGain(ALuint source, float gain);
	float SourceGetGain(ALuint source);

	float SourceGetMinGain(ALuint source);
	float SourceGetMaxGain(ALuint source);

	void ListenerSetGain(float gain);
	float ListenerGetGain();

	float ListenerGetMinGain();
	float ListenerGetMaxGain();


	void DestroySource(ALuint source, bool waitForFinish);

protected:
	struct Source {
		ALuint source;
		std::queue<ALuint> queuedBuffers;
		std::vector<ALuint> freeBuffers;
		float minGain;
		float maxGain;
		float gain;
	};

	std::vector<Source*> sources;

	float minGain, maxGain, gain;

	ALCdevice* device;
	ALCcontext* context;
};

