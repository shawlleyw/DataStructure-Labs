#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include "poisson_samples.hh"
#include "watershed.hh"

void CheckFile(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if(fd == -1) {
        fprintf(stderr, "FATAL: file %s not existed\n", filename);
        exit(-1);
    }
}

void CheckSeeds(int nseeds) {
    if(nseeds <= 0 || nseeds > 3000) {
        fprintf(stderr, "FATAL: number of seeds (%d) out of range\n", nseeds);
        exit(-1);
    }
}

int main(int argc, char ** argv) {
    if(argc < 3) {
        fprintf(stderr, "FATAL: lack of arguments\n");
        exit(-1);
    }
    CheckFile(argv[1]);
    CheckSeeds(atoi(argv[2]));
    printf("%d\n", atoi(argv[2]));
    try {
        IMAGE::ImageProcessor processor(argv[1], atoi(argv[2]));
        processor.DisplaySeedsOnImage("seeds");
        processor.PerformWaterShed();
        processor.DisplayResultOutput();
    } catch(int err) {
        fprintf(stderr, "FATAL: %s\n", strerror(err));
    }
    
    return 0;
}