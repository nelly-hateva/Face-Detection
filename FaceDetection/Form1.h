#include "FaceDetector.h"
#include "RetCoord.h"
#include "vector"

#pragma once

namespace FaceDetection {

    using namespace std;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
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
		}
	private: System::Windows::Forms::Button^  button1;
	protected: 
	private: System::Windows::Forms::PictureBox^  pictureBox1;

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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::DarkGray;
			this->button1->Location = System::Drawing::Point(12, 12);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Browse...";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(115, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(238, 180);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 2;
			this->pictureBox1->TabStop = false;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::LightGray;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->ClientSize = System::Drawing::Size(467, 246);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->button1);
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"FaceDetection";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog();
		openFileDialog1->InitialDirectory = "..\\Images";
        openFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
	    openFileDialog1->FilterIndex = 2;
	    openFileDialog1->RestoreDirectory = true;
	    if (System::Windows::Forms::DialogResult::OK == openFileDialog1->ShowDialog())
		    pictureBox1->Image = Image::FromFile(openFileDialog1->FileName);

		Bitmap^ original_image_as_bitmap = gcnew Bitmap(pictureBox1->Image);
		Bitmap^ image_as_bitmap = gcnew Bitmap(pictureBox1->Image);
		image_as_bitmap = FaceDetector::BlackAndWhite(FaceDetector::GrayScale(FaceDetector::NormalizeImageBrightness(image_as_bitmap)));

		vector< vector<int> > white_black;
		vector<int> row;
	    row.assign(image_as_bitmap->Width, 0);
	    white_black.assign(image_as_bitmap->Height, row);
      
		for (int height = 0; height < image_as_bitmap->Height; height++)
		    for (int width = 0; width < image_as_bitmap->Width; width++)
		    {
		        Color color = image_as_bitmap->GetPixel(width, height);
			    if (color.G == 255 && color.R == 255 && color.B == 255)
				    white_black[height][width] = 1;
				//there is no need of else, because other pixels are 0 anyway
			    //else if (color.G == 0 && color.R == 0 && color.B == 0)
				    //white_black[height][width]  = 0;
			}

		vector< vector<int> > connected_components_matrix = FaceDetector::ConectedComponents(white_black);
		vector< vector<int> > lrtbMatrix = FaceDetector::Lrtb(connected_components_matrix);
		for (unsigned int i = 0; i < lrtbMatrix.size(); i++)
		    for (unsigned int j = 0; j < lrtbMatrix[0].size(); j++)
			{
			    if (lrtbMatrix[i][j] == 0)
				    image_as_bitmap->SetPixel(j, i, Color::Black);
			    else
			    {
				    image_as_bitmap->SetPixel(j, i, Color::White);
				    lrtbMatrix[i][j] = 1;
			    }
			}

		List<RetCoord^> ^retCoord = FaceDetector::CountComponent(lrtbMatrix);
		for each (RetCoord^ coord in retCoord)
		{
		    for (int i = coord->left; i <= coord->right; i++)
				original_image_as_bitmap->SetPixel(i, coord->top, Color::Green);
		    for (int i = coord->left; i <= coord->right; i++)
				original_image_as_bitmap->SetPixel(i, coord->bottom, Color::Green);
		    for (int i = coord->top; i <= coord->bottom; i++)
				original_image_as_bitmap->SetPixel(coord->left, i, Color::Green);
		    for (int i = coord->top; i <= coord->bottom; i++)
				original_image_as_bitmap->SetPixel(coord->right, i, Color::Green);
		}

		pictureBox1->Image = original_image_as_bitmap;
		MessageBox::Show("Faces are detected!");
    }
	};
}

