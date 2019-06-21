#include "Song.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

#include <msclr\marshal_cppstd.h>


using namespace System::Windows::Forms;

namespace little_endian_io
{
	template <typename Word>
	std::ostream& write_word(std::ostream& outs, Word value, unsigned size = sizeof(Word))
	{
		for (; size; --size, value >>= 8)
			outs.put(static_cast <char> (value & 0xFF));
		return outs;
	}
}

Song::Song()
{
	saved = true;
	hasLocation = false;
	saveLocation = "";
	bpm = 100;
}

Song::Song(std::string location)
{
	load(location);
}

Song::Song(System::String ^ loc)
{
	std::string location = msclr::interop::marshal_as<std::string>(loc);
	load(location);	
}


Song::~Song()
{
	for (int i = 0; i < instruments.size(); ++i)
	{
		delete instruments[i];
	}
	instruments.clear();
}

void Song::save(std::string location)
{
	std::ofstream out(location, std::ios::binary);
	if (out.is_open()) {

		SongData songData = {};
		songData.magic = MAGIC_IDENT;
		songData.bpm = bpm;
		songData.sampleRate = sampleRate;
		songData.version = VERSION;
		songData.instrumentCount = instruments.size();
		songData.measureLength = measureLength;

		out.write((const char*)&songData, sizeof(SongData));

		float i = 0.f;
		for(Instrument* instrument : instruments)
		{
			Instrument::InstrumentData instrumentData = instrument->GetInstrumentData();
			out.write((const char*)&instrumentData, sizeof(Instrument::InstrumentData));
			std::vector<Note*> notes = instrument->getNotes();
			for (int j = 0; j < notes.size(); ++j) {
				Note::NoteData note = notes[j]->getNoteData();
				if (!note.followUp)
					out.write((const char*)&note, sizeof(Note::NoteData));
			}
		}

		saved = true;
		hasLocation = true;
		saveLocation = location;
	}
	else {
		MessageBox::Show(L"Indicated file couldn't be opened", L"Sylph Synthesizer");
	}

}

void Song::exportWav(std::string location, unsigned int sampleRate, float bpm, System::ComponentModel::BackgroundWorker^ worker)
{
	//progressOperationTypeIndicator->Text = L"Gathering notes";
	std::vector<Note*> notes;
	for (int i = 0; i < instruments.size(); ++i)
	{
		std::vector<Note*> instrumentNotes = instruments[i]->getNotes();
		notes.resize(notes.size() + instrumentNotes.size());
		for (int j = 0, k = notes.size() - instrumentNotes.size(); j < instrumentNotes.size(); ++j, ++k) {
			notes[k] = instrumentNotes[j];
		}

		worker->ReportProgress(25.f / (float)instruments.size()*(float)(i + 1));
	}
	std::vector<Note::SampleFloat_t> floatSamples;
	std::vector<Note::SampleFloat_t> measureSamples;
	std::sort(notes.begin(), notes.end(), Note::SortNotes);

	int currentMeasure = 1;

	for (int i = 0; i < notes.size(); ++i)
	{
		if (notes[i]->getMeasure() > currentMeasure) {
			currentMeasure++;
			floatSamples.insert(floatSamples.end(), measureSamples.begin(), measureSamples.end());
			measureSamples.clear();
			worker->ReportProgress(25.f + 25.f / (float)notes[notes.size()-1]->getMeasure()*(float)(currentMeasure));
		}
		notes[i]->play(&measureSamples, sampleRate, bpm, notes[i]->getInstrument());
	}

	float largestSample = 0.f;

	float factor = 1.f;

	for (int i = 0; i < floatSamples.size(); ++i) {
		if (abs(floatSamples[i].left) > largestSample) {
			largestSample = abs(floatSamples[i].left);
		}
		if (abs(floatSamples[i].right) > largestSample) {
			largestSample = abs(floatSamples[i].right);
		}
	}

	std::vector<AudioPlayer::Sample_t> samples;
	samples.resize(floatSamples.size());

	if (largestSample < 0x7fff)
		largestSample = 0x7fff;
	
	for (int i = 0; i < floatSamples.size(); ++i) {
		samples[i].left = map(floatSamples[i].left, largestSample*-1.f, largestSample, (float)0x7fff * -1.f, (float)0x7fff);
		samples[i].right = map(floatSamples[i].right, largestSample*-1.f, largestSample, (float)0x7fff * -1.f, (float)0x7fff);
		if (i % 1000 == 0)
			worker->ReportProgress(50.f + 25.f / (float)floatSamples.size()*(float)(i + 1));
	}
	
	
	std::ofstream out(location, std::ios::binary);
	if (out.is_open()) {
		out << "RIFF----WAVEfmt ";
		little_endian_io::write_word(out, 16, 4);
		little_endian_io::write_word(out, 1, 2);
		little_endian_io::write_word(out, 2, 2);
		little_endian_io::write_word(out, sampleRate, 4);
		little_endian_io::write_word(out, sampleRate * 16 * 2 / 8, 4);
		little_endian_io::write_word(out, 4, 2);
		little_endian_io::write_word(out, 16, 2);

		size_t dataChunkPos = out.tellp();
		out << "data----";

		for (int i = 0; i < samples.size(); ++i) {
			little_endian_io::write_word(out, samples[i].left, 2);
			little_endian_io::write_word(out, samples[i].right, 2);
			if (i % 1000 == 0)
				worker->ReportProgress(75.f + 25.f / (float)samples.size()*(float)(i + 1));
		}

		size_t fileLength = out.tellp();

		out.seekp(dataChunkPos + 4);
		little_endian_io::write_word(out, fileLength - dataChunkPos + 8);

		out.seekp(0 + 4);
		little_endian_io::write_word(out, fileLength - 8, 4);

		out.close();
	}
	else {
		MessageBox::Show(L"Indicated file couldn't be opened", L"Sylph Synthesizer");
	}
}

