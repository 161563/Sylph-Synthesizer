#pragma once

#include "Song.h"

using namespace System::Windows::Forms;

ref class NoteCanvas :
	public Panel
{
public:
	NoteCanvas();

	void setSong(Song *song) { this->song = song; };

private:
	Song* song;

};

