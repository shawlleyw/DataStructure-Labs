#include "poisson_samples.hh"

using namespace Seeds;

PoissonSample::PoissonSample(int n, int m, int k, int round): row_(n), column_(m), kpoints_(k), round_count_(round) { 
    result_ = new Point[k];
    memset(result_, 0, sizeof(Point) * k);
    dist_ = sqrt(n*m/k)*0.8;
    vis_ = new bool[n*m]{false};
    //printf("%f\n", dist_);
}

PoissonSample::~PoissonSample() {
    delete [] result_;
    delete [] vis_;
}

Point *PoissonSample::GetResult() {
    return this->result_;
}

void PoissonSample::PrintNumberResult() {
    for(int i = 0; i < kpoints_; i++) {
        printf("%d: (%d, %d)\n", i, result_[i].x, result_[i].y);
    }
}

void PoissonSample::PrintGraphResult() {
    for(int i = 0; i < row_; i++) {
        for(int j = 0; j < column_; j++){
            putchar(vis_[GetPos(i, j)] ? '#' : '.');
        }
        putchar('\n');
    }
}

void PoissonSample::PushResult(const Point &p) {
    result_[cnt_result_++] = p;
}

void PoissonSample::PushVis(const Point &p) {
    vis_[GetPos(p.x, p.y)] = true;
}

float PoissonSample::GetDist(const int &nx, const int &ny, const Point &p) {
    int sqr_dist = (nx-p.x) * (nx-p.x) + (ny-p.y) * (ny-p.y);
    return sqrt(1.0f*sqr_dist);
}

Point PoissonSample::RandomRound(const Point &p) {
    int nx{}, ny{};
    do {
        float dis = rng.uniform(dist_, dist_*2.0f);
        float angle = rng.uniform(0.0f, 2.0f*pi_);
        nx = static_cast<int>(p.x+dis*cos(angle));
        ny = static_cast<int>(p.y+dis*sin(angle));
    } while(!InMat(nx, ny));
    return Point(nx, ny);
}

bool PoissonSample::CheckNeighbor(const Point &p) {
    int up = p.x - dist_ >= 0 ? p.x - dist_ : 0;
    int down = p.x + dist_ < row_ ? p.x + dist_ : row_;
    int left = p.y - dist_ >= 0 ? p.y - dist_ : 0;
    int right = p.y + dist_ < column_ ? p.y + dist_ : column_;
    for(int i = up; i <= down; i++) {
        for(int j = left; j <= right; j++) {
            if(GetDist(i, j, p) < dist_ && vis_[GetPos(i, j)]) {
                return false;
            }
        }
    }
    return true;
}

void PoissonSample::GenerateSamples() {
    Utils::mySTL::Queue<Point> process_list(row_*column_);
    Point first(rng.uniform(0, row_), rng.uniform(0, column_));
    process_list.push(first);
    PushResult(first);
    PushVis(first);
    while(!process_list.empty() && cnt_result_ < kpoints_) {
        int idx = rng.uniform(0, process_list.len());
        Point pcur = process_list.at(idx);
        process_list.erase(idx);
        for(int i = 0; i < round_count_; i++) {
            Point pnew = RandomRound(pcur);
            if(CheckNeighbor(pnew)) {
                //printf("%d %d\n", pnew.x, pnew.y);
                process_list.push(pnew);
                PushResult(pnew);
                PushVis(pnew);
            }
        }
    }
}