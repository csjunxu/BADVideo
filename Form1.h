#pragma once

#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#include "EnhanceForm.h"
#include "ProgressDialog.h"

namespace BADVideo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
  using namespace System::Runtime::InteropServices;

	/// <summary>
	/// The main window (form) of BADVideo.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{

    #pragma region Hand-written code (not auto-generated)


    //------------------------------------------------------------------------
    //-------------------------     FIELDS     -------------------------------
    //------------------------------------------------------------------------

    private:
      ///the full path filename of the opened video
      String^ videoFileName;

      ///an array of images (frames) to hold a copy of the original video
      IplImage** originalFrames;

      ///an array of images (frames) to hold the enhanced version of the
      /// original video
      IplImage** enhancedFrames;

      ///frames per second of the video
      double fps;

      ///width of video (in pixels)
      int videoWidth;

      ///height of video (in pixels)
      int videoHeight;

      ///total number of frames in the video (also length of the frame array)
      int numFrames;

      ///holds the max luminosity values for each pixel across all frames
      float* max_lum_vals;
  private: System::Windows::Forms::Label^  FileNameLabel;

      ///holds the average intensity values for each channel of each pixel for
      /// all frames
      float* intensity_averages;


    //------------------------------------------------------------------------
    //------------------     CONSTRUCTORS/DESTRUCTOR     ---------------------
    //------------------------------------------------------------------------

	  public:

      ///<summary>
      ///Creates a Form1 object, which is the main Window for BADVideo.
      ///</summary>
		  Form1(void) {
        //call auto-generated code for initializing all GUI components
			  InitializeComponent();

        //set custom fields to default values
			  videoFileName  = nullptr;
        originalFrames = nullptr;
        enhancedFrames = nullptr;
        fps = 0.0;
        videoWidth = 0;
        videoHeight = 0;
        numFrames = 0;
        max_lum_vals = nullptr;
		  }


      //----------------------------------------------------------------------


		  /// <summary>
		  /// Clean up any resources being used.
		  /// </summary>
		  ~Form1() {
			  if (components) {
				  delete components;
			  }
        if (originalFrames) {
          delete originalFrames;
        }
        if(enhancedFrames) {
          delete enhancedFrames;
        }
		  }


    //------------------------------------------------------------------------
    //------------------------     METHODS     -------------------------------
    //------------------------------------------------------------------------

    private:

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
          FileNameLabel->Visible = true;
          FileNameLabel->Text = shortFileName;

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

          //reset original frames array
          if (originalFrames != nullptr) {
            delete originalFrames;
            originalFrames = nullptr;
          }
          //reset enhanced frames array
          if (enhancedFrames != nullptr) {
            delete enhancedFrames;
            enhancedFrames = nullptr;
          }

          //get all of the original frames to use for processing later
          originalFrames = new IplImage*[numFrames];
          for (int i=0; i < numFrames; ++i) {
            originalFrames[i] = cvCloneImage(cvQueryFrame(videoCapture));
          }

          /*
          //initialize the max luminance values array
          int length = videoWidth * videoHeight;
          max_lum_vals = new float[length];
          for (int i=0; i < length; ++i) {
            max_lum_vals[i] = (float) 0.0;
          }

          //initialize the average intenisty values array
          length *= 3;
          intensity_averages = new float[videoWidth*videoHeight*3];
          for (int i=0; i < numFrames; ++i) {
            intensity_averages[i] = (float) 0.0;
          }
          */
        }

      }// OPEN


      //----------------------------------------------------------------------


      ///<summary>
      ///On click of PREVIEW icon, play the original video along side an
      ///enhanced version of the original.
      ///</summary>
      System::Void PreviewImageButton_Click(System::Object^  sender, System::EventArgs^  e) {
        //check to see if the video has been enhanced--if not, only the original
        // video will be played back
        bool enhanced = true;
        if (enhancedFrames == nullptr)
          enhanced = false;

        //convert String^ to char* for the original video filename (for opencv)
        const char * fileName = (char*)Marshal::StringToHGlobalAnsi(videoFileName).ToPointer();

        //create viewing windows for the original and enhanced videos
        cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);
        if (enhanced)
          cvNamedWindow("Enhanced", CV_WINDOW_AUTOSIZE);
        //position the windows side-by-side
        cvMoveWindow("Original", this->Location.X + this->Size.Width + 10, this->Location.Y);
        if (enhanced)
          cvMoveWindow("Enhanced", this->Location.X + this->Size.Width + videoWidth + 20, this->Location.Y);

        //capture object for the original video
        CvCapture* videoCapture = cvCreateFileCapture(fileName);
        IplImage* frame;  //holds each frame in the original video

        //play the videos
        bool play = true;
        while (play) {
          //start video capture at first frame
          cvSetCaptureProperty(videoCapture, CV_CAP_PROP_POS_FRAMES, 0);
          for(int i=0; i < numFrames; ++i) {
            //grab next frame
            frame = cvQueryFrame(videoCapture);
            //end of video
            if (!frame)
              break;

            //display the current frame of each video
            cvShowImage("Original", frame);
            if (enhanced)
              cvShowImage("Enhanced", enhancedFrames[i]);

            //check for keystroke at each frame (assuming frames per second (fps) is 30)
            char c = cvWaitKey(33);
            //stop playback and close windows if user hit 'ESC'
            if (c == 27) {
              play = false;
              break;
            }
            //stop video if user hit spacebar
            if (c == 32) {
              //wait until user hits spacebar again to resume playback
              do {
                c = cvWaitKey(0);
              } while (c != 32);
            }
          }
        }

        //free allocated memory used by opencv
        cvReleaseCapture(&videoCapture);
        cvDestroyWindow("Original");
        if (enhanced)
          cvDestroyWindow("Enhanced");
      
      }// PREVIEW


      //----------------------------------------------------------------------


      ///<summary>
      ///On click of ENHANCE icon, process the stored video.
      ///</summary>
      System::Void EnhanceImageButton_Click(System::Object^  sender, System::EventArgs^ e) {
        //a video file hasn't been opened yet
        if (videoFileName == nullptr) {
          MessageBox::Show("You need to open a video file first.", "Sorry.");
          return;
        }

        //create the Enhance dialog window
        EnhanceForm^ eForm = gcnew EnhanceForm(numFrames/10);
        //open the Enhance dialog
        System::Windows::Forms::DialogResult dr = eForm->ShowDialog();
        //user clicked Cancel, get out
        if (dr == System::Windows::Forms::DialogResult::Cancel) {
          return;
        }
        //get the values that the user chose for frame count and gain
        int temporalMargin = eForm->getTemporalMargin();
        int gainFactor = eForm->getGainValue();
        float gain = (float) gainFactor / (float) 100.0;

        brightenAndDenoise(temporalMargin, gain);
        return;

        /* DEBUG: Show image of average of all frames

        calcIntensityAverages();
        cv::Mat avgIntensitiesMatrix(videoHeight, videoWidth, CV_32FC3, intensity_averages);
        IplImage* temp = new IplImage(avgIntensitiesMatrix);
        //float maxI = calcMaxImageIntensity(temp);
        //float maxGain = (((float) 1.0) / maxI) * (float) 3.0;

        IplImage* temp2 = brightenImage(temp, gain);

        CvCapture* videoCapture = cvCreateFileCapture("C:\\Users\\tgh_2\\Desktop\\workspace\\visual\ studio\ 2010\\Projects\\BADVideo\\BADVideo\\1_(correctly_exposed).avi");
        IplImage* goal = cvQueryFrame(videoCapture);

        //create windows
        cvNamedWindow("orig",CV_WINDOW_AUTOSIZE);
        cvShowImage("orig",newVideoFrames[0]);
        cvNamedWindow("avg",CV_WINDOW_AUTOSIZE);
        cvShowImage("avg",temp);
        cvNamedWindow("result", CV_WINDOW_AUTOSIZE);
        cvShowImage("result", temp2);
        cvNamedWindow("goal", CV_WINDOW_AUTOSIZE);
        cvShowImage("goal", goal);
        //wait for a keystroke...
        cvWaitKey(0);
        //destroy windows
        cvDestroyWindow("orig");
        cvDestroyWindow("avg");
        cvDestroyWindow("result");
        cvDestroyWindow("goal");
        cvReleaseCapture(&videoCapture);
        delete temp;
        delete temp2;
        return;
        
        /* END DEBUG */


        /* DEBUG: show results of 1 frame

        //enhance the frame
        cv::Mat enhancedMatrix(getLuminanceAsImage(newVideoFrames[0]));

        //gaussian blur
        cv::GaussianBlur(enhancedMatrix, enhancedMatrix, cv::Size(15,15), 0, 0);
        IplImage* i = new IplImage(enhancedMatrix);

        //show original frame in a window
        cvNamedWindow("orig",CV_WINDOW_AUTOSIZE);
        cvShowImage("orig",newVideoFrames[0]);
        //show enhanced frame in a window
        cvNamedWindow("new",CV_WINDOW_AUTOSIZE);
        cvShowImage("new", i);
        //wait for a keystroke...
        cvWaitKey(0);
        //destroy windows and move on
        cvDestroyWindow("orig");
        cvDestroyWindow("new");
        return;

        /* END Debug */

        /*
        for(int i=0; i < numFrames; ++i) {
          //extract the luminance from the frame
          //cv::Mat enhancedMatrix(getLuminanceAsImage(normalizeIplImage(newVideoFrames[i])));
          cv::Mat enhancedMatrix(normalizeIplImage(newVideoFrames[i]));
          cv::Mat tempMatrix(videoHeight, videoWidth, CV_32FC3);

          //apply a smooth filter to the luminance-only image
          try {
            cv::GaussianBlur(enhancedMatrix, enhancedMatrix, cv::Size(25,25), 0, 0);
          }
          catch (Exception^ e) {
            MessageBox::Show("The dimensional sizes of the gaussian kernel must be odd positive numbers.","Error");
            return;
          }

          bilateralFilter(enhancedMatrix, tempMatrix, 9, 100.0, 10.0);
          //newVideoFrames[i] = cvCloneImage(m(new IplImage(enhancedMatrix), 64));
          newVideoFrames[i] = cvCloneImage(new IplImage(tempMatrix));
        }
        */

      }// ENHANCE
      

      //----------------------------------------------------------------------


      ///<summary>
      ///On click of SAVE icon, the enhanced video is written out to a file.
      ///</summary>
      System::Void SaveImageButton_Click(System::Object^  sender, System::EventArgs^  e) {
        //video hasn't been enhanced
        if (enhancedFrames == nullptr) {
          MessageBox::Show("Video hasn't been enhanced yet.", "Can't save");
          return;
        }
        //user hit OK (i.e. did not cancel the operation)
        if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
          //get the filename that the user entered
          const char * fileName = (char*)Marshal::StringToHGlobalAnsi(saveFileDialog1->FileName).ToPointer();
          //create a CvSize structure to pass to cvCreateVideoWriter
          CvSize videoSize = cvSize(videoWidth, videoHeight);
          CvVideoWriter* writer = cvCreateVideoWriter(fileName, CV_FOURCC('P','I','M','1'), fps, videoSize);
          //write every frame
          for (int i=0; i < numFrames; ++i) {
            cvWriteFrame(writer, enhancedFrames[i]);
          }
          //cleanup
          cvReleaseVideoWriter(&writer);
        }

      }// SAVE


      //------------------------------------------------------------------------


      ///<summary>
      ///Extracts the luminance of an image and returns it as a new image, not
      /// modifying the original.  It is assumed the IplImage is of type
      /// CV_32FC3.
      ///</summary>
      IplImage* getLuminanceAsImage(IplImage * img) {
        int width  = img->width;
        int height = img->height;
        int len    = width * height;
        float * lum_vals = new float[len];    //to store the luminance values

        //extract the luminance values from the image pixel by pixel
        int j=0;
        for (int y=0; y < height; ++y) {
          float* ptr = (float*) (img->imageData + y * img->widthStep);
          for (int x=0; x < width; ++x) {
            //convert RGB values to luminance
            lum_vals[j] = (float) (0.2126 * ptr[3*x] + 0.7152 * ptr[3*x+1] + 0.0722 * ptr[3*x+2]);
            //see if this is the maximum luminance value for this pixel seen so far
            if (max_lum_vals[j] < lum_vals[j]) {
              max_lum_vals[j] = lum_vals[j];
            }
            ++j;
          }
        }

        //create a matrix with the values
        cv::Mat luminanceMatrix(height, width, CV_32FC1, lum_vals);
        delete lum_vals;
        //convert matrix of luminance values into an image
        return new IplImage(luminanceMatrix);

      }// getLuminanceAsImage(IplImage*)


      //----------------------------------------------------------------------


      ///<summary>
      ///
      ///</summary>
      IplImage* m(IplImage* img, int psi) {
        int width  = img->width;
        int height = img->height;
        int len    = width * height;
        float* gain_vals = new float[len];  //to store the gain factor values

        cv::Mat maxLumMatrix(height, width, CV_32FC1, max_lum_vals);
        IplImage* maxLumImg = new IplImage(maxLumMatrix);

        //estimate gain factors for each pixel
        int i=0;
        for (int y=0; y < height; ++y) {
          float* imgPtr = (float*) (img->imageData + y * img->widthStep);
          float* maxPtr = (float*) (maxLumImg->imageData + y * maxLumImg->widthStep);
          for (int x=0; x < width; ++x) {
            if (maxPtr[x] != 0) {
              gain_vals[i] = (float) ((log((long double)((imgPtr[x]/maxPtr[x])*(psi-1) + 1.0) / log((long double)psi))));
            }
            else {
              gain_vals[i] = (float) 0.0;
            }
            ++i;
          }
        }

        delete maxLumImg;

        //create a matrix with the values
        cv::Mat gainMatrix(height, width, CV_32FC1, gain_vals);
        delete gain_vals;
        //convert matrix of gain values into an image
        return new IplImage(gainMatrix);

      }// m(IplImage*, int)


      //----------------------------------------------------------------------


      ///<summary>
      ///Takes the given IplImage and returns a normalized version of the
      ///IplImage.  It is assumed the IplImage is of type CV_8UC3.
      ///</summary>
      IplImage* normalizeIplImage(IplImage* img) {
        float* normalizedVals = new float[videoWidth * videoHeight * 3];

        int i = 0;
        for (int y=0; y < videoHeight; ++y) {
          uchar* ptr = (uchar*) (img->imageData + y * img->widthStep);
          for (int x=0; x < videoWidth; ++x) {
            normalizedVals[i++] = (float) ptr[x*3]   / (float) 255.0;
            normalizedVals[i++] = (float) ptr[x*3+1] / (float) 255.0;
            normalizedVals[i++] = (float) ptr[x*3+2] / (float) 255.0;
          }
        }

        //create a matrix with the values
        cv::Mat matrix(videoHeight, videoWidth, CV_32FC3, normalizedVals);
        IplImage* normalizedImage = new IplImage(matrix);
        /*
        cvNamedWindow("orig",CV_WINDOW_AUTOSIZE);
        cvShowImage("orig",normalizedImage);
        cvWaitKey(0);
        cvDestroyWindow("orig");
        */
        IplImage* temp = cvCloneImage(normalizedImage);
        delete normalizedVals;
        //convert the matrix into an image
        return temp;

      }// normalizeIplImage(IplImage*)


      //----------------------------------------------------------------------


      ///<summary>
      ///The spatial bilateral filter.  It is assumed the IplImage is of type
      /// CV_32FC3.
      ///</summary>
      IplImage* B(IplImage* img, double sigma_h, double simgma_i, int kernel_radius) {
        int height_stop = img->height - kernel_radius;
        int width_stop  = img->width  - kernel_radius;
        int widthStep   = img->widthStep;

        //row by row for each kernel center pixel, s
        for (int y = kernel_radius; y < height_stop; ++y) {
          float* s = (float*) (img->imageData + y * widthStep);
          //column by column for each kernel center pixel, s
          for (int x = kernel_radius; x < width_stop; ++x) {
            float sum_numerator   = 0.0;
            float sum_denominator = 0.0;
            //row by row for each neighborhood pixel, p
            for (int i = y - kernel_radius; i < y + kernel_radius; ++i) {
              float* p = (float*) (img->imageData + i * widthStep);
              //column by column for each neighborhood pixel, p
              for (int j = x - kernel_radius; j < x + kernel_radius; ++j) {
                //sum_numerator   += g(dist(i, j), sigma_h) * g(D(I(p), I(s)), sigma_i) * I(p);
                //sum_denominator += g(dist(i, j), sigma_h) * g(D(I(p), I(s)), sigma_i);
              }
            }

          }
        }

        return nullptr;
      }


      //----------------------------------------------------------------------


      ///<summary>
      ///
      ///</summary>
      void calcIntensityAverages() {
        for (int i = 0; i < numFrames; ++i) {
          int j = 0;
          IplImage* img = normalizeIplImage(originalFrames[i]);
          for (int y=0; y < videoHeight; ++y) {
            float* ptr = (float*) (img->imageData + y * img->widthStep);
            for (int x=0; x < videoWidth; ++x) {
              intensity_averages[j++] += ptr[x*3];
              intensity_averages[j++] += ptr[x*3+1];
              intensity_averages[j++] += ptr[x*3+2];
            }
          }
        }

        int length = videoHeight * videoWidth * 3;
        for (int i = 0; i < length; ++i) {
          intensity_averages[i] /= (float) numFrames;
        }
      }


      //----------------------------------------------------------------------


      ///<summary>
      ///
      ///</summary>
      float calcMaxVideoIntensity() {
        float max = 0.0;
        for (int i=0; i < numFrames; ++i) {
          float imgMax = calcMaxImageIntensity(originalFrames[i]);
          if (imgMax > max) {
            max = imgMax;
          }
        }
        return max;
      }


      //----------------------------------------------------------------------


      ///<summary>
      ///
      ///</summary>
      float calcMaxImageIntensity(IplImage* img) {
        float max = 0.0;
        for (int y=0; y < videoHeight; ++y) {
          float* ptr = (float*) (img->imageData + y * img->widthStep);
          for (int x=0; x < videoWidth; ++x) {
            for (int i=0; i < 3; ++i) {
              if (max < ptr[x*3+i]) {
                max = ptr[x*3+i];
              }
            }
          }
        }
        return max;
      }


      //----------------------------------------------------------------------


      ///<summary>
      ///
      ///</summary>
      IplImage* brightenImage(IplImage* img, float gain) {
        float* bright_vals = new float[videoWidth*videoHeight*3];
        int j = 0;
        for (int y=0; y < videoHeight; ++y) {
          float* ptr = (float*) (img->imageData + y * img->widthStep);
          for (int x=0; x < videoWidth; ++x) {
            for (int i=0; i < 3; ++i) {
              bright_vals[j++] = ptr[x*3+i] * gain;
            }
          }
        }

        cv::Mat mat(videoHeight, videoWidth, CV_32FC3, bright_vals);
        delete bright_vals;
        return new IplImage(mat);
      }


      //----------------------------------------------------------------------


      ///<summary>
      ///
      ///</summary>
      void brightenAndDenoise(int temporalMargin, float gain) {
        //open the progress dialog window
        ProgressDialog^ progress = gcnew ProgressDialog(numFrames);
        progress->Show();

        //an array to hold the pixel channel values across the temporal plain
        // in order to average them
        int avgArrayLength = temporalMargin * 2 + 1;
        uchar* avgArray    = new uchar[avgArrayLength];
        //an array to hold the final brightened and denoised values for each
        // frame
        int imgArrayLength = videoHeight * videoWidth * 3;
        uchar* imgArray    = new uchar[imgArrayLength];

        //allocate the enhanced frames
        enhancedFrames = new IplImage*[numFrames];

        //process each frame...
        for (int i=0; i < numFrames; ++i) {
          //initialize start and end frames in the temporal plane
          int start = i - temporalMargin;
          int end   = i + temporalMargin + 1;

          //not enough marginal frames before the current frame
          if (i < temporalMargin) {
            start = 0;
            end += temporalMargin - i;
          }
          //not enough marginal frames after the current frame
          else if (i >= numFrames - temporalMargin) {
            end = numFrames;
            start -= temporalMargin - (numFrames - i - 1);
          }

          int imgIndex = 0;
          //process each row of the frame...
          for (int y=0; y < videoHeight; ++y) {
            //process each pixel in the row...
            for (int x=0; x < videoWidth; ++x) {
              //process each channel of the pixel...
              for (int c=0; c < 3; ++c, ++imgIndex) {
                int avgIndex = 0;
                //get the values of this channel from the other frames within
                // the temporal margin
                for (int j=start; j < end; ++j, ++avgIndex) {
                  uchar channelVal = getValue(originalFrames[j], y, x, c);
                  insert(avgArray, channelVal, 0, avgIndex);
                }
                //calculate the average of these values
                uchar median = calcMedian(avgArray, avgArrayLength);
                //apply the brightness gain factor to the average
                int val = median * gain;
                if (val > 255) {
                  val = 255;
                }
                imgArray[imgIndex] = (uchar) val;
              }
            }
          }

          //create a new image with the brightened and denoised values
          cv::Mat mat(videoHeight, videoWidth, CV_8UC3, imgArray);
          IplImage* temp = new IplImage(mat);
          //copy this image to the global result image array
          enhancedFrames[i] = cvCloneImage(temp);
          delete temp;
          //increment the progress bar
          progress->increment();
        }
        MessageBox::Show("Done.","Enhance");
        delete imgArray;
        delete avgArray;
        delete progress;
      }


      //----------------------------------------------------------------------


      ///<summary>
      ///
      ///<\summary>
      uchar getValue(IplImage* img, int y, int x, int c) {
        uchar* ptr = (uchar*) (img->imageData + y * img->widthStep);
        return ptr[x*3+c];
      }


      //----------------------------------------------------------------------


      ///<summary>
      ///
      ///</summary>
      uchar calcAverage(uchar* arr, int len) {
        int sum = 0;
        for (int i=0; i < len; ++i) {
          sum += arr[i];
        }
        return (uchar) (sum / len);
      }


      //----------------------------------------------------------------------


      ///<summary>
      ///
      ///</summary>
      void insert(uchar* arr, uchar val, int start, int stop) {
        for(int i=start; i <= stop; ++i) {
          if (i == stop) {
            arr[i] = val;
            return;
          }
          if (val < arr[i]) {
            uchar temp = arr[i];
            arr[i] = val;
            insert(arr, temp, i+1, stop);
            return;
          }
        }
      }


      //----------------------------------------------------------------------


      ///<summary>
      ///
      ///</summary>
      uchar calcMedian(uchar* arr, int len) {
        int i = len/2;
        if (len % 2 != 0) {
          return arr[i];
        }
        int temp = arr[i-1]+arr[i];
        return (uchar) (temp/2);
      }


      //----------------------------------------------------------------------


      /*
        for (int y=0; y < height; ++y) {
          float* ptr = (float*) (img->imageData + y * img->widthStep);
          for (int x=0; x < width; ++x) {

          }
        }

        for (int row=0; row < mat.rows; ++row) {
          const float* ptr = (const float*)(mat.data + row * mat.step);
          for (int col=0; col < mat.cols; ++col) {
            
          }
        }
      */

    //end of "Hand-written code (not auto-generated)" region
    #pragma endregion


    //========================================================================


    #pragma region Windows Form Designer generated code

	  private:
		  /// <summary>
		  /// Required designer variable.
		  /// </summary>
		  System::ComponentModel::Container ^components;



	    System::Windows::Forms::PictureBox^  SaveImageButton;
	    System::Windows::Forms::Label^  OpenLabel;
	    System::Windows::Forms::PictureBox^  PreviewImageButton;
	    System::Windows::Forms::Label^  PreviewLabel;
	    System::Windows::Forms::PictureBox^  OpenImageButton;
	    System::Windows::Forms::Label^  SaveLabel;
	    System::Windows::Forms::PictureBox^  exitImageButton;
	    System::Windows::Forms::Label^  ExitLabel;
      System::Windows::Forms::OpenFileDialog^  openFileDialog1;
      System::Windows::Forms::PictureBox^  EnhanceImageButton;
      System::Windows::Forms::Label^  EnhanceLabel;
      System::Windows::Forms::SaveFileDialog^  saveFileDialog1;



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
        this->FileNameLabel = (gcnew System::Windows::Forms::Label());
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
        this->SaveImageButton->Location = System::Drawing::Point(12, 250);
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
        this->OpenLabel->Location = System::Drawing::Point(82, 61);
        this->OpenLabel->Name = L"OpenLabel";
        this->OpenLabel->Size = System::Drawing::Size(61, 23);
        this->OpenLabel->TabIndex = 1;
        this->OpenLabel->Text = L"Open.";
        // 
        // PreviewImageButton
        // 
        this->PreviewImageButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"PreviewImageButton.Image")));
        this->PreviewImageButton->Location = System::Drawing::Point(12, 180);
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
        this->PreviewLabel->Location = System::Drawing::Point(82, 200);
        this->PreviewLabel->Name = L"PreviewLabel";
        this->PreviewLabel->Size = System::Drawing::Size(77, 23);
        this->PreviewLabel->TabIndex = 3;
        this->PreviewLabel->Text = L"Preview.";
        // 
        // OpenImageButton
        // 
        this->OpenImageButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"OpenImageButton.Image")));
        this->OpenImageButton->Location = System::Drawing::Point(12, 40);
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
        this->SaveLabel->Location = System::Drawing::Point(82, 268);
        this->SaveLabel->Name = L"SaveLabel";
        this->SaveLabel->Size = System::Drawing::Size(52, 23);
        this->SaveLabel->TabIndex = 5;
        this->SaveLabel->Text = L"Save.";
        // 
        // exitImageButton
        // 
        this->exitImageButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"exitImageButton.Image")));
        this->exitImageButton->Location = System::Drawing::Point(12, 320);
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
        this->ExitLabel->Location = System::Drawing::Point(82, 346);
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
        this->EnhanceImageButton->Location = System::Drawing::Point(12, 110);
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
        this->EnhanceLabel->Location = System::Drawing::Point(82, 127);
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
        // FileNameLabel
        // 
        this->FileNameLabel->AutoSize = true;
        this->FileNameLabel->Location = System::Drawing::Point(12, 13);
        this->FileNameLabel->Name = L"FileNameLabel";
        this->FileNameLabel->Size = System::Drawing::Size(35, 13);
        this->FileNameLabel->TabIndex = 10;
        this->FileNameLabel->Text = L"label1";
        this->FileNameLabel->Visible = false;
        // 
        // Form1
        // 
        this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
        this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        this->BackColor = System::Drawing::SystemColors::Control;
        this->ClientSize = System::Drawing::Size(222, 393);
        this->Controls->Add(this->FileNameLabel);
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
        this->Location = System::Drawing::Point(5, 5);
        this->Name = L"Form1";
        this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
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

      System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {}

    //end of "Windows Form Designer generated code" region
    #pragma endregion
  };
}