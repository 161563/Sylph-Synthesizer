#include "Note.h"

#include <fstream>
#include <sstream>

#include "Instrument.h"


using namespace System::Windows::Forms;



#define ADD_SAMPLE_LEFT(samples, i, value) (*samples)[(int)i].left = (value + (*samples)[(int)i].left)
#define ADD_SAMPLE_RIGHT(samples, i, value) (*samples)[(int)i].right = (value + (*samples)[(int)i].right)

Note::Note(int amplitude, int measure, float measureLength, float offset, float length, NOTE note, Instrument* instrument)
{
	while (offset >= measureLength) {
		offset -= measureLength;
		measure++;
	}
	this->amplitude = amplitude;
	this->measure = measure;
	this->measureLength = measureLength;
	this->offset = offset;
	this->length = length;
	this->note = note;
	selected = false;
	this->instrument = instrument;
	this->extended = false;
	this->followUp = false;

	if (offset + length > measureLength) {
		this->length = measureLength - offset;
		extended = true;
		this->extension = new Note(amplitude, measure + 1, measureLength, 0, length - this->length, note, instrument);
		instrument->addNote(extension);
		this->extension->setPredecessor(this);
	}
}

Note::Note(std::string data, Instrument* instrument, int* error)
{
	std::stringstream in(data);
	std::string segment;
	std::vector<std::string> segList;

	while (std::getline(in, segment, ' ')) {
		segList.push_back(segment);
	}

	if (segList.size() != 7) {
		if (segList.size() != 6) {/*
			std::wstring data;
			data = std::wstring(L"Invalid note data, " + std::to_wstring(segList.size()) + L" segments found.");
			MessageBox::Show(data, L"Sylph Synthesizer");*/
			*error = INVALID_DATA_ERROR;
			return;
		}
		else {
			segList.push_back("0");
		}
	}

	note = (NOTE)atoi(segList[0].c_str());
	measure = atoi(segList[1].c_str());
	measureLength = atof(segList[2].c_str());
	offset = atof(segList[3].c_str());
	float length = atof(segList[4].c_str());
	this->length = length;
	amplitude = atoi(segList[5].c_str());
	selected = false;

	this->extended = false;
	this->followUp = false;

	this->instrument = instrument;

	if (offset + length > measureLength) {
		this->length = measureLength - offset;
		extended = true;
		this->extension = new Note(amplitude, measure + 1, measureLength, 0, length - this->length, note, instrument);
		instrument->addNote(extension);
		this->extension->setPredecessor(this);
	}
}

Note::Note(NoteData data, Instrument* instrument)
{
	this->amplitude = data.amplitude;
	this->offset = data.offset;
	this->length = data.length;
	this->measure = data.measure;
	this->measureLength = data.measureLength;
	this->note = data.note;

	this->extended = false;
	this->followUp = false;

	this->instrument = instrument;

	if (data.offset + data.length > measureLength) {
		this->length = data.measureLength - data.offset;
		extended = true;
		this->extension = new Note(data.amplitude, data.measure + 1, data.measureLength, 0, data.length - this->length, data.note, instrument);
		instrument->addNote(extension);
		this->extension->setPredecessor(this);
	}
}

Note::~Note()
{
}

std::string Note::toString()
{
	//return std::string((note.ToI("d")) + " " + std::to_string(measure) + " " +
		//std::to_string(measureLength) + " " + std::to_string(offset) + " " +
		//std::to_string(length) + " " + std::to_string(amplitude));
	return "";
}