void Song::play(AudioPlayer* player, ThreadManager^ threadManager, unsigned int sampleRate, float bpm, int start_measure, float measure_length)
{
	setSampleRate(sampleRate);
	setMeasureLength(measure_length);
	setBPM(bpm);
	for (int i = 0; i < instruments.size(); ++i) {
		if (instruments[i]->getNotes().size() > 0)
			instruments[i]->prepPlay(player, threadManager, sampleRate, bpm, start_measure, measure_length);
	}
	for (int i = 0; i < instruments.size(); ++i) {
		if (instruments[i]->getNotes().size() > 0)
			instruments[i]->play(threadManager);
	}
}

Instrument * Song::createInstrument()
{
	Instrument* instrument = new Instrument();
	instruments.push_back(instrument);
	return instrument;
}

void Song::destroyInstrument(Instrument* instrument)
{
	std::vector<Instrument*>::iterator it = instruments.begin();
	for (; it != instruments.end(); ++it) {
		if (*it == instrument) {
			break;
		}
	}
	if (it != instruments.end()) {
		delete instrument;
		instruments.erase(it);
	}
}

void Song::addNote(int amplitude, int measure, float measureLength, float offset, float length, NOTE note, Instrument* instrument)
{
	for (int i = 0; i < instruments.size(); ++i) {
		if (instruments[i] == instrument) {
			instrument->addNote(amplitude, measure, measureLength, offset, length, note);
			saved = false;
			return;
		}
	}
}

void Song::removeNote(Instrument * instrument, Note * note)
{
	for (int i = 0; i < instruments.size(); ++i) {
		if (instruments[i] == instrument) {
			instrument->removeNote(note);
			return;
		}
	}
}

void Song::stopPlay(ThreadManager^ threadManager)
{
	for (int i = 0; i < instruments.size(); ++i) {
		instruments[i]->stop(threadManager);
	}
}

void Song::load(std::string location)
{
	bpm = 0;
	saved = true;
	if (location.substr(location.size() - 3) == "not") {
		std::ifstream in(location);

		if (in.is_open()) {
			int key;
			in >> key;
			if (key == 0xace) {
				in >> bpm;
				this->bpm = bpm;
				this->sampleRate = 44100;
				int size;
				in >> size;
				//MessageBox::Show(System::Convert::ToString(size));
				std::string s;
				std::getline(in, s);
				for (int i = 0; i < size; i++) {
					int error = 0;
					//if (progressBar)progressBar->Value = ((float)i + 1.f) / (float)size*100.f;
					std::string data;
					std::getline(in, data);

					std::stringstream noteDataString(data);
					std::string segment;
					std::vector<std::string> segList;

					while (std::getline(noteDataString, segment, ' ')) {
						segList.push_back(segment);
					}

					if (segList.size() != 7) {
						if (segList.size() != 6) {
							continue;
						}
						else {
							segList.push_back("0");
						}
					}

					WAVE_TYPE wave = (WAVE_TYPE)atoi(segList[6].c_str());

					Instrument* instrument = nullptr;

					for (int j = 0; j < instruments.size(); ++j) {
						if (instruments[j]->getWaveType() == wave) {
							instrument = instruments[j];
							break;
						}
					}

					if (instrument == nullptr) {
						instrument = createInstrument();
						instrument->setWaveType(wave);
					}

					Note* note = new Note(data, instrument, &error);
					if (error == INVALID_DATA_ERROR) {
						delete note;
					}
					else {
						
						instrument->addNote(note);
					}
				}
				this->measureLength = instruments[0]->getNotes()[0]->getMeasureLength();
			}
			else {
				MessageBox::Show(L"specified file is not a note file.", L"Sylph Synthesizer");
			}
		}
		in.close();
	}
	else if (location.substr(location.size() - 3) == "syl") {
		hasLocation = true;
		saveLocation = location;
		std::ifstream in(location, std::ios::binary);

		if (in.is_open()) {
			SongData songData = {};
			in.read((char*)&songData, sizeof(SongData));

			if (songData.magic != MAGIC_IDENT) {
				MessageBox::Show(L"Specified file is not a sylph file.", L"Sylph Synthesizer");
				in.close();
				return;
			}
			if (songData.version > VERSION) {
					DialogResult ret = MessageBox::Show(L"The file was made with a newer version of Sylph Synthesizer. Loading this can lead to errors. Are you sure you want to load this file",
					L"Sylph synthesizer", MessageBoxButtons::OKCancel);
					if (ret == DialogResult::Cancel)
						return;
			}

			this->bpm = songData.bpm;
			this->sampleRate = songData.sampleRate;
			this->instruments.resize(songData.instrumentCount);
			this->measureLength = songData.measureLength;
			for (int i = 0; i < instruments.size(); ++i) {
				Instrument::InstrumentData instrumentData = {};
				in.read((char*)&instrumentData, sizeof(Instrument::InstrumentData));

				instruments[i] = new Instrument(instrumentData);
				for (int j = 0; j < instrumentData.noteCount; ++j) {
					Note::NoteData noteData = {};
					in.read((char*)&noteData, sizeof(Note::NoteData));

					if (measureLength != 0)
						noteData.measureLength = measureLength;

					Note* note = new Note(noteData, instruments[i]);

					instruments[i]->addNote(note);
				}
			}
			if (measureLength == 0)
				this->measureLength = instruments[0]->getNotes()[0]->getMeasureLength();
		}
		else {
			MessageBox::Show(L"Specified file could not be opened.", L"Sylph Synthesizer");
		}
	}
}
