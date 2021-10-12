#ifndef DS_POISSON_H_
#define DS_POISSON_H_

#include <cstdio>
#include <cmath>

#include "utils.hh"

namespace Seeds {
struct Point{

    int x;
    
    int y;

    Point() = default;
     
    Point(int nx, int ny) : x(nx), y(ny) { }

};

class PoissonSample {
  private:
    
    int row_;

    int column_;

    int kpoints_;

    Point *result_;

    int cnt_result_{};

    float radius_;

    int round_count_;

    bool *vis_;

    Utils::RandomGenerator rg;

    int GetRow(int pos) {
        if(!row_) return -1;
        return pos / column_;
    }

    int GetLine(int pos) {
        if(!row_) return -1;
        return pos % column_;
    }

    int GetPos(int row, int column) {
        return row * column_ + column;
    }

    bool InMat(int x, int y) {
        return x >= 0 && x < row_ && y >= 0 && y < column_;
    }

    Point RandomRound(Point p);

    bool CheckNeighbor(Point p);

    void PushResult(Point p);

    void PushVis(Point p);

    float GetDist(int nx, int ny, Point p);

  public:

    explicit PoissonSample(int n, int m, int k, int round = 30);

    ~PoissonSample();

    void GenerateSamples();

    Point *GetResult();

    void PrintNumberResult();

    void PrintGraphResult();

};

}

#endif