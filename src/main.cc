#include <cstdio>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "poisson_samples.hh"
int main() {
    Seeds::PoissonSample sample_gen(500, 200, 500);
    sample_gen.GenerateSamples();
    sample_gen.PrintGraphResult();
    return 0;
}