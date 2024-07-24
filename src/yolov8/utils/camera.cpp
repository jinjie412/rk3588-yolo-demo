#include "camera.h"

#include "kaylordut/log/logger.h"
#include "thread"

Camera::Camera(const std::string& rtsp_url, cv::Size size, double framerate)
    : capture_(rtsp_url), size_(size) {
  KAYLORDUT_LOG_INFO("Instantiate a Camera object with RTSP URL: {}", rtsp_url);
  // Initialize the camera with RTSP URL
  if (!capture_.isOpened()) {
    KAYLORDUT_LOG_ERROR("Error opening video stream or file");
    exit(EXIT_FAILURE);
  }
  capture_.set(cv::CAP_PROP_FOURCC,
               cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
  // Check if format is successfully set
  int fourcc = capture_.get(cv::CAP_PROP_FOURCC);
  if (fourcc != cv::VideoWriter::fourcc('M', 'J', 'P', 'G')) {
    KAYLORDUT_LOG_WARN("Set video format failed");
  }
  capture_.set(cv::CAP_PROP_FRAME_WIDTH, size_.width);
  capture_.set(cv::CAP_PROP_FRAME_HEIGHT, size_.height);
  if (!capture_.set(cv::CAP_PROP_FPS, framerate)) {
    KAYLORDUT_LOG_WARN("Set framerate failed!!");
  }
  std::this_thread::sleep_for(std::chrono::seconds(1));
  KAYLORDUT_LOG_INFO("Camera width: {}, height: {}, fps: {}",
                     capture_.get(cv::CAP_PROP_FRAME_WIDTH),
                     capture_.get(cv::CAP_PROP_FRAME_HEIGHT),
                     capture_.get(cv::CAP_PROP_FPS));
}

Camera::~Camera() {
  if (capture_.isOpened()) {
    KAYLORDUT_LOG_INFO("Release camera");
    capture_.release();
  }
}

std::unique_ptr<cv::Mat> Camera::GetNextFrame() {
  auto frame = std::make_unique<cv::Mat>();
  capture_ >> *frame;
  if (frame->empty()) {
    KAYLORDUT_LOG_ERROR("Get frame error");
    return nullptr;
  }
  return std::move(frame);
}
