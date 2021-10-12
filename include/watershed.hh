#ifndef DS_WATERSHED_H_
#define DS_WATERSHED_H_

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include "poisson_samples.hh"
#include "utils.hh"

namespace IMAGE {

class ImageProcessor {
  private:

    cv::Mat image_;

    cv::Mat markers_{};

    int kseeds_;

    Seeds::Point *seeds_;

    void DisplayImage(cv::Mat image, const char *image_name);

  public:
    
    ImageProcessor(const char *filename, int kseeds);

    ~ImageProcessor() = default;

    void DisplayResultImage(const char *image_name);

    void DisplaySeedsOnImage(const char *image_name);

};

};


#endif