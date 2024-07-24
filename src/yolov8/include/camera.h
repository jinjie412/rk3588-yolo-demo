#pragma once
#include "opencv2/opencv.hpp"
#include <memory>  // For std::unique_ptr
#include <string>  // For std::string

class Camera {
 public:
  Camera(const std::string& rtsp_url, cv::Size size, double framerate);
  ~Camera();
  std::unique_ptr<cv::Mat> GetNextFrame();

 private:
  cv::Size size_;
  cv::VideoCapture capture_;
};
