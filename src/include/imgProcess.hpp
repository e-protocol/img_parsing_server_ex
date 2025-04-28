#pragma once
#include "opencv4/opencv2/objdetect.hpp"
#include "opencv4/opencv2/highgui.hpp"
#include "opencv4/opencv2/imgproc.hpp"
#include <vector>
#include <stdexcept>
#include "misc.hpp"
/**
 * @brief base builder class for creating task sub-classes
 * @details Process method is used to precocess class task
 */
class BaseDetector
{
public:
  virtual void Blur(cv::Mat& img) = 0;
  virtual void Recognition(cv::Mat& img) = 0;
  virtual bool ShowImg(cv::Mat& img) = 0;
  virtual void Process() = 0;
  virtual ~BaseDetector(){}
};

//-----------------------------------------------------------------------------------------------------
/**
 * @brief base class face recognition for task sub-classes 
 */
class FaceDetector : public BaseDetector
{
public:
  virtual ~FaceDetector(){}

  /**
   * @brief get processed file as raw data
   */
  const char* GetImg() { return data_; }

protected:
  const char* data_ = nullptr;

  void Recognition(cv::Mat& img) override
  {
    cv::CascadeClassifier cascade;
    std::string path = "haarcascade_frontalface_alt.xml";
    cascade.load(path);
    DetectAndDraw(img, cascade, 1.0);
  }

  void DetectAndDraw(cv::Mat& img, cv::CascadeClassifier& cascade, double scale)
  {
    using namespace cv;
    std::vector<Rect> faces;
    Mat gray, small_img;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    double fx = 1.0 / scale;

    //resize grayscale image before detection
    resize(gray, small_img, Size(), fx, fx, INTER_LINEAR);
    equalizeHist(small_img, small_img);
    cascade.detectMultiScale(small_img, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(50, 50), Size(200, 200));

    //draw rectangle on detected faces
    for(size_t i = 0; i < faces.size(); ++i)
    {
      Rect r = faces[i];
      Scalar color = Scalar(250, 0, 0);
      rectangle(img, r, color, 3, 8, 0);
    }
  }

  void Blur(cv::Mat& img)
  {
    
  }

  bool ShowImg(cv::Mat& img)
  {
    cv::imshow("Display Window", img);
		int key = cv::waitKey('q');
    return key == 'q';
  }
};

//-----------------------------------------------------------------------------------------------------
/**
 * @brief class task for face recognition from webcam stream
 */
class FaceDetectorWebcam : public FaceDetector
{
public:
/**
 * @brief process face recognition from webcam stream in constructor
 */
  void Process()
  {
    PrintTerminal("press q to close webcam window");
    try
    {
      cv::Mat img;
      cv::VideoCapture capture;
      capture.open(0);

      if(!capture.isOpened())
        throw std::runtime_error("webcam error");

      while(true)
      {
        capture >> img;
        Recognition(img);
        bool ok = ShowImg(img);
        if(ok) break;
      }
    }
    catch(const std::exception& e){ throw::std::runtime_error(e.what()); }
  }
};

//-----------------------------------------------------------------------------------------------------
/**
 * @brief class task for face recognition and bluring from img
 * @details constructor applies path to an image file
 */
class FaceDetectorImg : public FaceDetector
{
  std::string& path_;
public:
  FaceDetectorImg(std::string& path) : path_(path){}
/**
 * @brief process face recognition and blur from image in constructor
 */
  void Process() override
  {
    if(path_.empty()) return;
		try
    {
      cv::Mat img = cv::imread(path_, cv::IMREAD_COLOR);
      Recognition(img);
      Blur(img);
      bool ok = ShowImg(img);
    }
    catch(const std::exception& e){ throw::std::runtime_error(e.what()); }
  }
};