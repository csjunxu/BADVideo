#pragma once

#include <stdlib.h>
#include <cv.h>
#include <highgui.h>

namespace BADVideo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
  using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
    private:
      String^ videoFileName;
  private: System::Windows::Forms::PictureBox^  EnhanceImageButton;
  private: System::Windows::Forms::Label^  EnhanceLabel;
           IplImage** newVideoFrames;
           double fps;
           int videoWidth;
           int videoHeight;
  private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
  private: System::Windows::Forms::Label^  FilenameLabel;
           int numFrames;

	  public:
		  Form1(void) {
			  InitializeComponent();
			  videoFileName = nullptr;
        newVideoFrames = nullptr;
        fps = 0.0;
        videoWidth = 0;
        videoHeight = 0;
        numFrames = 0;
		  }

	  protected:
		  /// <summary>
		  /// Clean up any resources being used.
		  /// </summary>
		  ~Form1() {
			  if (components) {
				  delete components;
			  }

        if (newVideoFrames) {
          delete newVideoFrames;
        }
		  }
	  private: System::Windows::Forms::PictureBox^  SaveImageButton;
	  protected: 
	  private: System::Windows::Forms::Label^  OpenLabel;
	  private: System::Windows::Forms::PictureBox^  PreviewImageButton;
	  private: System::Windows::Forms::Label^  PreviewLabel;
	  private: System::Windows::Forms::PictureBox^  OpenImageButton;
	  private: System::Windows::Forms::Label^  SaveLabel;
	  private: System::Windows::Forms::PictureBox^  exitImageButton;
	  private: System::Windows::Forms::Label^  ExitLabel;
  private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;

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
        System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
        this->SaveImageButton = (gcnew System::Windows::Forms::PictureBox());
        this->OpenLabel = (gcnew System::Windows::Forms::Label());
        this->PreviewImageButton = (gcnew System::Windows::Forms::PictureBox());
        this->PreviewLabel = (gcnew System::Windows::Forms::Label());
        this->OpenImageButton = (gcnew System::Windows::Forms::PictureBox());
        this->SaveLabel = (gcnew System::Windows::Forms::Label());
        this->exitImageButton = (gcnew System::Windows::Forms::PictureBox());
        this->ExitLabel = (gcnew System::Windows::Forms::Label());
        this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
        this->EnhanceImageButton = (gcnew System::Windows::Forms::PictureBox());
        this->EnhanceLabel = (gcnew System::Windows::Forms::Label());
        this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
        this->FilenameLabel = (gcnew System::Windows::Forms::Label());
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SaveImageButton))->BeginInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PreviewImageButton))->BeginInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->OpenImageButton))->BeginInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->exitImageButton))->BeginInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->EnhanceImageButton))->BeginInit();
        this->SuspendLayout();
        // 
        // SaveImageButton
        // 
        this->SaveImageButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"SaveImageButton.Image")));
        this->SaveImageButton->Location = System::Drawing::Point(12, 222);
        this->SaveImageButton->Name = L"SaveImageButton";
        this->SaveImageButton->Size = System::Drawing::Size(64, 64);
        this->SaveImageButton->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
        this->SaveImageButton->TabIndex = 0;
        this->SaveImageButton->TabStop = false;
        this->SaveImageButton->Click += gcnew System::EventHandler(this, &Form1::SaveImageButton_Click);
        // 
        // OpenLabel
        // 
        this->OpenLabel->AutoSize = true;
        this->OpenLabel->Font = (gcnew System::Drawing::Font(L"Constantia", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
          static_cast<System::Byte>(0)));
        this->OpenLabel->Location = System::Drawing::Point(82, 33);
        this->OpenLabel->Name = L"OpenLabel";
        this->OpenLabel->Size = System::Drawing::Size(61, 23);
        this->OpenLabel->TabIndex = 1;
        this->OpenLabel->Text = L"Open.";
        // 
        // PreviewImageButton
        // 
        this->PreviewImageButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"PreviewImageButton.Image")));
        this->PreviewImageButton->Location = System::Drawing::Point(12, 152);
        this->PreviewImageButton->Name = L"PreviewImageButton";
        this->PreviewImageButton->Size = System::Drawing::Size(64, 64);
        this->PreviewImageButton->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
        this->PreviewImageButton->TabIndex = 2;
        this->PreviewImageButton->TabStop = false;
        this->PreviewImageButton->Click += gcnew System::EventHandler(this, &Form1::PreviewImageButton_Click);
        // 
        // PreviewLabel
        // 
        this->PreviewLabel->AutoSize = true;
        this->PreviewLabel->Font = (gcnew System::Drawing::Font(L"Constantia", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
          static_cast<System::Byte>(0)));
        this->PreviewLabel->Location = System::Drawing::Point(82, 172);
        this->PreviewLabel->Name = L"PreviewLabel";
        this->PreviewLabel->Size = System::Drawing::Size(77, 23);
        this->PreviewLabel->TabIndex = 3;
        this->PreviewLabel->Text = L"Preview.";
        // 
        // OpenImageButton
        // 
        this->OpenImageButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"OpenImageButton.Image")));
        this->OpenImageButton->Location = System::Drawing::Point(12, 12);
        this->OpenImageButton->Name = L"OpenImageButton";
        this->OpenImageButton->Size = System::Drawing::Size(64, 64);
        this->OpenImageButton->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
        this->OpenImageButton->TabIndex = 4;
        this->OpenImageButton->TabStop = false;
        this->OpenImageButton->Click += gcnew System::EventHandler(this, &Form1::OpenImageButton_Click);
        // 
        // SaveLabel
        // 
        this->SaveLabel->AutoSize = true;
        this->SaveLabel->Font = (gcnew System::Drawing::Font(L"Constantia", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
          static_cast<System::Byte>(0)));
        this->SaveLabel->Location = System::Drawing::Point(82, 240);
        this->SaveLabel->Name = L"SaveLabel";
        this->SaveLabel->Size = System::Drawing::Size(52, 23);
        this->SaveLabel->TabIndex = 5;
        this->SaveLabel->Text = L"Save.";
        // 
        // exitImageButton
        // 
        this->exitImageButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"exitImageButton.Image")));
        this->exitImageButton->Location = System::Drawing::Point(12, 292);
        this->exitImageButton->Name = L"exitImageButton";
        this->exitImageButton->Size = System::Drawing::Size(64, 64);
        this->exitImageButton->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
        this->exitImageButton->TabIndex = 6;
        this->exitImageButton->TabStop = false;
        this->exitImageButton->Click += gcnew System::EventHandler(this, &Form1::exitImageButton_Click);
        // 
        // ExitLabel
        // 
        this->ExitLabel->AutoSize = true;
        this->ExitLabel->Font = (gcnew System::Drawing::Font(L"Constantia", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
          static_cast<System::Byte>(0)));
        this->ExitLabel->Location = System::Drawing::Point(82, 318);
        this->ExitLabel->Name = L"ExitLabel";
        this->ExitLabel->Size = System::Drawing::Size(47, 23);
        this->ExitLabel->TabIndex = 7;
        this->ExitLabel->Text = L"Exit.";
        // 
        // openFileDialog1
        // 
        this->openFileDialog1->FileName = L"openFileDialog1";
        this->openFileDialog1->Filter = L"AVI Files (*.avi)|*.avi";
        this->openFileDialog1->InitialDirectory = L"C:\\Users\\tgh_2\\Desktop\\workspace\\visual studio 2010\\Projects\\BADVideo\\BADVideo\\";
        this->openFileDialog1->Title = L"Select an AVI file";
        // 
        // EnhanceImageButton
        // 
        this->EnhanceImageButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"EnhanceImageButton.Image")));
        this->EnhanceImageButton->Location = System::Drawing::Point(12, 82);
        this->EnhanceImageButton->Name = L"EnhanceImageButton";
        this->EnhanceImageButton->Size = System::Drawing::Size(64, 59);
        this->EnhanceImageButton->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
        this->EnhanceImageButton->TabIndex = 8;
        this->EnhanceImageButton->TabStop = false;
        this->EnhanceImageButton->Click += gcnew System::EventHandler(this, &Form1::EnhanceImageButton_Click);
        // 
        // EnhanceLabel
        // 
        this->EnhanceLabel->AutoSize = true;
        this->EnhanceLabel->Font = (gcnew System::Drawing::Font(L"Constantia", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
          static_cast<System::Byte>(0)));
        this->EnhanceLabel->Location = System::Drawing::Point(82, 99);
        this->EnhanceLabel->Name = L"EnhanceLabel";
        this->EnhanceLabel->Size = System::Drawing::Size(86, 23);
        this->EnhanceLabel->TabIndex = 9;
        this->EnhanceLabel->Text = L"Enhance.";
        // 
        // saveFileDialog1
        // 
        this->saveFileDialog1->Filter = L"MPEG Files (*.mpg)|*.mpg";
        this->saveFileDialog1->InitialDirectory = L"C:\\Users\\tgh_2\\Desktop\\workspace\\visual studio 2010\\Projects\\BADVideo\\BADVideo\\";
        this->saveFileDialog1->Title = L"Save As...";
        // 
        // FilenameLabel
        // 
        this->FilenameLabel->AutoSize = true;
        this->FilenameLabel->Font = (gcnew System::Drawing::Font(L"Courier New", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
          static_cast<System::Byte>(0)));
        this->FilenameLabel->ForeColor = System::Drawing::Color::Green;
        this->FilenameLabel->Location = System::Drawing::Point(149, 40);
        this->FilenameLabel->Name = L"FilenameLabel";
        this->FilenameLabel->Size = System::Drawing::Size(56, 16);
        this->FilenameLabel->TabIndex = 10;
        this->FilenameLabel->Text = L"label1";
        this->FilenameLabel->Visible = false;
        // 
        // Form1
        // 
        this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
        this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        this->BackColor = System::Drawing::SystemColors::Window;
        this->ClientSize = System::Drawing::Size(340, 368);
        this->Controls->Add(this->FilenameLabel);
        this->Controls->Add(this->EnhanceLabel);
        this->Controls->Add(this->EnhanceImageButton);
        this->Controls->Add(this->ExitLabel);
        this->Controls->Add(this->exitImageButton);
        this->Controls->Add(this->SaveLabel);
        this->Controls->Add(this->OpenImageButton);
        this->Controls->Add(this->PreviewLabel);
        this->Controls->Add(this->PreviewImageButton);
        this->Controls->Add(this->OpenLabel);
        this->Controls->Add(this->SaveImageButton);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
        this->MaximizeBox = false;
        this->Name = L"Form1";
        this->Text = L"BADVideo";
        this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SaveImageButton))->EndInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PreviewImageButton))->EndInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->OpenImageButton))->EndInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->exitImageButton))->EndInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->EnhanceImageButton))->EndInit();
        this->ResumeLayout(false);
        this->PerformLayout();

      }
  #pragma endregion

			
	  private:
      System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {}

      //----------------------------------------------------------------------

      ///<summary>
      ///On click of EXIT icon, exit the program.
      ///</summary>
      System::Void exitImageButton_Click(System::Object^  sender, System::EventArgs^  e) {
        exit(0);
			}

      //----------------------------------------------------------------------

      ///<summary>
      ///On click of OPEN icon, open the avi file select dialog
      ///</summary>
      System::Void OpenImageButton_Click(System::Object^  sender, System::EventArgs^  e) {
        if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
          //save the (full path) filename
          videoFileName = openFileDialog1->FileName;

          //get the filename only (without the path)
          int idx = videoFileName->LastIndexOf('\\');
          String^ shortFileName = videoFileName->Substring(idx+1);

          //show the filename in the form
          FilenameLabel->Text = shortFileName;
          FilenameLabel->Visible = true;

          //convert String^ to char* for opencv
          const char * fileName = (char*)Marshal::StringToHGlobalAnsi(videoFileName).ToPointer();

          CvCapture* videoCapture = cvCreateFileCapture(fileName);
          //get the frames per second (fps) of the video
          fps = cvGetCaptureProperty(videoCapture, CV_CAP_PROP_FPS);
          //get the size of the video
          videoWidth  = (int)cvGetCaptureProperty(videoCapture, CV_CAP_PROP_FRAME_WIDTH);
          videoHeight = (int)cvGetCaptureProperty(videoCapture, CV_CAP_PROP_FRAME_HEIGHT);
          //get the number of frames for the video
          // (subtract 1, because it seems the last frame is null, but is
          // included in the frame count)
          numFrames = cvGetCaptureProperty(videoCapture, CV_CAP_PROP_FRAME_COUNT) - 1;
          newVideoFrames = new IplImage*[numFrames];
          //store the frames of the video for later use
          for (int i=0; i < numFrames; ++i) {
            newVideoFrames[i] = cvCloneImage(cvQueryFrame(videoCapture));
          }
        }
      }

      //----------------------------------------------------------------------

      ///<summary>
      ///On click of PREVIEW icon, play the original video along side an
      ///enhanced version of the original.
      ///</summary>
      System::Void PreviewImageButton_Click(System::Object^  sender, System::EventArgs^  e) {
        //convert String^ to char* for opencv
        const char * fileName = (char*)Marshal::StringToHGlobalAnsi(videoFileName).ToPointer();
        //create a viewing window for the original video
        cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);

        CvCapture* videoCapture = cvCreateFileCapture(fileName);
        IplImage* frame;  //holds each frame in the video

        //play the video
        while(1) {
          //grab next frame
          frame = cvQueryFrame(videoCapture);
          //end of video
          if (!frame)
            break;
          //display the frame
          cvShowImage("Original", frame);
          //check for keystroke at each frame (assuming frames per second (fps) is 30)
          char c = cvWaitKey(33);
          //stop playback if user hit 'ESC'
          if (c == 27)
            break;
        }

        //free allocated memory used by opencv
        cvReleaseCapture(&videoCapture);
        cvDestroyWindow("Original");
      }

      //----------------------------------------------------------------------

      ///<summary>
      ///On click of ENHANCE icon, process the stored video.
      ///</summary>
      System::Void EnhanceImageButton_Click(System::Object^  sender, System::EventArgs^  e) {
      
      }
      
      //----------------------------------------------------------------------

      ///<summary>
      ///On click of SAVE icon, the enhanced video is written out to a file.
      ///</summary>
      System::Void SaveImageButton_Click(System::Object^  sender, System::EventArgs^  e) {
        if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
          //create a CvSize structure to pass to cvCreateVideoWriter
          CvSize videoSize = cvSize(videoWidth, videoHeight);
          //the argument '0' is telling Windows to create a writer of uncompressed .avi files
          CvVideoWriter* writer = cvCreateVideoWriter("newfile.mpg", CV_FOURCC('P','I','M','1'), fps, videoSize);
          //write every frame
          for (int i=0; i < numFrames; ++i) {
            cvWriteFrame(writer, newVideoFrames[i]);
          }
          //cleanup
          cvReleaseVideoWriter(&writer);
        }
      }
  };
}