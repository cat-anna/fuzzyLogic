#pragma once

#include "Fuzzy.h"
#include "FuzzyRules.h"

#pragma managed

namespace FuzzyLogic {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			m_Fuzzy = CreateWorld();
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			delete m_Fuzzy;
		}

	protected: 
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::Windows::Forms::TrackBar^  trackBar2;

	private: System::Windows::Forms::Label^  label2;


	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::ListView^  listView1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;
	private: System::Windows::Forms::Label^  label3;

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
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->label3 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(135, 15);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"label1";
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(12, 31);
			this->trackBar1->Maximum = 300;
			this->trackBar1->Minimum = 150;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(383, 45);
			this->trackBar1->TabIndex = 2;
			this->trackBar1->Value = 150;
			this->trackBar1->ValueChanged += gcnew System::EventHandler(this, &Form1::trackBar2_ValueChanged);
			// 
			// trackBar2
			// 
			this->trackBar2->Location = System::Drawing::Point(12, 92);
			this->trackBar2->Maximum = 80;
			this->trackBar2->Minimum = -20;
			this->trackBar2->Name = L"trackBar2";
			this->trackBar2->Size = System::Drawing::Size(383, 45);
			this->trackBar2->TabIndex = 3;
			this->trackBar2->ValueChanged += gcnew System::EventHandler(this, &Form1::trackBar2_ValueChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(135, 76);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(35, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"label2";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(17, 79);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(89, 13);
			this->label5->TabIndex = 8;
			this->label5->Text = L"Temperatura zew";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(17, 15);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(92, 13);
			this->label6->TabIndex = 7;
			this->label6->Text = L"Temperatura wew";
			// 
			// listView1
			// 
			this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->columnHeader1, this->columnHeader2, 
				this->columnHeader3});
			this->listView1->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->listView1->Location = System::Drawing::Point(12, 143);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(269, 214);
			this->listView1->TabIndex = 10;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::Details;
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"Parametr";
			this->columnHeader1->Width = 79;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"Wartoœæ";
			this->columnHeader2->Width = 73;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"Przynale¿noœæ";
			this->columnHeader3->Width = 85;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->label3->Location = System::Drawing::Point(12, 371);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(41, 13);
			this->label3->TabIndex = 11;
			this->label3->Text = L"label3";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(407, 393);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->listView1);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->trackBar2);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->label1);
			this->Name = L"Form1";
			this->Text = L"Sterownik klimatyzacji";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		FuzzyWorld *m_Fuzzy;

	private: void RefreshValuesList() {
				 listView1->BeginUpdate();
				 listView1->Items->Clear();
				 for(auto &it: m_Fuzzy->AccessValues()) {
					 ListViewItem ^itm = listView1->Items->Add(gcnew String(it.Word->Owner->GetName().c_str()));
					 itm->SubItems->Add(gcnew String(it.Word->Name.c_str()));
					 itm->SubItems->Add(String::Format("{0}", it.value));
				 }

				 if(!m_Fuzzy->Results().empty()){
					 auto &it =* m_Fuzzy->Results().begin();
					 float f = it.second;
					 if(abs(f) <= 0.01f) f = 0;
					 label3->Text = String::Format("Klimatyzacja {0}", f);
				 }
				 listView1->EndUpdate();
			 }

	private: System::Void trackBar2_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 float v[] = { trackBar1->Value / 10.0f, trackBar2->Value / 2.0f };
				 label1->Text = String::Format("{0} st C", v[0]);
				 label2->Text = String::Format("{0} st C", v[1]);

				 const char **paramt = ParamNamesTable;

				 m_Fuzzy->ClearValues();

				 for(int i = 0; i < 2; ++i) {
					 try {
						 m_Fuzzy->AddParameterValue(paramt[i], (float)v[i]);
					 }
					 catch(...) { }
				 }
				 m_Fuzzy->GetRules().Process();
				 RefreshValuesList();
			 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 trackBar1->Value = 220;
			 trackBar2->Value = 15;
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}

