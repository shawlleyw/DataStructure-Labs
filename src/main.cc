#include <cstdio>
#include "poisson_samples.hh"
#include "watershed.hh"

int main() {
    IMAGE::ImageProcessor processor("../icon.jpg", 200);
    processor.DisplaySeedsOnImage("cpy");
    return 0;
}