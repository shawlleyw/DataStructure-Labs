#include "watershed.hh"
#include <cstdlib>

using namespace IMAGE;
using namespace cv;

ImageProcessor::ImageProcessor(const char *filename, int kseeds) : kseeds_(kseeds) {
    image_ = imread(filename);
    seeds_ = new Seeds::Point[kseeds];
    //printf ("row: %d  col: %d\n", image_.rows, image_.cols);
    Seeds::PoissonSample samples(image_.rows, image_.cols, kseeds);
    
    samples.GenerateSamples();
    samples.PrintNumberResult();
    memcpy(seeds_, samples.GetResult(), sizeof(Seeds::Point) * kseeds);
}

void ImageProcessor::DisplayImage(Mat image, const char *image_name) {
    imshow(image_name, image);
    waitKey(0);
}

void ImageProcessor::DisplayResultImage(const char *image_name) {
    DisplayImage(image_, image_name);
}

void ImageProcessor::DisplaySeedsOnImage(const char *image_name) {
    for(int i = 0; i < kseeds_; i++) {
        circle(image_, cv::Point(seeds_[i].x, seeds_[i].y), 3, Scalar(128, 128, 128), -1);
    }
    DisplayResultImage(image_name);

}