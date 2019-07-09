#pragma once

#include "Song.h"
#include "Music.h"
#include "Instrument.h"
#include "CopyMeasures.h"
#include "EquilizeAmplitude.h"
#include "TransposeMeasure.h"
#include "AudioPlayer.h"
#include "VulkanRenderer.h"

#include <msclr\marshal_cppstd.h>
#include <stack>
namespace Synthesizer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainScreen
	/// </summary>
	public ref class MainScreen : public System::Windows::Forms::Form
	{
	public:

		Song* song;
		Note* selectedNote;
		AudioPlayer* audioPlayer;
		renderer::vulkan::VulkanRenderer* renderer;

		ThreadManager^ threadManager;

	private: System::Windows::Forms::Button^  deleteNote;
	private: System::Windows::Forms::Button^  editNote;
	private: System::Windows::Forms::CheckBox^  IncreaseOffsetCheckBox;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Label^  ProgressBarOperationIndicator;

	private: System::Windows::Forms::ToolStripMenuItem^  utilitiesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  copyMeasuresToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  equilizeAmplitudeToolStripMenuItem;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::ToolStripMenuItem^  playFromCurrentMeasureToolStripMenuItem;
	private: System::Windows::Forms::ComboBox^  NoteTypeComboBox;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::ToolStripMenuItem^  transposeMeasureToolStripMenuItem;
	private: System::Windows::Forms::TrackBar^  VolumeBar;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  ExtensionIndicator;
	private: System::ComponentModel::BackgroundWorker^  ExportWavWorker;
	public:

	public:
		int measure;
	private: System::ComponentModel::BackgroundWorker^ VulkanRenderUpdate;
	public:
		String^ exportWavFileName;

		void setSong(Song* song) { this->song = song;
		float bpm = song->getBPM();
		BpmBox->Text = System::Convert::ToString(bpm);
		};

		MainScreen(void)
		{
			measure = 1;
			InitializeComponent();

			renderer = new renderer::vulkan::VulkanRenderer(VulkanRenderView);
			
		}
	private: System::Windows::Forms::TextBox^  SampleRateBox;
	public:
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  BPM;
	private: System::Windows::Forms::TextBox^  BpmBox;
	private: System::Windows::Forms::TextBox^  MeasureBox;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  MeasureGoTo;
	private: System::Windows::Forms::Button^  MeasurePrev;
	private: System::Windows::Forms::Button^  MeasureNext;
	private: System::Windows::Forms::TextBox^  MeasureLengthBox;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  OffsetBox;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  LengthBox;
	private: System::Windows::Forms::ComboBox^  NoteComboBox;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::TextBox^  AmplitudeBox;
	private: System::Windows::Forms::Panel^ VulkanRenderView;




	private: System::Windows::Forms::Label^  label6;


	protected:
		/// <summary>

		/// </summary>
		~MainScreen()
		{
			if (components)
			{
				if (song) {
					threadManager->CloseAll();
					delete song;
				}
				delete renderer;
				delete audioPlayer;
				delete components;
			}
		}

		bool saveDialog() {
			if (!song->isSaved()) {

			save:
				System::Windows::Forms::DialogResult result;
				result = MessageBox::Show(L"The current song has not been saved, do you want to save now?", L"Sylph Synthesizer",
					MessageBoxButtons::YesNoCancel, MessageBoxIcon::Warning);

				if (result == System::Windows::Forms::DialogResult::Yes) {
					if (song->hasSaveLocation())
						song->save(song->getSaveLocation());
					else {
						SaveFileDialog^ dialog = gcnew SaveFileDialog();
						dialog->Filter = "Note File|*.not|All Files|*.*";
						dialog->Title = "Save a note file";
						dialog->FileName = "";
						dialog->ShowDialog();
						if (dialog->FileName != "") {
							song->save(msclr::interop::marshal_as<std::string>(dialog->FileName));
							threadManager->CloseAll();
							delete song;
							song = new Song();
							MeasureBox->Text = L"0";
							measure = 0;
							delete dialog;
						}
						else {
							goto save;
						}
					}
				}
				else if (result == System::Windows::Forms::DialogResult::No) {
					threadManager->CloseAll();
					delete song;
					song = new Song();
					MeasureBox->Text = L"1";
					measure = 1;
				}
				else {
					return false;
				}
			}
			else {
				song->stopPlay(threadManager);
				threadManager->CloseAll();
				delete song;
				song = new Song();
				threadManager = gcnew ThreadManager();
				MeasureBox->Text = L"1";
				measure = 1;
			}
			return true;
		};
		
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected:
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  newToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^  saveAsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exportToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  wavToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  songToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  playToolStripMenuItem;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->newToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exportToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->wavToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->songToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->playToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->playFromCurrentMeasureToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->utilitiesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->copyMeasuresToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->equilizeAmplitudeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->transposeMeasureToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->SampleRateBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->BPM = (gcnew System::Windows::Forms::Label());
			this->BpmBox = (gcnew System::Windows::Forms::TextBox());
			this->MeasureBox = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->MeasureGoTo = (gcnew System::Windows::Forms::Button());
			this->MeasurePrev = (gcnew System::Windows::Forms::Button());
			this->MeasureNext = (gcnew System::Windows::Forms::Button());
			this->MeasureLengthBox = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->OffsetBox = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->LengthBox = (gcnew System::Windows::Forms::TextBox());
			this->NoteComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->AmplitudeBox = (gcnew System::Windows::Forms::TextBox());
			this->VulkanRenderView = (gcnew System::Windows::Forms::Panel());
			this->deleteNote = (gcnew System::Windows::Forms::Button());
			this->editNote = (gcnew System::Windows::Forms::Button());
			this->IncreaseOffsetCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->ProgressBarOperationIndicator = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->NoteTypeComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->VolumeBar = (gcnew System::Windows::Forms::TrackBar());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->ExtensionIndicator = (gcnew System::Windows::Forms::Label());
			this->ExportWavWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->VulkanRenderUpdate = (gcnew System::ComponentModel::BackgroundWorker());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->VolumeBar))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->fileToolStripMenuItem,
					this->songToolStripMenuItem, this->utilitiesToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1183, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->newToolStripMenuItem,
					this->openToolStripMenuItem, this->saveToolStripMenuItem, this->saveAsToolStripMenuItem, this->exportToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// newToolStripMenuItem
			// 
			this->newToolStripMenuItem->Name = L"newToolStripMenuItem";
			this->newToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::N));
			this->newToolStripMenuItem->Size = System::Drawing::Size(177, 22);
			this->newToolStripMenuItem->Text = L"New";
			this->newToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainScreen::New_OnClick);
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->openToolStripMenuItem->Size = System::Drawing::Size(177, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainScreen::Open_OnClick);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
			this->saveToolStripMenuItem->Size = System::Drawing::Size(177, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainScreen::Save_OnClick);
			// 
			// saveAsToolStripMenuItem
			// 
			this->saveAsToolStripMenuItem->Name = L"saveAsToolStripMenuItem";
			this->saveAsToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Alt)
				| System::Windows::Forms::Keys::S));
			this->saveAsToolStripMenuItem->Size = System::Drawing::Size(177, 22);
			this->saveAsToolStripMenuItem->Text = L"Save As";
			this->saveAsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainScreen::SaveAs_OnClick);
			// 
			// exportToolStripMenuItem
			// 
			this->exportToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->wavToolStripMenuItem });
			this->exportToolStripMenuItem->Name = L"exportToolStripMenuItem";
			this->exportToolStripMenuItem->Size = System::Drawing::Size(177, 22);
			this->exportToolStripMenuItem->Text = L"Export";
			// 
			// wavToolStripMenuItem
			// 
			this->wavToolStripMenuItem->Name = L"wavToolStripMenuItem";
			this->wavToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::E));
			this->wavToolStripMenuItem->Size = System::Drawing::Size(140, 22);
			this->wavToolStripMenuItem->Text = L".Wav";
			this->wavToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainScreen::ExportWav_OnClick);
			// 
			// songToolStripMenuItem
			// 
			this->songToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->playToolStripMenuItem,
					this->playFromCurrentMeasureToolStripMenuItem
			});
			this->songToolStripMenuItem->Name = L"songToolStripMenuItem";
			this->songToolStripMenuItem->Size = System::Drawing::Size(46, 20);
			this->songToolStripMenuItem->Text = L"Song";
			// 
			// playToolStripMenuItem
			// 
			this->playToolStripMenuItem->Name = L"playToolStripMenuItem";
			this->playToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::R));
			this->playToolStripMenuItem->Size = System::Drawing::Size(291, 22);
			this->playToolStripMenuItem->Text = L"Play";
			this->playToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainScreen::Play_OnClick);
			// 
			// playFromCurrentMeasureToolStripMenuItem
			// 
			this->playFromCurrentMeasureToolStripMenuItem->Name = L"playFromCurrentMeasureToolStripMenuItem";
			this->playFromCurrentMeasureToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Shift)
				| System::Windows::Forms::Keys::R));
			this->playFromCurrentMeasureToolStripMenuItem->Size = System::Drawing::Size(291, 22);
			this->playFromCurrentMeasureToolStripMenuItem->Text = L"Play From Current Measure";
			this->playFromCurrentMeasureToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainScreen::PlayFromMeasureOnClick);
			// 
			// utilitiesToolStripMenuItem
			// 
			this->utilitiesToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->copyMeasuresToolStripMenuItem,
					this->equilizeAmplitudeToolStripMenuItem, this->transposeMeasureToolStripMenuItem
			});
			this->utilitiesToolStripMenuItem->Name = L"utilitiesToolStripMenuItem";
			this->utilitiesToolStripMenuItem->Size = System::Drawing::Size(58, 20);
			this->utilitiesToolStripMenuItem->Text = L"Utilities";
			// 
			// copyMeasuresToolStripMenuItem
			// 
			this->copyMeasuresToolStripMenuItem->Name = L"copyMeasuresToolStripMenuItem";
			this->copyMeasuresToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::C));
			this->copyMeasuresToolStripMenuItem->Size = System::Drawing::Size(218, 22);
			this->copyMeasuresToolStripMenuItem->Text = L"Copy Measures";
			this->copyMeasuresToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainScreen::CopyMeasures_OnClick);
			// 
			// equilizeAmplitudeToolStripMenuItem
			// 
			this->equilizeAmplitudeToolStripMenuItem->Name = L"equilizeAmplitudeToolStripMenuItem";
			this->equilizeAmplitudeToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::A));
			this->equilizeAmplitudeToolStripMenuItem->Size = System::Drawing::Size(218, 22);
			this->equilizeAmplitudeToolStripMenuItem->Text = L"Equalize Amplitude";
			this->equilizeAmplitudeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainScreen::onClick);
			// 
			// transposeMeasureToolStripMenuItem
			// 
			this->transposeMeasureToolStripMenuItem->Name = L"transposeMeasureToolStripMenuItem";
			this->transposeMeasureToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::T));
			this->transposeMeasureToolStripMenuItem->Size = System::Drawing::Size(218, 22);
			this->transposeMeasureToolStripMenuItem->Text = L"Transpose Measure";
			this->transposeMeasureToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainScreen::TransposeMeasure_onClick);
			// 
			// SampleRateBox
			// 
			this->SampleRateBox->Location = System::Drawing::Point(60, 167);
			this->SampleRateBox->Name = L"SampleRateBox";
			this->SampleRateBox->Size = System::Drawing::Size(100, 20);
			this->SampleRateBox->TabIndex = 1;
			this->SampleRateBox->Text = L"44100";
			this->SampleRateBox->TextChanged += gcnew System::EventHandler(this, &MainScreen::onSampleRateChange);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(60, 148);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(68, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Sample Rate";
			// 
			// BPM
			// 
			this->BPM->AutoSize = true;
			this->BPM->Location = System::Drawing::Point(60, 194);
			this->BPM->Name = L"BPM";
			this->BPM->Size = System::Drawing::Size(30, 13);
			this->BPM->TabIndex = 3;
			this->BPM->Text = L"BPM";
			// 
			// BpmBox
			// 
			this->BpmBox->Location = System::Drawing::Point(60, 211);
			this->BpmBox->Name = L"BpmBox";
			this->BpmBox->Size = System::Drawing::Size(100, 20);
			this->BpmBox->TabIndex = 4;
			this->BpmBox->Text = L"120";
			this->BpmBox->TextChanged += gcnew System::EventHandler(this, &MainScreen::OnBPMChange);
			// 
			// MeasureBox
			// 
			this->MeasureBox->Location = System::Drawing::Point(63, 326);
			this->MeasureBox->Name = L"MeasureBox";
			this->MeasureBox->Size = System::Drawing::Size(100, 20);
			this->MeasureBox->TabIndex = 5;
			this->MeasureBox->Text = L"1";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(60, 307);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(48, 13);
			this->label2->TabIndex = 6;
			this->label2->Text = L"Measure";
			// 
			// MeasureGoTo
			// 
			this->MeasureGoTo->Location = System::Drawing::Point(169, 323);
			this->MeasureGoTo->Name = L"MeasureGoTo";
			this->MeasureGoTo->Size = System::Drawing::Size(75, 23);
			this->MeasureGoTo->TabIndex = 7;
			this->MeasureGoTo->Text = L"Go To";
			this->MeasureGoTo->UseVisualStyleBackColor = true;
			this->MeasureGoTo->Click += gcnew System::EventHandler(this, &MainScreen::MeasureGoTo_OnClick);
			// 
			// MeasurePrev
			// 
			this->MeasurePrev->Location = System::Drawing::Point(63, 352);
			this->MeasurePrev->Name = L"MeasurePrev";
			this->MeasurePrev->Size = System::Drawing::Size(48, 23);
			this->MeasurePrev->TabIndex = 8;
			this->MeasurePrev->Text = L"prev";
			this->MeasurePrev->UseVisualStyleBackColor = true;
			this->MeasurePrev->Click += gcnew System::EventHandler(this, &MainScreen::MeasurePrev_OnClick);
			// 
			// MeasureNext
			// 
			this->MeasureNext->Location = System::Drawing::Point(117, 352);
			this->MeasureNext->Name = L"MeasureNext";
			this->MeasureNext->Size = System::Drawing::Size(46, 23);
			this->MeasureNext->TabIndex = 9;
			this->MeasureNext->Text = L"next";
			this->MeasureNext->UseVisualStyleBackColor = true;
			this->MeasureNext->Click += gcnew System::EventHandler(this, &MainScreen::MeasureNext_OnClick);
			// 
			// MeasureLengthBox
			// 
			this->MeasureLengthBox->Location = System::Drawing::Point(63, 284);
			this->MeasureLengthBox->Name = L"MeasureLengthBox";
			this->MeasureLengthBox->Size = System::Drawing::Size(100, 20);
			this->MeasureLengthBox->TabIndex = 10;
			this->MeasureLengthBox->Text = L"4";
			this->MeasureLengthBox->TextChanged += gcnew System::EventHandler(this, &MainScreen::OnMeasureLengthChange);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(60, 268);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(166, 13);
			this->label3->TabIndex = 11;
			this->label3->Text = L"Measure Length (in quarter notes)";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(60, 383);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(143, 13);
			this->label4->TabIndex = 12;
			this->label4->Text = L"Note Offset (in quarter notes)";
			// 
			// OffsetBox
			// 
			this->OffsetBox->Location = System::Drawing::Point(63, 399);
			this->OffsetBox->Name = L"OffsetBox";
			this->OffsetBox->Size = System::Drawing::Size(100, 20);
			this->OffsetBox->TabIndex = 13;
			this->OffsetBox->Text = L"0";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(60, 422);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(148, 13);
			this->label5->TabIndex = 14;
			this->label5->Text = L"Note Length (in quarter notes)";
			// 
			// LengthBox
			// 
			this->LengthBox->Location = System::Drawing::Point(63, 438);
			this->LengthBox->Name = L"LengthBox";
			this->LengthBox->Size = System::Drawing::Size(100, 20);
			this->LengthBox->TabIndex = 15;
			this->LengthBox->Text = L"1";
			// 
			// NoteComboBox
			// 
			this->NoteComboBox->Location = System::Drawing::Point(63, 516);
			this->NoteComboBox->Name = L"NoteComboBox";
			this->NoteComboBox->Size = System::Drawing::Size(121, 21);
			this->NoteComboBox->TabIndex = 16;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(60, 500);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(107, 13);
			this->label6->TabIndex = 17;
			this->label6->Text = L"Note (C4 is central C)";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(63, 543);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 18;
			this->button1->Text = L"Add Note";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainScreen::AddNote_OnClick);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(60, 461);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(306, 13);
			this->label7->TabIndex = 19;
			this->label7->Text = L"Amplitude (shouldn\'t reach above 30000 for all notes combined)";
			// 
			// AmplitudeBox
			// 
			this->AmplitudeBox->Location = System::Drawing::Point(63, 477);
			this->AmplitudeBox->Name = L"AmplitudeBox";
			this->AmplitudeBox->Size = System::Drawing::Size(100, 20);
			this->AmplitudeBox->TabIndex = 20;
			this->AmplitudeBox->Text = L"10000";
			// 
			// VulkanRenderView
			// 
			this->VulkanRenderView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->VulkanRenderView->Location = System::Drawing::Point(372, 28);
			this->VulkanRenderView->Name = L"VulkanRenderView";
			this->VulkanRenderView->Size = System::Drawing::Size(801, 541);
			this->VulkanRenderView->TabIndex = 21;
			this->VulkanRenderView->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MainScreen::Panel_OnClick);
			// 
			// deleteNote
			// 
			this->deleteNote->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->deleteNote->Enabled = false;
			this->deleteNote->Location = System::Drawing::Point(372, 573);
			this->deleteNote->Name = L"deleteNote";
			this->deleteNote->Size = System::Drawing::Size(75, 23);
			this->deleteNote->TabIndex = 22;
			this->deleteNote->Text = L"Delete Note";
			this->deleteNote->UseVisualStyleBackColor = true;
			this->deleteNote->Click += gcnew System::EventHandler(this, &MainScreen::deleteNoteOnClick);
			// 
			// editNote
			// 
			this->editNote->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->editNote->Enabled = false;
			this->editNote->Location = System::Drawing::Point(453, 573);
			this->editNote->Name = L"editNote";
			this->editNote->Size = System::Drawing::Size(75, 23);
			this->editNote->TabIndex = 23;
			this->editNote->Text = L"Edit Note";
			this->editNote->UseVisualStyleBackColor = true;
			this->editNote->Click += gcnew System::EventHandler(this, &MainScreen::editNoteOnClick);
			// 
			// IncreaseOffsetCheckBox
			// 
			this->IncreaseOffsetCheckBox->AutoSize = true;
			this->IncreaseOffsetCheckBox->Location = System::Drawing::Point(188, 402);
			this->IncreaseOffsetCheckBox->Name = L"IncreaseOffsetCheckBox";
			this->IncreaseOffsetCheckBox->Size = System::Drawing::Size(178, 17);
			this->IncreaseOffsetCheckBox->TabIndex = 24;
			this->IncreaseOffsetCheckBox->Text = L"increase offset after adding note";
			this->IncreaseOffsetCheckBox->UseVisualStyleBackColor = true;
			// 
			// progressBar1
			// 
			this->progressBar1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->progressBar1->Location = System::Drawing::Point(587, 573);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(584, 23);
			this->progressBar1->Step = 1;
			this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBar1->TabIndex = 25;
			this->progressBar1->UseWaitCursor = true;
			// 
			// ProgressBarOperationIndicator
			// 
			this->ProgressBarOperationIndicator->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->ProgressBarOperationIndicator->AutoSize = true;
			this->ProgressBarOperationIndicator->Location = System::Drawing::Point(534, 578);
			this->ProgressBarOperationIndicator->Name = L"ProgressBarOperationIndicator";
			this->ProgressBarOperationIndicator->Size = System::Drawing::Size(47, 13);
			this->ProgressBarOperationIndicator->TabIndex = 26;
			this->ProgressBarOperationIndicator->Text = L"progress";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(13, 28);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 27;
			this->button2->Text = L"Cancel song";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MainScreen::PlayCancel_OnClick);
			// 
			// NoteTypeComboBox
			// 
			this->NoteTypeComboBox->FormattingEnabled = true;
			this->NoteTypeComboBox->Location = System::Drawing::Point(188, 516);
			this->NoteTypeComboBox->Name = L"NoteTypeComboBox";
			this->NoteTypeComboBox->Size = System::Drawing::Size(121, 21);
			this->NoteTypeComboBox->TabIndex = 28;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(188, 500);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(57, 13);
			this->label9->TabIndex = 29;
			this->label9->Text = L"Note Type";
			// 
			// VolumeBar
			// 
			this->VolumeBar->LargeChange = 10;
			this->VolumeBar->Location = System::Drawing::Point(63, 100);
			this->VolumeBar->Maximum = 100;
			this->VolumeBar->Name = L"VolumeBar";
			this->VolumeBar->Size = System::Drawing::Size(250, 45);
			this->VolumeBar->TabIndex = 30;
			this->VolumeBar->TickFrequency = 10;
			this->VolumeBar->ValueChanged += gcnew System::EventHandler(this, &MainScreen::VolumeChanged);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(63, 81);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(42, 13);
			this->label10->TabIndex = 31;
			this->label10->Text = L"Volume";
			// 
			// ExtensionIndicator
			// 
			this->ExtensionIndicator->AutoSize = true;
			this->ExtensionIndicator->Location = System::Drawing::Point(169, 441);
			this->ExtensionIndicator->Name = L"ExtensionIndicator";
			this->ExtensionIndicator->Size = System::Drawing::Size(53, 13);
			this->ExtensionIndicator->TabIndex = 32;
			this->ExtensionIndicator->Text = L"Extension";
			this->ExtensionIndicator->Visible = false;
			// 
			// ExportWavWorker
			// 
			this->ExportWavWorker->WorkerReportsProgress = true;
			this->ExportWavWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainScreen::ExportWavAsync);
			this->ExportWavWorker->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &MainScreen::OnProgressChange);
			this->ExportWavWorker->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &MainScreen::OnExportComplete);
			// 
			// VulkanRenderUpdate
			// 
			this->VulkanRenderUpdate->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainScreen::VulkanUpdate);
			// 
			// MainScreen
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1183, 601);
			this->Controls->Add(this->ExtensionIndicator);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->VolumeBar);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->NoteTypeComboBox);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->ProgressBarOperationIndicator);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->IncreaseOffsetCheckBox);
			this->Controls->Add(this->editNote);
			this->Controls->Add(this->deleteNote);
			this->Controls->Add(this->VulkanRenderView);
			this->Controls->Add(this->AmplitudeBox);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->NoteComboBox);
			this->Controls->Add(this->LengthBox);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->OffsetBox);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->MeasureLengthBox);
			this->Controls->Add(this->MeasureNext);
			this->Controls->Add(this->MeasurePrev);
			this->Controls->Add(this->MeasureGoTo);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->MeasureBox);
			this->Controls->Add(this->BpmBox);
			this->Controls->Add(this->BPM);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->SampleRateBox);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MainScreen";
			this->Text = L"Sylph Synthesizer";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainScreen::onClose);
			this->Load += gcnew System::EventHandler(this, &MainScreen::onLoad);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->VolumeBar))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void New_OnClick(System::Object^  sender, System::EventArgs^  e) {
		if (song) {
			saveDialog();
		}
		else {
			song = new Song();
			this->Text = L"Sylph Synthesizer";
			MeasureBox->Text = L"1";
			measure = 1;
			try {
				song->setSampleRate(int::Parse(SampleRateBox->Text));
				song->setBPM(float::Parse(BpmBox->Text));
				song->setMeasureLength(float::Parse(MeasureLengthBox->Text));
			}
			catch (...) {
				song->setSampleRate(44100);
				song->setBPM(100.f);
				song->setMeasureLength(4.f);
				SampleRateBox->Text = L"44100";
				BpmBox->Text = L"100";
				MeasureLengthBox->Text = L"4";
			}

			threadManager = gcnew ThreadManager();
		}
	}
