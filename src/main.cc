#include <cstdio>
#include "poisson_samples.hh"
#include "watershed.hh"

int main() {
    IMAGE::ImageProcessor processor("../icon.jpg", 100);
    processor.DisplaySeedsOnImage("cpy");
    processor.PerformWaterShed();
    processor.DisplayResultOutput();
    return 0;
}