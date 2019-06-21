#pragma once

#include "Music.h"

namespace Synthesizer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for EquilizeAmplitude
	/// </summary>
	public ref class EquilizeAmplitude : public System::Windows::Forms::Form
	{
	public:
		EquilizeAmplitude(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EquilizeAmplitude()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	public: System::Windows::Forms::TextBox^  MeasureFromTextBox;
	private:
	public: System::Windows::Forms::TextBox^  MeasureTillTextBox;
	private: System::Windows::Forms::Label^  label4;
	public:
	public: System::Windows::Forms::TextBox^  AmplitudeTextBox;
	private:
	private: System::Windows::Forms::Button^  button1;
	public:
	private: System::Windows::Forms::Button^  button2;
	public: System::Windows::Forms::CheckBox^  SelectInstrumentCheckBox;
	private:
	public: System::Windows::Forms::ComboBox^  Instrument;



	private: System::Windows::Forms::Label^  label5;
	protected:

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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->MeasureFromTextBox = (gcnew System::Windows::Forms::TextBox());
			this->MeasureTillTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->AmplitudeTextBox = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SelectInstrumentCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->Instrument = (gcnew System::Windows::Forms::ComboBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(49, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(48, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Measure";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(13, 30);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(30, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"From";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(13, 50);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(20, 13);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Till";
			// 
			// MeasureFromTextBox
			// 
			this->MeasureFromTextBox->Location = System::Drawing::Point(50, 26);
			this->MeasureFromTextBox->Name = L"MeasureFromTextBox";
			this->MeasureFromTextBox->Size = System::Drawing::Size(100, 20);
			this->MeasureFromTextBox->TabIndex = 3;
			this->MeasureFromTextBox->Text = L"1";
			// 
			// MeasureTillTextBox
			// 
			this->MeasureTillTextBox->Location = System::Drawing::Point(50, 47);
			this->MeasureTillTextBox->Name = L"MeasureTillTextBox";
			this->MeasureTillTextBox->Size = System::Drawing::Size(100, 20);
			this->MeasureTillTextBox->TabIndex = 4;
			this->MeasureTillTextBox->Text = L"1";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(49, 133);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(52, 13);
			this->label4->TabIndex = 5;
			this->label4->Text = L"amplitude";
			// 
			// AmplitudeTextBox
			// 
			this->AmplitudeTextBox->Location = System::Drawing::Point(50, 149);
			this->AmplitudeTextBox->Name = L"AmplitudeTextBox";
			this->AmplitudeTextBox->Size = System::Drawing::Size(100, 20);
			this->AmplitudeTextBox->TabIndex = 6;
			this->AmplitudeTextBox->Text = L"10000";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(12, 175);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 7;
			this->button1->Text = L"Ok";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &EquilizeAmplitude::Ok_OnClick);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(93, 175);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 8;
			this->button2->Text = L"Cancel";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &EquilizeAmplitude::Cancel_OnClick);
			// 
			// SelectInstrumentCheckBox
			// 
			this->SelectInstrumentCheckBox->AutoSize = true;
			this->SelectInstrumentCheckBox->Location = System::Drawing::Point(50, 74);
			this->SelectInstrumentCheckBox->Name = L"SelectInstrumentCheckBox";
			this->SelectInstrumentCheckBox->Size = System::Drawing::Size(107, 17);
			this->SelectInstrumentCheckBox->TabIndex = 9;
			this->SelectInstrumentCheckBox->Text = L"Select instrument";
			this->SelectInstrumentCheckBox->UseVisualStyleBackColor = true;
			this->SelectInstrumentCheckBox->Click += gcnew System::EventHandler(this, &EquilizeAmplitude::SelectInstrument_onClick);
			// 
			// Instrument
			// 
			this->Instrument->FormattingEnabled = true;
			this->Instrument->Location = System::Drawing::Point(73, 98);
			this->Instrument->Name = L"Instrument";
			this->Instrument->Size = System::Drawing::Size(121, 21);
			this->Instrument->TabIndex = 10;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(11, 101);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(56, 13);
			this->label5->TabIndex = 11;
			this->label5->Text = L"Instrument";
			// 
			// EquilizeAmplitude
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(209, 210);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->Instrument);
			this->Controls->Add(this->SelectInstrumentCheckBox);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->AmplitudeTextBox);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->MeasureTillTextBox);
			this->Controls->Add(this->MeasureFromTextBox);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"EquilizeAmplitude";
			this->Text = L"Equilize Amplitude";
			this->Load += gcnew System::EventHandler(this, &EquilizeAmplitude::onLoad);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Cancel_OnClick(System::Object^  sender, System::EventArgs^  e) {
		this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
		this->Close();
	}
private: System::Void Ok_OnClick(System::Object^  sender, System::EventArgs^  e) {
	this->DialogResult = System::Windows::Forms::DialogResult::OK;
}
private: System::Void SelectInstrument_onClick(System::Object^  sender, System::EventArgs^  e) {
	this->Instrument->Enabled = this->SelectInstrumentCheckBox->Checked;
}
private: System::Void onLoad(System::Object^  sender, System::EventArgs^  e) {
	this->Instrument->DataSource = Enum::GetValues(WAVE_TYPE::typeid);
	this->Instrument->Enabled = this->SelectInstrumentCheckBox->Checked;
}
};
}