private: System::Void Save_OnClick(System::Object^  sender, System::EventArgs^  e) {
	if (song) {
		if (song->hasSaveLocation())
			song->save(song->getSaveLocation());
		else {
			SaveFileDialog^ dialog = gcnew SaveFileDialog();
			dialog->Filter = "Sylph File|*.syl|All Files|*.*";
			dialog->Title = "Save a sylph file";
			dialog->FileName = "";
			dialog->ShowDialog();
			if (dialog->FileName != "") {
				song->save(msclr::interop::marshal_as<std::string>(dialog->FileName));
				this->Text = L"Sylph Synthesizer - " + dialog->FileName;
				delete dialog;
			}
		}
	}
}
private: System::Void SaveAs_OnClick(System::Object^  sender, System::EventArgs^  e) {
	if (song) {
		SaveFileDialog^ dialog = gcnew SaveFileDialog();
		dialog->Filter = "Sylph File|*.syl|All Files|*.*";
		dialog->Title = "Save a sylph file";
		dialog->FileName = "";
		dialog->ShowDialog();
		if (dialog->FileName != "") {
			song->save(msclr::interop::marshal_as<std::string>(dialog->FileName));
			this->Text = L"Sylph Synthesizer - " + dialog->FileName;
			delete dialog;
		}
	}
}
private: System::Void Open_OnClick(System::Object^  sender, System::EventArgs^  e) {
	if (song) {
		if (!song->isSaved()) {

		save:
			System::Windows::Forms::DialogResult result;
			result = MessageBox::Show(L"The current song has not been saved, do you want to save now?", L"Sylph Synthesizer",
				MessageBoxButtons::YesNoCancel, MessageBoxIcon::Warning);

			if (result == System::Windows::Forms::DialogResult::Yes) {
				if (song->hasSaveLocation()) {
					song->save(song->getSaveLocation());
					threadManager->CloseAll();
					delete song;
				}
				else {
					SaveFileDialog^ dialog = gcnew SaveFileDialog();
					dialog->Filter = "sylph File|*.syl|All Files|*.*";
					dialog->Title = "Save a sylph file";
					dialog->FileName = "";
					dialog->ShowDialog();
					if (dialog->FileName != "") {
						song->save(msclr::interop::marshal_as<std::string>(dialog->FileName));
						threadManager->CloseAll();
						delete song;
						delete dialog;
					}
					else {
						goto save;
					}
				}
			}
			else if (result == System::Windows::Forms::DialogResult::No) {
				threadManager->CloseAll();
				delete song;
			}
			else {
				return;
			}
		}
		else {
			threadManager->CloseAll();
			delete song;
		}
		OpenFileDialog^ dialog = gcnew OpenFileDialog();
		dialog->Filter = "Sylph File|*.syl|Note File|*.not|All Files|*.*";
		dialog->Title = "Select a sylph file";
		dialog->FileName = "";
		dialog->ShowDialog();
		if (dialog->FileName != "") {
			song = new Song(msclr::interop::marshal_as<std::string>(dialog->FileName));
			this->Text = L"Sylph Synthesizer - " + dialog->FileName;
			float bpm = song->getBPM();
			int sampleRate = song->getSampleRate();
			BpmBox->Text = System::Convert::ToString(bpm);
			SampleRateBox->Text = System::Convert::ToString(sampleRate);
			MeasureLengthBox->Text = System::Convert::ToString(song->getMeasureLength());
			MeasureBox->Text = L"1";
			measure = 1;
			//threadManager->CloseAll();

			threadManager = gcnew ThreadManager();
		}
	}
}
private: System::Void ExportWav_OnClick(System::Object^  sender, System::EventArgs^  e) {
	if (song) {
		SaveFileDialog^ dialog = gcnew SaveFileDialog();
		dialog->Filter = "Wave File|*.wav|All Files|*.*";
		dialog->Title = "Select a note file";
		dialog->FileName = "";
		dialog->ShowDialog();
		if (dialog->FileName != "") {
			exportWavFileName = dialog->FileName;
			ExportWavWorker->RunWorkerAsync();
		}
	}
}
private: System::Void Play_OnClick(System::Object^  sender, System::EventArgs^  e) {
	if (song) {
		try {
			song->play(audioPlayer, threadManager, (unsigned int)int::Parse(SampleRateBox->Text),
				float::Parse(BpmBox->Text), 1, float::Parse(MeasureLengthBox->Text));
		}
		catch (...) {
			MessageBox::Show(L"One of the info boxes contained invalid data.", L"Sylph Synthesizer");
		}
	}
}
private: System::Void onLoad(System::Object^  sender, System::EventArgs^  e) {
	this->NoteComboBox->DataSource = Enum::GetValues(NOTE::typeid);
	this->NoteComboBox->SelectedItem = NOTE::C4;
	this->NoteTypeComboBox->DataSource = Enum::GetValues(WAVE_TYPE::typeid);

	this->audioPlayer = new AudioPlayer();

	this->threadManager = gcnew ThreadManager();

	float volume = map(this->audioPlayer->ListenerGetGain(),
		this->audioPlayer->ListenerGetMinGain(),
		this->audioPlayer->ListenerGetMaxGain(),
		this->VolumeBar->Minimum,
		this->VolumeBar->Maximum);

	this->VolumeBar->Value = (int)volume;

	if (song) {
		if (song->hasSaveLocation()) {
			MeasureLengthBox->Text = System::Convert::ToString(song->getMeasureLength());
			SampleRateBox->Text = System::Convert::ToString(song->getSampleRate());
			BpmBox->Text = System::Convert::ToString(song->getBPM());
			this->Text = L"Sylph Synthesizer - " + gcnew String(song->getSaveLocation().c_str());
		}
	}
}
private: System::Void AddNote_OnClick(System::Object^  sender, System::EventArgs^  e) {
	NOTE note = (NOTE)NoteComboBox->SelectedItem;
	WAVE_TYPE wave = (WAVE_TYPE)NoteTypeComboBox->SelectedItem;
	Instrument* instrument = nullptr;
	std::vector<Instrument*> instruments = song->getInstruments();
	for (int i = 0; i < instruments.size(); ++i) {
		if (instruments[i]->getWaveType() == wave) {
			instrument = instruments[i];
			break;
		}
	}
	if (instrument == nullptr) {
		instrument = song->createInstrument();
		instrument->setWaveType(wave);
	}

	int amplitude;
	int measure;
	float measureLength;
	float offset;
	float length;

	try {
		amplitude = int::Parse(AmplitudeBox->Text);
		measure = int::Parse(MeasureBox->Text);
		measureLength = float::Parse(MeasureLengthBox->Text);
		offset = float::Parse(OffsetBox->Text);
		length = float::Parse(LengthBox->Text);
	}
	catch (...) {
		return;
	}
	while (offset >= measureLength) {
		offset -= measureLength;
		measure++;
	}
	instrument->addNote(amplitude, measure,
		measureLength, offset, length, note);
	if (IncreaseOffsetCheckBox->Checked) {
		OffsetBox->Text = System::Convert::ToString(length + offset);
	}
}
private: System::Void MeasureGoTo_OnClick(System::Object^  sender, System::EventArgs^  e) {
	measure = int::Parse(MeasureBox->Text);
	if (measure <= 0) {
		measure = 1;
		MeasureBox->Text = System::Convert::ToString(1);
	}
}
private: System::Void MeasurePrev_OnClick(System::Object^  sender, System::EventArgs^  e) {
	int measure = int::Parse(MeasureBox->Text);
	measure--;
	MeasureBox->Text = System::Convert::ToString(measure);
	MeasureGoTo_OnClick(sender, e);
}
	private: System::Void MeasureNext_OnClick(System::Object^ sender, System::EventArgs^ e) {
		int measure = int::Parse(MeasureBox->Text);
		measure++;
		MeasureBox->Text = System::Convert::ToString(measure);
		MeasureGoTo_OnClick(sender, e);
	}

