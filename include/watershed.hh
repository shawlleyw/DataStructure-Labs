#ifndef DS_WATERSHED_H_
#define DS_WATERSHED_H_

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/core.hpp>
#include <vector>
#include "poisson_samples.hh"
#include "utils.hh"

namespace IMAGE {

class ImageProcessor {
  private:

    cv::Mat image_;

    cv::Mat seeds_image_;

    cv::Mat markers_{};
    
    cv::Mat output_{};

    // ! index start from 1
    int comp_count_{1};

    int kseeds_;

    Seeds::Point *seeds_;

     cv::RNG rng_;

    void DisplayImage(const cv::Mat &image, const char *image_name);
    
    void InitMarkers();

  public:
    
    ImageProcessor(const char *filename, int kseeds);

    ~ImageProcessor() = default;

    void DisplayResultImage(const char *image_name);

    void DisplaySeedsOnImage(const char *image_name);

    void DoWaterShed();

    void PerformWaterShed();

    void ProcessOutput();

    void PrintResultOutput();

};

};


#endif