void Note::play(std::vector<SampleFloat_t>* samples, unsigned int sampleRate, float bpm, Instrument* instrument)
{
	float speed = 60.f / bpm;
	float start = (offset)*speed;
	float length = this->length*speed;
	float extendedLength = length;
	if (samples->size() < (start + length)*sampleRate)
		samples->resize((start + length)*sampleRate);
	double increment = translateNote(note) / sampleRate;
	double frequency = translateNote(note);
	double harmonic_2 = translateNote(note)*2.f / sampleRate;
	double harmonic_3 = translateNote(note)*3.f / sampleRate;
	double x = 0;
	double y = 0;
	double z = 0;
	double w = 0;
	float amplitude = this->amplitude;
	float amplitudeOld = amplitude;
	int time = 0;
	int sign = 1;

	WAVE_TYPE wave = instrument->getWaveType();

	if (extended) {
		Note* currentNote = this;
		while (currentNote->isExtended()) {
			currentNote = currentNote->getExtension();
			extendedLength += currentNote->getLength()*speed;
		}
	}

	PlayInfo info = {};

	if (followUp) {
		info = predecessor->getPlayInfo();
		extendedLength = info.extendedLength;
		x = info.x;
		y = info.y;
		z = info.z;
		w = info.w;
		amplitude = info.amplitude;
		amplitudeOld = info.amplitudeOld;
		time = info.time;
		sign = info.sign;
	}

	for (float i = start*sampleRate; i < (int)((start + length)*sampleRate); i++) {
		float point;
		float t;
		if (followUp) {
			t = info.length + ((i - (float)(start*sampleRate)) / (float)sampleRate);
		}
		else {
			t = ((i - (float)(start*sampleRate)) / (float)sampleRate);
		}
		if (t < (extendedLength * 0.9f)) {
			amplitude = amplitudeOld/* * __max(0.7, 1.0 *(1 - (float)time / ((float)sampleRate)))*/;
		}
		else {
			amplitude = __max(amplitudeOld - (amplitudeOld)/(0.1f*extendedLength)*((t-extendedLength*0.9f)), 0.f);
		}
		
		if (wave == WAVE_TYPE::SINE) {
			point = amplitude*0.66f*sin(x*M_PI * 2) + amplitude*0.33f*0.66f*sin(y*M_PI * 2) + amplitude*0.33f*0.33f*sin(z*M_PI * 2);
			/*
			if (sin(x*M_PI * 2) >= 0)
				point = amplitude;
			else
				point = -amplitude;
				*/
			(*samples)[(int)i].left = (point + (*samples)[(int)i].left);
			(*samples)[(int)i].right = (point + (*samples)[(int)i].right);

			x += increment;
			y += harmonic_2;
			z += harmonic_3;
		}
		if (wave == WAVE_TYPE::SAW) {
			float p = sampleRate/frequency;
			point = amplitude*2.f*((float)time / p - floor(0.5f+((float)time / p)));
			(*samples)[(int)i].left = (point + (*samples)[(int)i].left);
			(*samples)[(int)i].right = (point + (*samples)[(int)i].right);
		}
		if (wave == WAVE_TYPE::TRIANGLE) {
			float p = sampleRate / frequency;
			float period = 1.f / frequency;
			point = amplitude * (2.f*abs(2.f*((t - period / 4.f) / period - floor((t - period / 4.f) / period + 0.5f))) - 1.f);
			(*samples)[(int)i].left = (point + (*samples)[(int)i].left);
			(*samples)[(int)i].right = (point + (*samples)[(int)i].right);
		}
		if(wave==WAVE_TYPE::SQUARE){
			point = amplitude*signbit(sin(x*M_PI * 2));
			(*samples)[(int)i].left = (point + (*samples)[(int)i].left);
			(*samples)[(int)i].right = (point + (*samples)[(int)i].right);

			x += increment;
		}
		if (wave == WAVE_TYPE::DRUM_KICK) {
			float t;
			if (followUp) {
				t = ((i - (float)(info.endPoint*sampleRate)) / (float)sampleRate);
			}
			else {
				t = ((i - (float)(start*sampleRate)) / (float)sampleRate);
			}

			point =amplitude * expf(-1.f*t * 4.f) * sinf(2.f*M_PI*t*frequency * expf(-1.f*t*20.f));
			
			(*samples)[(int)i].left = (point + (*samples)[(int)i].left);
			(*samples)[(int)i].right = (point + (*samples)[(int)i].right);

			x += increment;
		}
		if (wave == WAVE_TYPE::DRUM_SNARE) {
			float t;
			if (followUp) {
				t = ((i - (float)(info.endPoint*sampleRate)) / (float)sampleRate);
			}
			else {
				t = ((i - (float)(start*sampleRate)) / (float)sampleRate);
			}

			point = 2 * amplitude * expf(-1.f*t * 4.f) * sinf(2.f*M_PI*t * 180 * expf(-1.f*t*20.f));
			point += 2 * amplitude * expf(-1.f*t * 4.f) * sinf(2.f*M_PI*t * 330 * expf(-1.f*t*20.f));
			float p = sampleRate / 175.f;
			point += expf(-1.f*t*4.f)*(((2.f*amplitude * 2) / p)*(abs(fmodf((float)time, p) - (p / 2.f)) - (p / 4.f)));
			p = sampleRate / 224.f;
			point += expf(-1.f*t*4.f)*(((2.f*amplitude * 2) / p)*(abs(fmodf((float)time, p) - (p / 2.f)) - (p / 4.f)));

			point /= 4;

			(*samples)[(int)i].left = (point + (*samples)[(int)i].left);
			(*samples)[(int)i].right = (point + (*samples)[(int)i].right);

			x += increment;
		}
		if (wave == WAVE_TYPE::REESE_BASS) {
			float detune = powf(2.f, 50.f /4800.f);
			point = sinf(2.f*M_PI*t*frequency) + 0.5f*sinf(2 * M_PI*t*frequency*2.f) + (1.f / 3.f)*sinf(2.f*M_PI*t*frequency*3.f) +
				sinf(2.f*M_PI*t*frequency*detune) + 0.5f*sinf(2 * M_PI*t*frequency*2.f*detune) + (1.f / 3.f)*sinf(2.f*M_PI*t*frequency*3.f*detune);
			point *= amplitude / (3.f + 2.f / 3.f);

			ADD_SAMPLE_LEFT(samples, i, point);
			ADD_SAMPLE_RIGHT(samples, i, point);

			x += increment;
		}
		if (wave == WAVE_TYPE::TEST) {
			point = amplitude*(sin(abs(sinf(x*2.f*M_PI))+x*2.f*M_PI));

			ADD_SAMPLE_LEFT(samples, i, point);
			ADD_SAMPLE_RIGHT(samples, i, point);

			x += increment;
		}
		if (wave == WAVE_TYPE::THEREMIN) {
			float sampleLength = (float)sampleRate / frequency;
			float ratio = 1024.f / sampleLength;

			float location = fmodf(time, sampleLength);
			location *= ratio;
			location = __min(1024.f, __max(0.f, location));
			float data = (float)ThereminTable[(int)location];
			data /= 2048.f;
			data *= amplitude;

			point = (int)data;

			ADD_SAMPLE_LEFT(samples, i, point);
			ADD_SAMPLE_RIGHT(samples, i, point);

			x += increment;
		}
		if (wave == WAVE_TYPE::CELLO) {
			float sampleLength = (float)sampleRate / frequency;
			float ratio = 1024.f / sampleLength;

			float location = fmodf(time, sampleLength);
			location *= ratio;
			location = __min(1024.f, __max(0.f, location));
			float data = (float)PhoenixTable[(int)location];
			data /= 2048.f;
			data *= amplitude;

			point = (int)data;

			ADD_SAMPLE_LEFT(samples, i, point);
			ADD_SAMPLE_RIGHT(samples, i, point);

		}
		time++;
	}
	info = {};
	info.amplitude = amplitude;
	info.amplitudeOld = amplitudeOld;
	info.endPoint = start + length;
	info.extendedLength = extendedLength;
	info.frequency = frequency;
	info.harmonic_2 = harmonic_2;
	info.harmonic_3 = harmonic_3;
	info.increment = increment;
	info.length = length;
	info.sign = sign;
	info.speed = speed;
	info.start = start;
	info.time = time;
	info.w = w;
	info.x = x;
	info.y = y;
	info.z = z;
	this->playInfo = info;
}

