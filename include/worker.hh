#ifndef DS_WORKER_H_
#define DS_WORKER_H_

#include "watershed.hh"
#include "utils.hh"
#include <map>

namespace WORKER {

class Worker {
  private:
    cv::Mat markers_;

    cv::Mat recolor_{};

    int nareas_;

    Utils::ADJACENT::AdjacentTables table_;

    int *size_;

    Utils::mySTL::MapChunk *chunks_;

    void GetSize();

    bool CheckInBound(int x, int y);

    void Highlight(const int &n, int *idxs);

    int SearchLowerBound(const int &x);

    int SearchUpperBound(const int &x);
    
  public:

    explicit Worker() = default;

    explicit Worker(const cv::Mat &markers, const int &nareas);

    ~Worker();

    void ProcessAdjecent();

    void ReColor();

    void SortbySize();

    void Queries();

};

};

#endif