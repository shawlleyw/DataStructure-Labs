#ifndef DS_POISSON_H_
#define DS_POISSON_H_

#include <cstdio>
#include <cmath>
#include <cstring>
#include <opencv2/imgproc.hpp>

#include "utils.hh"

namespace Seeds {


// ! point in matrix
struct Point{

    int x;
    
    int y;

    Point() = default;
     
    Point(int nx, int ny) : x(nx), y(ny) { }

};

// ! poisson-disk samples implement
class PoissonSample {
  private:
    
    constexpr static const float pi_ = 3.14158f;

    int row_;

    int column_;

    int kpoints_;

    Point *result_;

    int cnt_result_{};

    float dist_;

    int round_count_;

    bool *vis_;

    cv::RNG rng;

    int GetRow(const int &pos) {
        if(!row_) return -1;
        return pos / column_;
    }

    int GetLine(const int &pos) {
        if(!row_) return -1;
        return pos % column_;
    }

    int GetPos(const int &row, const int &column) {
        return row * column_ + column;
    }

    bool InMat(const int &x, const int &y) {
        return x >= 0 && x < row_ && y >= 0 && y < column_;
    }

    Point RandomRound(const Point &p);

    bool CheckNeighbor(const Point &p);

    void PushResult(const Point &p);

    void PushVis(const Point &p);

    float GetDist(const int &nx, const int &ny, const Point &p);

  public:

    explicit PoissonSample(int n, int m, int k, int round = 100);

    ~PoissonSample();

    // ! generate k seeds
    void GenerateSamples();

    // ! get the array of k seeds
    Point *GetResult();

    // ! print result in values
    void PrintNumberResult();

    // ! print result in graph
    void PrintGraphResult();

};

}

#endif