void Note::edit(int amplitude, int measure, float measureLength, float offset, float length, NOTE note)
{
	this->amplitude = amplitude;
	this->measure = measure;
	this->measureLength = measureLength;
	this->offset = offset;
	this->length = length;
	this->note = note;

	if (extended) {
		std::vector<Note*> extensions;
		Note* currentNote = this;
		while (currentNote->isExtended()) {
			currentNote = currentNote->getExtension();
			extensions.push_back(currentNote);
		}
		for (int i = 0; i < extensions.size(); ++i) {
			instrument->removeNote(extensions[i]);
		}
		if (offset + length > measureLength) {
			this->length = measureLength - offset;
			extended = true;
			this->extension = new Note(amplitude, measure + 1, measureLength, 0, length - this->length, note, instrument);
			instrument->addNote(extension);
			this->extension->setPredecessor(this);
		}
	}
}

void Note::setInstrument(Instrument * instrument)
{
	this->instrument = instrument;
}

Instrument * Note::getInstrument()
{
	return this->instrument;
}

void Note::setPredecessor(Note * note)
{
	this->predecessor = note;
	this->followUp = note != nullptr ? true : false;
}

void Note::setExtension(Note * note)
{
	extension = note;
	extended = note != nullptr ? true : false;
}

Note::NoteData Note::getNoteData()
{
	NoteData data = {};
	data.amplitude = amplitude;
	data.length = length;
	data.measure = measure;
	data.measureLength = measureLength;
	data.note = note;
	data.offset = offset;
	data.followUp = followUp;
	
	Note* next = this;
	while (next->isExtended()) {
		next = next->getExtension();
		data.length += next->getLength();
	}

	return data;
}

Note::PlayInfo Note::getPlayInfo()
{
	return playInfo;
}
