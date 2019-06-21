#include "AudioPlayer.h"

#include <iostream>

AudioPlayer::AudioPlayer()
{
	device = alcOpenDevice(NULL);
	if (!device) {
		std::cout << "[Error] Creating sound device failed" << std::endl;
		return;
	}

	context = alcCreateContext(device, NULL);
	if (!context) {
		std::cout << "[Error] Creating sound context failed" << std::endl;
		return;
	}

	alcMakeContextCurrent(context);

	gain = minGain = maxGain = 0.f;
	maxGain = 1.f;
	ALenum error = alGetError();

	alGetListenerf(AL_GAIN, (ALfloat*)(&gain));

	error = alGetError();

}


AudioPlayer::~AudioPlayer()
{
	for (int i = 0; i < sources.size(); ++i) {
		ALint playing = AL_PLAYING;
		ALint finishedBuffers = 0;
		ALint queuedBuffers = sources[i]->queuedBuffers.size();
		while (playing == AL_PLAYING && finishedBuffers != queuedBuffers) {
			alGetSourcei(sources[i]->source, AL_SOURCE_STATE, &playing);
			alGetSourcei(sources[i]->source, AL_BUFFERS_QUEUED, &queuedBuffers);
			alGetSourcei(sources[i]->source, AL_BUFFERS_PROCESSED, &finishedBuffers);
		}
		while (!(sources[i]->queuedBuffers.empty())) {
			sources[i]->freeBuffers.push_back(sources[i]->queuedBuffers.front());
			sources[i]->queuedBuffers.pop();
		}
		alDeleteBuffers(sources[i]->freeBuffers.size(), sources[i]->freeBuffers.data());
		alDeleteSources(1, &(sources[i]->source));
		delete sources[i];
	}

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

ALuint AudioPlayer::CreateSource()
{
	Source* source = new Source();
	sources.push_back(source);

	alGenSources(1, &(source->source));

	ALenum error;

	if ((error = alGetError()) != AL_NO_ERROR) {
		std::cout << "[Error] genSources returned " << error << std::endl;
		return 0;
	}

	source->freeBuffers.resize(3);
	alGenBuffers(source->freeBuffers.size(), source->freeBuffers.data());

	if ((error = alGetError()) != AL_NO_ERROR) {
		std::cout << "[Error] genBuffers returned " << error << std::endl;
		return 0;
	}

	source->gain = source->minGain = source->maxGain = 0.f;

	alGetSourcef(source->source, AL_MIN_GAIN, (ALfloat*)(&source->minGain));
	alGetSourcef(source->source, AL_MAX_GAIN, (ALfloat*)(&source->maxGain));
	alGetSourcef(source->source, AL_GAIN, (ALfloat*)(&source->gain));

	return source->source;
}

void AudioPlayer::QueueSound(ALuint source, std::vector<Sample> samples, bool autoPlay)
{
	for (int i = 0; i < sources.size(); ++i) {
		if (sources[i]->source != source)
			continue;

		Source* s = sources[i];

		ALuint buffer;
		if (s->freeBuffers.size() == 0) {
			ALint processed = 0;
			alGetSourcei(s->source, AL_BUFFERS_PROCESSED, &processed);
			if (processed > 0) {
				for (int j = 0; j < processed; ++j) {
					s->freeBuffers.push_back(s->queuedBuffers.front());
					s->queuedBuffers.pop();
				}
				alSourceUnqueueBuffers(s->source, processed, s->freeBuffers.data());
				buffer = s->freeBuffers[0];
				s->freeBuffers.erase(s->freeBuffers.begin());
			}
			else {
				alGenBuffers(1, &buffer);
			}
		}
		else {
			buffer = s->freeBuffers[0];
			s->freeBuffers.erase(s->freeBuffers.begin());
		}
		alBufferData(buffer, AL_FORMAT_STEREO16, samples.data(), samples.size() * sizeof(Sample), SAMPLE_RATE);

		s->queuedBuffers.push(buffer);

		alSourceQueueBuffers(s->source, 1, &buffer);

		if (autoPlay) {
			ALint state;
			alGetSourcei(s->source, AL_SOURCE_STATE, &state);
			if (state != AL_PLAYING) {
				alSourcePlay(s->source);
			}
		}

		return;
	}
}

void AudioPlayer::PlaySource(ALuint source)
{
	for (int i = 0; i < sources.size(); ++i) {
		if (sources[i]->source != source)
			continue;

		ALint state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING) {
			alSourcePlay(source);
		}

	}
}

