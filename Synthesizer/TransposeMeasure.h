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
	/// Summary for TransposeMeasure
	/// </summary>
	public ref class TransposeMeasure : public System::Windows::Forms::Form
	{
	public:
		TransposeMeasure(void)
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
		~TransposeMeasure()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected:
	private: System::Windows::Forms::Label^  label2;
	public: System::Windows::Forms::TextBox^  StartMeasure;
	private:
	public: System::Windows::Forms::TextBox^  EndMeasure;
	private: System::Windows::Forms::Label^  label3;
	public:
	public: System::Windows::Forms::TextBox^  Offset;
	private:
	public: System::Windows::Forms::CheckBox^  SelectInstrumentCheckBox;
	private: System::Windows::Forms::Label^  label4;
	public:
	public: System::Windows::Forms::ComboBox^  Instrument;
	private: System::Windows::Forms::Button^  button1;
	public:
	private: System::Windows::Forms::Button^  button2;
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
			this->StartMeasure = (gcnew System::Windows::Forms::TextBox());
			this->EndMeasure = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->Offset = (gcnew System::Windows::Forms::TextBox());
			this->SelectInstrumentCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->Instrument = (gcnew System::Windows::Forms::ComboBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 16);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(73, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Start Measure";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(16, 43);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(70, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"End Measure";
			// 
			// StartMeasure
			// 
			this->StartMeasure->Location = System::Drawing::Point(93, 13);
			this->StartMeasure->Name = L"StartMeasure";
			this->StartMeasure->Size = System::Drawing::Size(100, 20);
			this->StartMeasure->TabIndex = 2;
			// 
			// EndMeasure
			// 
			this->EndMeasure->Location = System::Drawing::Point(93, 40);
			this->EndMeasure->Name = L"EndMeasure";
			this->EndMeasure->Size = System::Drawing::Size(100, 20);
			this->EndMeasure->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(12, 91);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(288, 13);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Offset (halve notes, positive increases, negative decreases)";
			// 
			// Offset
			// 
			this->Offset->Location = System::Drawing::Point(12, 108);
			this->Offset->Name = L"Offset";
			this->Offset->Size = System::Drawing::Size(288, 20);
			this->Offset->TabIndex = 5;
			// 
			// SelectInstrumentCheckBox
			// 
			this->SelectInstrumentCheckBox->AutoSize = true;
			this->SelectInstrumentCheckBox->Location = System::Drawing::Point(12, 152);
			this->SelectInstrumentCheckBox->Name = L"SelectInstrumentCheckBox";
			this->SelectInstrumentCheckBox->Size = System::Drawing::Size(108, 17);
			this->SelectInstrumentCheckBox->TabIndex = 6;
			this->SelectInstrumentCheckBox->Text = L"Select Instrument";
			this->SelectInstrumentCheckBox->UseVisualStyleBackColor = true;
			this->SelectInstrumentCheckBox->Click += gcnew System::EventHandler(this, &TransposeMeasure::SelectInstrument_onClick);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(12, 179);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(56, 13);
			this->label4->TabIndex = 7;
			this->label4->Text = L"Instrument";
			// 
			// Instrument
			// 
			this->Instrument->FormattingEnabled = true;
			this->Instrument->Location = System::Drawing::Point(74, 176);
			this->Instrument->Name = L"Instrument";
			this->Instrument->Size = System::Drawing::Size(121, 21);
			this->Instrument->TabIndex = 8;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(12, 203);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 9;
			this->button1->Text = L"Confirm";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &TransposeMeasure::Confirm_onClick);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(221, 203);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 10;
			this->button2->Text = L"Cancel";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &TransposeMeasure::Cancel_onClick);
			// 
			// TransposeMeasure
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(308, 233);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->Instrument);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->SelectInstrumentCheckBox);
			this->Controls->Add(this->Offset);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->EndMeasure);
			this->Controls->Add(this->StartMeasure);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"TransposeMeasure";
			this->Text = L"Transpose Measure";
			this->Load += gcnew System::EventHandler(this, &TransposeMeasure::onLoad);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void SelectInstrument_onClick(System::Object^  sender, System::EventArgs^  e) {
		this->Instrument->Enabled = this->SelectInstrumentCheckBox->Checked;
	}
private: System::Void onLoad(System::Object^  sender, System::EventArgs^  e) {
	this->Instrument->DataSource = Enum::GetValues(WAVE_TYPE::typeid);
	this->Instrument->Enabled = this->SelectInstrumentCheckBox->Checked;
}
private: System::Void Confirm_onClick(System::Object^  sender, System::EventArgs^  e) {
	this->DialogResult = System::Windows::Forms::DialogResult::OK;
	this->Close();
}
private: System::Void Cancel_onClick(System::Object^  sender, System::EventArgs^  e) {
	this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->Close();
}
};
}
