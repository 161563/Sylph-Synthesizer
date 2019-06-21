#include "MainScreen.h"
#include "Song.h"
#include "Music.h"

#include <string>
using namespace System;
using namespace System::Windows::Forms;


[STAThread]
void main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	for (int i = 0; i < args->Length; i++) {
		//MessageBox::Show(args[i]);
	}
	Synthesizer::MainScreen screen;
	if (args->Length == 1) {
		screen.setSong(new Song(args[0]));
	}
	else {
		screen.setSong(new Song());
	}
	Application::Run(%screen);
}