//	private: System::Void Panel_OnPaint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
//		//MessageBox::Show(L"painting");
//		std::vector<Instrument*> instruments = song->getInstruments();
//		Graphics^ g = e->Graphics;
//		for (int i = 0; i <= (int)NOTE::B8; i++) {
//			g->DrawLine(System::Drawing::Pens::Black, panel1->Width / ((float)NOTE::B8 + 1)*i,
//				(float)0, panel1->Width / ((float)NOTE::B8 + 1)*i, (float)panel1->Height);
//		}
//		for (int i = 0; i < (int)NOTE::B8; i += (int)NOTE::C1) {
//			System::Drawing::Brush^ brush = System::Drawing::Brushes::Black;
//			g->FillRectangle(brush, (float)(i + 1)*panel1->Width / ((float)NOTE::B8 + 1),
//				0.f,
//				panel1->Width / ((float)NOTE::B8 + 1),
//				(float)panel1->Height);
//			g->FillRectangle(brush, (float)(i + 3)*panel1->Width / ((float)NOTE::B8 + 1),
//				0.f,
//				panel1->Width / ((float)NOTE::B8 + 1),
//				(float)panel1->Height);
//			g->FillRectangle(brush, (float)(i + 6)*panel1->Width / ((float)NOTE::B8 + 1),
//				0.f,
//				panel1->Width / ((float)NOTE::B8 + 1),
//				(float)panel1->Height);
//			g->FillRectangle(brush, (float)(i + 8)*panel1->Width / ((float)NOTE::B8 + 1),
//				0.f,
//				panel1->Width / ((float)NOTE::B8 + 1),
//				(float)panel1->Height);
//			g->FillRectangle(brush, (float)(i + 10)*panel1->Width / ((float)NOTE::B8 + 1),
//				0.f,
//				panel1->Width / ((float)NOTE::B8 + 1),
//				(float)panel1->Height);
//		}
//		g->DrawLine(System::Drawing::Pens::Black, (float)panel1->Width - 1,
//			(float)0, (float)panel1->Width - 1, (float)panel1->Height);
//
//		for (int i = 0; i < int::Parse(MeasureLengthBox->Text) + 1; i++) {
//			g->DrawLine(System::Drawing::Pens::DarkBlue, 0, panel1->Height / int::Parse(MeasureLengthBox->Text) * i,
//				panel1->Width, panel1->Height / int::Parse(MeasureLengthBox->Text) * i);
//		}
//		for (int i = 0; i < instruments.size(); ++i) {
//			std::vector<Note*> notes = instruments[i]->getNotes();
//			for each (Note* note in notes)
//			{
//				if (note->getMeasure() == measure) {
//					System::Drawing::Brush^ brush;
//					brush = System::Drawing::Brushes::GreenYellow;
//					if (instruments[i]->getWaveType() == WAVE_TYPE::SINE)
//						brush = System::Drawing::Brushes::GreenYellow;
//					if (instruments[i]->getWaveType() == WAVE_TYPE::SAW)
//						brush = System::Drawing::Brushes::CadetBlue;
//					if (instruments[i]->getWaveType() == WAVE_TYPE::TRIANGLE)
//						brush = System::Drawing::Brushes::Red;
//					if (instruments[i]->getWaveType() == WAVE_TYPE::SQUARE)
//						brush = System::Drawing::Brushes::Purple;
//					if (instruments[i]->getWaveType() == WAVE_TYPE::DRUM_KICK) {
//						brush = System::Drawing::Brushes::Cyan;
//					}
//					if (instruments[i]->getWaveType() == WAVE_TYPE::DRUM_SNARE) {
//						brush = System::Drawing::Brushes::BlueViolet;
//					}
//					if (instruments[i]->getWaveType() == WAVE_TYPE::REESE_BASS) {
//						brush = System::Drawing::Brushes::Orange;
//					}
//					if (instruments[i]->getWaveType() == WAVE_TYPE::THEREMIN) {
//						brush = System::Drawing::Brushes::DarkGreen;
//					}
//					if (instruments[i]->getWaveType() == WAVE_TYPE::CELLO) {
//						brush = System::Drawing::Brushes::DarkGoldenrod;
//					}
//					g->FillRectangle(brush, (float)note->getNote()*panel1->Width / ((float)NOTE::B8 + 1),
//						panel1->Height - panel1->Height / note->getMeasureLength()*(note->getOffset() + note->getLength()),
//						panel1->Width / ((float)NOTE::B8 + 1),
//						panel1->Height / note->getMeasureLength()*note->getLength());
//					System::Drawing::Pen^ pen = System::Drawing::Pens::DarkBlue;
//					if (note->isSelected()) {
//						pen = System::Drawing::Pens::DarkRed;
//					}
//					g->DrawRectangle(pen, (float)note->getNote()*panel1->Width / ((float)NOTE::B8 + 1),
//						panel1->Height - panel1->Height / note->getMeasureLength()*(note->getOffset() + note->getLength()),
//						panel1->Width / ((float)NOTE::B8 + 1),
//						panel1->Height / note->getMeasureLength()*note->getLength());
//
//				}
//			}
//		}
//	}
private: System::Void Panel_OnClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
	if (selectedNote != nullptr) {
		selectedNote->select(false);
		selectedNote = nullptr;
		deleteNote->Enabled = false;
		editNote->Enabled = false;
		ExtensionIndicator->Visible = false;
	}
	NOTE noteValue = (NOTE)(int)(e->X*((float)NOTE::B8 + 1) / VulkanRenderView->Width);
	float time = (e->Y - (float)VulkanRenderView->Height) / (-(float)VulkanRenderView->Height / float::Parse(MeasureLengthBox->Text));
	std::vector<Instrument*> instruments = song->getInstruments();
	for (int i = 0; i < instruments.size(); ++i) {
		selectedNote = instruments[i]->getNote(noteValue, measure, time);
		if (selectedNote != nullptr) {
			selectedNote->select(true);
			deleteNote->Enabled = true;
			editNote->Enabled = true;
			OffsetBox->Text = System::Convert::ToString(selectedNote->getOffset());
			LengthBox->Text = System::Convert::ToString(selectedNote->getLength());
			AmplitudeBox->Text = System::Convert::ToString(selectedNote->getAmplitude());
			NoteComboBox->SelectedItem = selectedNote->getNote();
			NoteTypeComboBox->SelectedItem = instruments[i]->getWaveType();
			if (selectedNote->hasPredecessor())
				ExtensionIndicator->Visible = true;
			break;
		}
	}
}
private: System::Void deleteNoteOnClick(System::Object^  sender, System::EventArgs^  e) {
	if (selectedNote != nullptr) {
		selectedNote->getInstrument()->removeNote(selectedNote);
		selectedNote = nullptr;
	}
}
private: System::Void editNoteOnClick(System::Object^  sender, System::EventArgs^  e) {
	if (selectedNote != nullptr) {
		NOTE note = (NOTE)NoteComboBox->SelectedItem;
		WAVE_TYPE wave = (WAVE_TYPE)NoteTypeComboBox->SelectedItem;
		selectedNote->edit(int::Parse(AmplitudeBox->Text), int::Parse(MeasureBox->Text),
			float::Parse(MeasureLengthBox->Text), float::Parse(OffsetBox->Text), float::Parse(LengthBox->Text), note);
		if (selectedNote->getInstrument()->getWaveType() != wave) {
			selectedNote->getInstrument()->removeNote(selectedNote);
			Instrument* newInstrument = nullptr;
			for (int i = 0; i < song->getInstruments().size(); ++i) {
				if (song->getInstruments()[i]->getWaveType() == wave) {
					newInstrument = song->getInstruments()[i];
					break;
				}
			}
			if (newInstrument == nullptr) {
				newInstrument = song->createInstrument();
				newInstrument->setWaveType(wave);
			}
			selectedNote = newInstrument->addNote(int::Parse(AmplitudeBox->Text), int::Parse(MeasureBox->Text),
				float::Parse(MeasureLengthBox->Text), float::Parse(OffsetBox->Text), float::Parse(LengthBox->Text), note);
		}
	}
}
private: System::Void onClose(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
	if (!saveDialog()) {
		e->Cancel = true;
	}
	else {
		song->stopPlay(threadManager);
		if (song) {
			threadManager->CloseAll();
			delete song;
		}
		delete renderer;
		delete audioPlayer;
	}
}
private: System::Void CopyMeasures_OnClick(System::Object^  sender, System::EventArgs^  e) {
	CopyMeasures^ copyMeasuresForm = gcnew CopyMeasures();
	if (copyMeasuresForm->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		int from = int::Parse(copyMeasuresForm->MeasureFromTextBox->Text);
		int till = int::Parse(copyMeasuresForm->MeasureTillTextBox->Text);
		int dest = int::Parse(copyMeasuresForm->MeasureDestinationTextBox->Text);
		copyMeasuresForm->MeasureFromTextBox->Text = L"";
		copyMeasuresForm->MeasureTillTextBox->Text = L"";
		copyMeasuresForm->MeasureDestinationTextBox->Text = L"";
		if (from > 0 && till > 0 && dest > 0) {
			if (till > from) {
				for each (Instrument* instrument in song->getInstruments())
				{
					if (copyMeasuresForm->overwriteTextBox->Checked) {
						std::vector<Note*> notes = instrument->getNotes();
						std::stack<Note*> removeStack;
						for each (Note* note in notes)
						{
							if (note->getMeasure() >= dest && note->getMeasure() <= dest + (till - from - 1)) {
								removeStack.push(note);
							}
						}
						while (!removeStack.empty()) {
							instrument->removeNote(removeStack.top());
							removeStack.pop();
						}
					}
					if (copyMeasuresForm->SelectInstrumentCheckBox->Checked) {
						if ((WAVE_TYPE)copyMeasuresForm->Instruments->SelectedItem == instrument->getWaveType()) {
							continue;
						}
					}

					std::vector<Note*> notes = instrument->getNotes();
					for (int i = from; i < till; i++) {
						for each (Note* note in notes)
						{
							if (note->getMeasure() == i) {
								instrument->addNote(note->getAmplitude(), dest + (i - from), note->getMeasureLength(), note->getOffset(), note->getLength(), note->getNote());

							}

						}
					}
				}\
			}
			else {
				MessageBox::Show(L"Till must be larger then from");
			}
		}
		else {
			MessageBox::Show(L"All measures must be larger then 0");
		}
		//copyMeasuresForm->Dispose(true);
		delete copyMeasuresForm;
	}
}
private: System::Void onClick(System::Object^  sender, System::EventArgs^  e) {
	EquilizeAmplitude^ EquilizeDialog = gcnew EquilizeAmplitude();
	if (EquilizeDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		int from = int::Parse(EquilizeDialog->MeasureFromTextBox->Text);
		int till = int::Parse(EquilizeDialog->MeasureTillTextBox->Text);
		int amplitude = int::Parse(EquilizeDialog->AmplitudeTextBox->Text);
		if (amplitude > 0) {
			if (from > 0 && till > 0) {
				if (till > from) {
					for each (Instrument* instrument in song->getInstruments())
					{
						if (EquilizeDialog->SelectInstrumentCheckBox->Checked) {
							if ((WAVE_TYPE)EquilizeDialog->Instrument->SelectedItem != instrument->getWaveType()) {
								continue;
							}
						}
						std::vector<Note*> notes = instrument->getNotes();
						for each (Note* note in notes)
						{
							if (note->getMeasure() >= from && note->getMeasure() < till) {
								note->edit(amplitude, note->getMeasure(), note->getMeasureLength(), note->getOffset(), note->getLength(), note->getNote());
							}
						}
					}
				}
				else {
					MessageBox::Show(L"Till must be larger than From");
				}
			}
			else {
				MessageBox::Show(L"From and Till must be larger than 0");
			}
		}
		else {
			MessageBox::Show(L"amplitude must be larger than 0");
		}
	}
	delete EquilizeDialog;
}
private: System::Void PlayCancel_OnClick(System::Object^  sender, System::EventArgs^  e) {
	song->stopPlay(threadManager);
}
private: System::Void PlayFromMeasureOnClick(System::Object^  sender, System::EventArgs^  e) {
	if (song) {
		song->play(audioPlayer, threadManager, (unsigned int)int::Parse(SampleRateBox->Text),
			float::Parse(BpmBox->Text), measure, float::Parse(MeasureLengthBox->Text));
	}
}
private: System::Void TransposeMeasure_onClick(System::Object^  sender, System::EventArgs^  e) {
	TransposeMeasure^ TransposeDialog = gcnew TransposeMeasure();
	if (TransposeDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		int from = int::Parse(TransposeDialog->StartMeasure->Text);
		int till = int::Parse(TransposeDialog->EndMeasure->Text);
		int transpose = int::Parse(TransposeDialog->Offset->Text);
		if (from > 0 && till > 0) {
			if (till > from) {
				for each (Instrument* instrument in song->getInstruments())
				{
					if (TransposeDialog->SelectInstrumentCheckBox->Checked) {
						if (instrument->getWaveType() != (WAVE_TYPE)TransposeDialog->Instrument->SelectedItem) {
							continue;
						}
					}
					std::vector<Note*> notes = instrument->getNotes();
					for each (Note* note in notes) {
						if (note->getMeasure() >= from && note->getMeasure() < till) {

							int noteValue = (int)note->getNote();

							for (noteValue += transpose; noteValue < 0; noteValue += (int)NOTE::B8);
							for (; noteValue > (int)NOTE::B8; noteValue -= (int)NOTE::B8);
							note->edit(note->getAmplitude(),
								note->getMeasure(),
								note->getMeasureLength(),
								note->getOffset(),
								note->getLength(),
								(NOTE)noteValue);

						}
					}
				}
			}
			else {
				MessageBox::Show(L"End measure must be larger than Start measure");
			}
		}
		else {
			MessageBox::Show(L"Start measure and End measure must be larger than 0");
		}
	}
	delete TransposeDialog;
}
private: System::Void ResizeEnd(System::Object^  sender, System::EventArgs^  e) {
	//panel1->Invalidate();
}
private: System::Void VolumeChanged(System::Object^  sender, System::EventArgs^  e) {
	float volume = map((float)this->VolumeBar->Value,
		(float)this->VolumeBar->Minimum,
		(float)this->VolumeBar->Maximum,
		this->audioPlayer->ListenerGetMinGain(),
		this->audioPlayer->ListenerGetMaxGain());
	this->audioPlayer->ListenerSetGain(volume);
}
private: System::Void OnBPMChange(System::Object^  sender, System::EventArgs^  e) {
	try {
		song->setBPM(float::Parse(BpmBox->Text));
	}
	catch (...) {

	}
	finally{

	}
}
private: System::Void onSampleRateChange(System::Object^  sender, System::EventArgs^  e) {
	try {
		song->setSampleRate(int::Parse(SampleRateBox->Text));
	}
	catch (...) {

	}
	finally{

	}
}
private: System::Void OnMeasureLengthChange(System::Object^  sender, System::EventArgs^  e) {
	try {
		song->setMeasureLength(float::Parse(MeasureLengthBox->Text));
	}
	catch (...) {

	}
}
private: System::Void ExportWavAsync(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
	try {
		System::String^ tempString = exportWavFileName; //Marshal as can't handle class members. Use this as work around.
		song->exportWav(msclr::interop::marshal_as<std::string>(tempString), (unsigned int)int::Parse(SampleRateBox->Text),
			float::Parse(BpmBox->Text), ExportWavWorker);
	}
	catch (...) {

	}
}
private: System::Void OnProgressChange(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) {
	progressBar1->Value = e->ProgressPercentage;
}
private: System::Void OnExportComplete(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) {
	progressBar1->Value = 0;
}
private: System::Void VulkanUpdate(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) {

}
};
}
