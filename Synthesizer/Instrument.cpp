#include "Instrument.h"
#include "AudioPlayer.h"

Instrument::Instrument()
{
}

Instrument::Instrument(InstrumentData data)
{
	this->wave = data.wave;
}

Instrument::~Instrument()
{
	for (int i = 0; i < notes.size(); ++i) {
		delete notes[i];
	}
	notes.clear();
}

Note* Instrument::addNote(int amplitude, int measure, float measureLength, float offset, float length, NOTE note)
{
	Note* n = new Note(amplitude, measure, measureLength, offset, length, note, this);
	n->setInstrument(this);
	notes.push_back(n);
	return n;
}

Note* Instrument::addNote(Note * note)
{
	note->setInstrument(this);
	notes.push_back(note);
	return note;
}

void Instrument::prepPlay(AudioPlayer * audioPlayer, ThreadManager^ threadManager, unsigned int sampleRate, float bpm, int start_measure, float measure_length)
{
	if (this->audioPlayer != audioPlayer) {
		this->audioPlayer = audioPlayer;
		this->source = audioPlayer->CreateSource();
	}

	Music* musicInfo = new Music;

	musicInfo->audioPlayer = audioPlayer;
	musicInfo->bpm = bpm;
	musicInfo->instrument = this;
	musicInfo->measureLength = measure_length;
	musicInfo->notes = this->notes;
	musicInfo->sampleRate = sampleRate;
	musicInfo->startMeasure = start_measure;
	musicInfo->source = source;

	MusicThread^ thread = gcnew MusicThread(musicInfo);

	if (threadManager->MusicThreadExists(threadID)) {
		threadManager->ReplaceMusicThread(threadID, thread);
	}
	else {
		threadID = threadManager->AddMusicThread(thread);
	}

	thread->getThread()->Start();

	while (!thread->prepComplete);
}

void Instrument::play(ThreadManager^ threadManager)
{
	if (threadManager->MusicThreadExists(threadID)) {
		threadManager->GetMusicThread(threadID)->playMusic();
	}
}

void Instrument::stop(ThreadManager^ threadManager)
{
	if (threadManager->MusicThreadExists(threadID)) {
		threadManager->GetMusicThread(threadID)->stop();
	}
}

Note * Instrument::getNote(NOTE noteValue, int measure, float time)
{
	for(Note* note : notes)
	{
		if (note->getMeasure() == measure && note->getNote() == noteValue) {
			if (note->getOffset() < time&&note->getOffset() + note->getLength() > time)
				return note;
		}
	}
	return nullptr;
}

void Instrument::removeNote(Note * note)
{
	std::vector<Note*>::iterator it = notes.begin();
	for (; it != notes.end(); it++) {
		if (*it == note) {
			break;
		}
	}
	if (it != notes.end()) {
		if (note->isExtended())
			note->getExtension()->setPredecessor(nullptr);
		if (note->hasPredecessor())
			note->getPredecessor()->setExtension(nullptr);
		delete note;
		notes.erase(it);
	}
}

Instrument::InstrumentData Instrument::GetInstrumentData()
{
	InstrumentData data = {};
	data.wave = wave;
	data.noteCount = notes.size();
	for each (Note* note in notes)
	{
		if (note->hasPredecessor())
			data.noteCount--;
	}
	return data;
}