void AudioPlayer::StopSource(ALuint source)
{
	for (int i = 0; i < sources.size(); ++i) {
		if (sources[i]->source != source)
			continue;

		alSourceStop(sources[i]->source);

		std::vector<ALuint> unqueueBuffers;

		while (!(sources[i]->queuedBuffers.empty())) {
			unqueueBuffers.push_back(sources[i]->queuedBuffers.front());
			sources[i]->queuedBuffers.pop();
		}
		alSourceUnqueueBuffers(source, unqueueBuffers.size(), unqueueBuffers.data());
		for (int j = 0; j < unqueueBuffers.size(); ++j) {
			sources[i]->freeBuffers.push_back(unqueueBuffers[j]);
		}
		return;
	}
}

bool AudioPlayer::SourceFinished(ALuint source)
{
	for (int i = 0; i < sources.size(); ++i) {
		if (sources[i]->source != source)
			continue;

		ALint state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING) {
			return true;
		}
		else {
			return false;
		}

	}
}

int AudioPlayer::SourceBuffersQueuedCount(ALuint source)
{
	for (int i = 0; i < sources.size(); ++i) {
		if (sources[i]->source != source)
			continue;

		ALint processed = 0;
		alGetSourcei(sources[i]->source, AL_BUFFERS_PROCESSED, &processed);
		if (processed > 0) {
			for (int j = 0; j < processed; ++j) {
				sources[i]->freeBuffers.push_back(sources[i]->queuedBuffers.front());
				sources[i]->queuedBuffers.pop();
			}
			alSourceUnqueueBuffers(sources[i]->source, processed, sources[i]->freeBuffers.data());
		}

		ALint enqueued = 0;
		alGetSourcei(source, AL_BUFFERS_QUEUED, &enqueued);

		return (int)enqueued;
	}
}

void AudioPlayer::SourceSetGain(ALuint source, float gain)
{
	for (int i = 0; i < sources.size(); ++i) {
		if (sources[i]->source != source)
			continue;

		gain = __min(sources[i]->maxGain, __max(sources[i]->minGain, gain));
		
		alSourcef(source, AL_GAIN, gain);

		sources[i]->gain = gain;

		return;
	}
}

float AudioPlayer::SourceGetGain(ALuint source)
{
	for (int i = 0; i < sources.size(); ++i) {
		if (sources[i]->source != source)
			continue;

		return sources[i]->gain;
	}
}

float AudioPlayer::SourceGetMinGain(ALuint source)
{
	for (int i = 0; i < sources.size(); ++i) {
		if (sources[i]->source != source)
			continue;

		return sources[i]->minGain;
	}
}

float AudioPlayer::SourceGetMaxGain(ALuint source)
{
	for (int i = 0; i < sources.size(); ++i) {
		if (sources[i]->source != source)
			continue;

		return sources[i]->maxGain;
	}
}

void AudioPlayer::ListenerSetGain(float gain)
{
	gain = __min(maxGain, __max(minGain, gain));

	alListenerf(AL_GAIN, gain);

	this->gain = gain;
}

float AudioPlayer::ListenerGetGain()
{
	return this->gain;
}

float AudioPlayer::ListenerGetMinGain()
{
	return this->minGain;
}

float AudioPlayer::ListenerGetMaxGain()
{
	return this->maxGain;
}

void AudioPlayer::DestroySource(ALuint source, bool waitForFinish)
{
	std::vector<Source*>::iterator it;
	for (it = sources.begin(); it != sources.end(); ++it) {
		if (it[0]->source == source) {
			alSourceStop(it[0]->source);

			std::vector<ALuint> unqueueBuffers;

			while (!(it[0]->queuedBuffers.empty())) {
				unqueueBuffers.push_back(it[0]->queuedBuffers.front());
				it[0]->queuedBuffers.pop();
			}
			alSourceUnqueueBuffers(it[0]->source, unqueueBuffers.size(), unqueueBuffers.data());
			alDeleteBuffers(unqueueBuffers.size(), unqueueBuffers.data());
			alDeleteBuffers(it[0]->freeBuffers.size(), it[0]->freeBuffers.data());
			alDeleteSources(1, &(it[0]->source));

			delete it[0];
			break;
		}
	}
	if (it != sources.end()) {
		sources.erase(it);
	}
}
