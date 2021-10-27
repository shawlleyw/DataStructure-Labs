#include "watershed.hh"
#include <cstdlib>
#include <chrono>

using namespace IMAGE;

ImageProcessor::ImageProcessor(const char *filename, int kseeds) : kseeds_(kseeds) {
    image_ = cv::imread(filename);
    seeds_ = new Seeds::Point[kseeds_];
    printf ("row: %d  col: %d\n", image_.rows, image_.cols);

    Seeds::PoissonSample samples(image_.cols, image_.rows, kseeds_);
    auto st = std::chrono::steady_clock::now();
    samples.GenerateSamples();
    auto ed = std::chrono::steady_clock::now();
    auto cost = std::chrono::duration_cast<std::chrono::duration<double>> (ed-st);
    samples.PrintNumberResult();
    printf("%.6lfs\n", cost.count());

    memcpy(seeds_, samples.GetResult(), 1ll * sizeof(Seeds::Point) * kseeds_);
    seeds_image_ = image_.clone();
    for(int i = 0; i < kseeds_; i++) {
        cv::circle(seeds_image_, cv::Point(seeds_[i].x, seeds_[i].y), 3, cv::Scalar::all(255), -1);
    }
}

void ImageProcessor::DisplayImage(const cv::Mat &image, const char *image_name) {
    // cv::imshow(image_name, image);
    // cv::waitKey(0);
}

void ImageProcessor::DisplayResultImage(const char *image_name) {
    DisplayImage(image_, image_name);
}

void ImageProcessor::DisplaySeedsOnImage(const char *image_name) {
    cv::Mat output_image = seeds_image_.clone();
    
    char *location = new char[100];
    for(int i = 0; i < kseeds_; i++) {
        sprintf(location, "%d", i);
        putText(output_image, location, cv::Point(seeds_[i].x, seeds_[i].y), 6, 0.3, cv::Scalar(255, 0, 0));
    }
    DisplayImage(output_image, image_name);
    
    delete [] location;
    
}
void ImageProcessor::InitMarkers() {

    cv::Mat marker_mask(image_.size(), 8, 1);
    cv::cvtColor(seeds_image_, marker_mask, cv::COLOR_BGR2GRAY);
	GaussianBlur(marker_mask,marker_mask, cv::Size(5,5),2);   
	Canny(marker_mask, marker_mask, 80, 150);
    
    // ! perfomr findContours
    // ! with using std::vector
    std::vector<std::vector<cv::Point>> contours;  
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(marker_mask, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE, cv::Point());  

    // ! perform drawContours
	cv::Mat contours_output = cv::Mat::zeros(image_.size(), CV_8UC1);  	
	markers_.create(image_.size(), CV_32S);   
	markers_ = cv::Scalar::all(0);
	for(int index = 0; index >= 0; index = hierarchy[index][0], comp_count_++ ) {
		cv::drawContours(markers_, contours, index, cv::Scalar::all(comp_count_), 1, 8, hierarchy);
		cv::drawContours(contours_output, contours, index, cv::Scalar::all(255), 1, 8, hierarchy);  
	}

    DisplayImage(contours_output, "contours_output");

    cv::Mat markers_output;
    cv::convertScaleAbs(markers_, markers_output);
    DisplayImage(markers_output, "markers");
}

void ImageProcessor::ProcessOutput() {
    output_.create(image_.size(), CV_8UC3);

    cv::Mat random_color(1, comp_count_, CV_8UC3);
    for(int i = 0; i < comp_count_; i++) {
        uchar* ptr = random_color.data + i*3;
        ptr[0] = static_cast<uchar>(rng_.uniform(50, 200));
        ptr[1] = static_cast<uchar>(rng_.uniform(50, 200));
        ptr[2] = static_cast<uchar>(rng_.uniform(50, 200));
    }

    for(int i = 0; i < markers_.rows; i++ ) {
        for(int j = 0; j < markers_.cols; j++ ) {
            int index = markers_.at<int>(i, j);
            if(index == -1) {
                output_.at<cv::Vec3b>(i, j)[0] = 255;
                output_.at<cv::Vec3b>(i, j)[1] = 255;
                output_.at<cv::Vec3b>(i, j)[2] = 255;
            } else {
                output_.at<cv::Vec3b>(i, j)[0] = random_color.at<cv::Vec3b>(1, index)[0];
                output_.at<cv::Vec3b>(i, j)[1] = random_color.at<cv::Vec3b>(1, index)[1];
                output_.at<cv::Vec3b>(i, j)[2] = random_color.at<cv::Vec3b>(1, index)[2];
            }
        }
   }
   cv::addWeighted(output_, 0.5f, image_, 0.5f, 0, output_);
}

void ImageProcessor::PerformWaterShed() {
    InitMarkers();
    cv::watershed(image_, markers_);
    cv::Mat watershed_output;
	cv::convertScaleAbs(markers_, watershed_output);
	DisplayImage(watershed_output, "Watershed");
    ProcessOutput();
}

void ImageProcessor::DisplayResultOutput() {
    DisplayImage(output_, "process_output");
}