#include "poisson_samples.hh"

using namespace Seeds;

PoissonSample::PoissonSample(int n, int m, int k, int round): row_(n), column_(m), kpoints_(k), round_count_(round) { 
    result_ = new Point[k];
    memset(result_, 0, sizeof(Point) * k);
    radius_ = sqrt(1.7f*n*m/k/pi_);
    vis_ = new bool[n*m]{false};
    //printf("%f\n", radius_);
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

void PoissonSample::PushResult(Point p) {
    result_[cnt_result_++] = p;
}

void PoissonSample::PushVis(Point p) {
    vis_[GetPos(p.x, p.y)] = true;
}

float PoissonSample::GetDist(int nx, int ny, Point p) {
    int sqr_dist = (nx-p.x) * (nx-p.x) + (ny-p.y) * (ny-p.y);
    return sqrt(1.0f*sqr_dist);
}

Point PoissonSample::RandomRound(Point p) {
    int nx{}, ny{};
    do {
        int temp = rg.RangeInt(0, static_cast<int>(radius_));
        float dis = radius_ + radius_ * radius_ / (radius_ + temp);

        int t1 = rg.RangeInt(1, static_cast<int>(radius_)), t2 = rg.RangeInt(1, static_cast<int>(radius_));
        float angle = 2.0 * t1 / (t1 + t2) * pi_;
    
        nx = static_cast<int>(p.x+dis*cos(angle));
        ny = static_cast<int>(p.y+dis*sin(angle));
    } while(!InMat(nx, ny));
    return Point(nx, ny);
}

bool PoissonSample::CheckNeighbor(Point p) {
    int up = p.x - radius_ >= 0 ? p.x - radius_ : 0;
    int down = p.x + radius_ < row_ ? p.x + radius_ : row_;
    int left = p.y - radius_ >= 0 ? p.y - radius_ : 0;
    int right = p.y + radius_ < column_ ? p.y + radius_ : column_;
    for(int i = up; i <= down; i++) {
        for(int j = left; j <= right; j++) {
            if(GetDist(i, j, p) < radius_ && vis_[GetPos(i, j)]) {
                return false;
            }
        }
    }
    return true;
}

void PoissonSample::GenerateSamples() {
    Utils::Queue<Point> process_list(row_*column_);
    Point first(rg.RangeInt(0, row_), rg.RangeInt(0, column_));
    process_list.push(first);
    PushResult(first);
    PushVis(first);
    while(!process_list.empty() && cnt_result_ < kpoints_) {
        int idx = rg.RangeInt(0, process_list.len());
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