#pragma once

#include "music.h"

namespace Synthesizer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for CopyMeasures
	/// </summary>
	public ref class CopyMeasures : public System::Windows::Forms::Form
	{
	public:
		CopyMeasures(void)
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
		~CopyMeasures()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected:
	private: System::Windows::Forms::Label^  label2;
	public: System::Windows::Forms::TextBox^  MeasureFromTextBox;
	private:


	private: System::Windows::Forms::Label^  label3;
	public: System::Windows::Forms::TextBox^  MeasureTillTextBox;
	private:


	private: System::Windows::Forms::Label^  label4;
	public: System::Windows::Forms::TextBox^  MeasureDestinationTextBox;
	private:


	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	public: System::Windows::Forms::CheckBox^  overwriteTextBox;
	public: System::Windows::Forms::ComboBox^  Instruments;

	public:

	public:
	private: System::Windows::Forms::Label^  label5;
	public: System::Windows::Forms::CheckBox^  SelectInstrumentCheckBox;
	private:


	private:


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
			this->MeasureFromTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->MeasureTillTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->MeasureDestinationTextBox = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->overwriteTextBox = (gcnew System::Windows::Forms::CheckBox());
			this->Instruments = (gcnew System::Windows::Forms::ComboBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->SelectInstrumentCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(46, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(91, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Measures to copy";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(13, 34);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(30, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"From";
			// 
			// MeasureFromTextBox
			// 
			this->MeasureFromTextBox->Location = System::Drawing::Point(49, 27);
			this->MeasureFromTextBox->Name = L"MeasureFromTextBox";
			this->MeasureFromTextBox->Size = System::Drawing::Size(100, 20);
			this->MeasureFromTextBox->TabIndex = 2;
			this->MeasureFromTextBox->Text = L"1";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(16, 51);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(20, 13);
			this->label3->TabIndex = 3;
			this->label3->Text = L"Till";
			// 
			// MeasureTillTextBox
			// 
			this->MeasureTillTextBox->Location = System::Drawing::Point(49, 51);
			this->MeasureTillTextBox->Name = L"MeasureTillTextBox";
			this->MeasureTillTextBox->Size = System::Drawing::Size(100, 20);
			this->MeasureTillTextBox->TabIndex = 4;
			this->MeasureTillTextBox->Text = L"1";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(156, 9);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(60, 13);
			this->label4->TabIndex = 5;
			this->label4->Text = L"Destination";
			// 
			// MeasureDestinationTextBox
			// 
			this->MeasureDestinationTextBox->Location = System::Drawing::Point(159, 27);
			this->MeasureDestinationTextBox->Name = L"MeasureDestinationTextBox";
			this->MeasureDestinationTextBox->Size = System::Drawing::Size(100, 20);
			this->MeasureDestinationTextBox->TabIndex = 6;
			this->MeasureDestinationTextBox->Text = L"1";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(49, 120);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 7;
			this->button1->Text = L"Confirm";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &CopyMeasures::Confirm_OnClick);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(159, 120);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 8;
			this->button2->Text = L"Cancel";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &CopyMeasures::Cancel_OnClick);
			// 
			// overwriteTextBox
			// 
			this->overwriteTextBox->AutoSize = true;
			this->overwriteTextBox->Location = System::Drawing::Point(159, 53);
			this->overwriteTextBox->Name = L"overwriteTextBox";
			this->overwriteTextBox->Size = System::Drawing::Size(117, 17);
			this->overwriteTextBox->TabIndex = 9;
			this->overwriteTextBox->Text = L"Overwrite old notes";
			this->overwriteTextBox->UseVisualStyleBackColor = true;
			// 
			// Instruments
			// 
			this->Instruments->FormattingEnabled = true;
			this->Instruments->Location = System::Drawing::Point(159, 93);
			this->Instruments->Name = L"Instruments";
			this->Instruments->Size = System::Drawing::Size(121, 21);
			this->Instruments->TabIndex = 10;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(92, 96);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(61, 13);
			this->label5->TabIndex = 11;
			this->label5->Text = L"Instruments";
			// 
			// SelectInstrumentCheckBox
			// 
			this->SelectInstrumentCheckBox->AutoSize = true;
			this->SelectInstrumentCheckBox->Location = System::Drawing::Point(159, 70);
			this->SelectInstrumentCheckBox->Name = L"SelectInstrumentCheckBox";
			this->SelectInstrumentCheckBox->Size = System::Drawing::Size(107, 17);
			this->SelectInstrumentCheckBox->TabIndex = 12;
			this->SelectInstrumentCheckBox->Text = L"Select instrument";
			this->SelectInstrumentCheckBox->UseVisualStyleBackColor = true;
			this->SelectInstrumentCheckBox->Click += gcnew System::EventHandler(this, &CopyMeasures::instrumentCheckBox_onCLick);
			// 
			// CopyMeasures
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(313, 155);
			this->Controls->Add(this->SelectInstrumentCheckBox);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->Instruments);
			this->Controls->Add(this->overwriteTextBox);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->MeasureDestinationTextBox);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->MeasureTillTextBox);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->MeasureFromTextBox);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"CopyMeasures";
			this->Text = L"Copy Measures";
			this->Load += gcnew System::EventHandler(this, &CopyMeasures::onLoad);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Confirm_OnClick(System::Object^  sender, System::EventArgs^  e) {
		this->DialogResult = System::Windows::Forms::DialogResult::OK;
		this->Close();
	}
private: System::Void Cancel_OnClick(System::Object^  sender, System::EventArgs^  e) {
	this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->Close();
}
private: System::Void onLoad(System::Object^  sender, System::EventArgs^  e) {
	this->Instruments->DataSource = Enum::GetValues(WAVE_TYPE::typeid);
	if (!this->SelectInstrumentCheckBox->Checked) {
		this->Instruments->Enabled = false;
	}
	else {
		this->Instruments->Enabled = true;
	}
}
private: System::Void instrumentCheckBox_onCLick(System::Object^  sender, System::EventArgs^  e) {
	if (!this->SelectInstrumentCheckBox->Checked) {
		this->Instruments->Enabled = false;
	}else{
		this->Instruments->Enabled = true;
	}
}
};
}
