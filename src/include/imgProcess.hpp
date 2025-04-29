#pragma once
#include "opencv4/opencv2/objdetect.hpp"
#include "opencv4/opencv2/highgui.hpp"
#include "opencv4/opencv2/imgproc.hpp"
#include "opencv4/opencv2/imgcodecs.hpp"
#include <vector>
#include <stdexcept>
#include <map>
#include <fstream>
#include "misc.hpp"
/**
 * @brief base builder class for creating task sub-classes
 * @details Process method is used to precocess class task
 */
class BaseDetector
{
public:
  enum CasdaceType
  {
    FRONTAL_FACE_DEFAULT,
    FRONTAL_FACE_ALT,
    FRONTAL_CAT_FACE,
    FRONTAL_FACE_ALT2,
    FRONTAL_FACE_ALT_TREE
  };

  virtual void Blur(cv::Mat& img) = 0;
  virtual void Recognition(cv::Mat& img, CasdaceType t) = 0;
  virtual bool ShowFrame(const cv::Mat& img) = 0;
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

  FaceDetector()
  {
    classifiers_[FRONTAL_FACE_DEFAULT] = "haarcascade_frontalface_default.xml";
    classifiers_[FRONTAL_FACE_ALT] = "haarcascade_frontalface_alt.xml";
    classifiers_[FRONTAL_CAT_FACE] = "haarcascade_frontalcatface.xml";
    classifiers_[FRONTAL_FACE_ALT2] = "haarcascade_frontalface_alt2.xml";
    classifiers_[FRONTAL_FACE_ALT_TREE] = "haarcascade_frontalface_alt_tree.xml";
  }

protected:
  std::map<CasdaceType, std::string> classifiers_;
  std::vector<cv::Rect> faces_;

  /**
   * @brief method to do object detection
   */
  void Recognition(cv::Mat& img, CasdaceType t) override
  {
    auto it = classifiers_.find(t);
    if(it == classifiers_.end()) throw std::range_error("unkonwon classifier");
    std::string cl_file = it->second;
    cv::CascadeClassifier cl;
    cl.load(cl_file);
    DetectAndDraw(img, cl, 1.0);
  }

  /**
   * @brief method to find all objects on frame acording to classifier
   */
  void DetectAndDraw(cv::Mat& img, cv::CascadeClassifier& cascade, double scale)
  {
    using namespace cv;
    faces_.clear();
    cascade.detectMultiScale(img, faces_, 1.3, 5);

    //draw rectangle on detected faces
    for(size_t i = 0; i < faces_.size(); ++i)
    {
      Rect r = faces_[i];
      Scalar color = Scalar(250, 0, 0);
      rectangle(img, r, color, 3, 8, 0);
    }
  }

  /**
   * @brief method to blur objects
   */
  void Blur(cv::Mat& img) override
  {
    for(auto& r : faces_)
      cv::GaussianBlur(img(r), img(r), cv::Size(0, 0), 20, 0);
  }

  /**
   * @brief method to show frame in new window
   */
  bool ShowFrame(const cv::Mat& img) override
  {
    cv::imshow("Display Window", img);
    return cv::waitKey('q') == 'q';
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
 * @brief process face recognition from webcam stream
 */
  FaceDetectorWebcam(CasdaceType t)
  {
    try
    {
      cv::Mat img;
      cv::VideoCapture capture;
      capture.open(0);

      if(!capture.isOpened())
        throw std::runtime_error("webcam error");

      PrintTerminal("press q to close Display Window");
      while(true)
      {
        capture >> img;
        Recognition(img, t);
        if(ShowFrame(img)) break;
      }
      cv::destroyWindow("Display Window");
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
public:

/**
 * @brief process face recognition from saved img
 */
  FaceDetectorImg(const std::string& path, const std::string& filename, CasdaceType t) 
  {
    try
    {
      if(path.empty() || filename.empty()) throw std::runtime_error("Invalid path or filename");
      cv::Mat img = cv::imread(path + "/" + filename, cv::IMREAD_COLOR);
      Recognition(img, t);
      Blur(img);
      Save(img, path, filename);
    }
    catch(const std::exception& e){ throw::std::runtime_error(e.what()); }
  }

  void Save(cv::Mat& img, const std::string& path, const std::string& filename)
  {
    cv::imwrite(path + "/" + filename, img);
  }
};