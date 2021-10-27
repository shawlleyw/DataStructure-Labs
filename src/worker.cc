#include "worker.hh"

using namespace WORKER;

Worker::Worker(const cv::Mat &markers, const int &nareas) : markers_(markers), nareas_(nareas), table_(nareas), size_(new int[nareas]), chunks_(new Utils::mySTL::MapChunk[nareas_]) { }

Worker::~Worker() { delete[] size_; delete[] chunks_; }

bool Worker::CheckInBound(int x, int y) {
    return x >= 0 && x < markers_.rows && y >= 0 && y < markers_.cols;
}

void Worker::ProcessAdjecent() {
    int dx[] = {0, 0, 1, 1, 1, -1, -1, -1};
    int dy[] = {1, -1, -1, 0, 1, -1, 0, 1};
    std::map<std::pair<int, int>, bool> mp{};
    
    for(int i = 0; i < markers_.rows; i++) {
        for(int j = 0; j < markers_.cols; j++) {
            if(markers_.at<int>(i, j) != -1) {
                continue;
            }
            for(int lstep = 0; lstep < 8; lstep++) {
                int lx = i + dx[lstep];
                int ly = j + dy[lstep];
                if(!CheckInBound(lx, ly)) {
                    continue;
                }
                int lp = markers_.at<int>(lx, ly);
                if(lp == -1 ) {
                    continue;
                }
                for(int rstep = 0; rstep < lstep; rstep++) {
                    int rx = i + dx[rstep];
                    int ry = j + dy[rstep];
                    if(!CheckInBound(rx, ry)) {
                    continue;
                }
                    int rp = markers_.at<int>(rx, ry);
                    if(rp == -1 || rp == lp || mp[std::make_pair(lp, rp)]){ 
                        continue;
                    }
                    mp[std::make_pair(lp, rp)] = true;
                    mp[std::make_pair(rp, lp)] = true;
                    table_.Link(lp, rp);
                    table_.Link(rp, lp);
                }   
            }    
        }
    }
    
}

void Worker::ReColor() {
    int *colors = new int[nareas_];
    bool *vis = new bool[nareas_];
    bool which_color[5];
    for(int i = 0; i < nareas_; i++) {
        colors[i] = -1;
        vis[i] =false;
    }
    Utils::mySTL::Queue<int>q(nareas_);
    q.push(1);
    vis[1] = true;
    while (!q.empty()) {
        int cur_area = q.front();
        q.pop();
        
        for(int i = 1; i <= 4; i++) {
            which_color[i] = false;
        }

        for(Utils::ADJACENT::node *u = table_.head(cur_area); u != nullptr; u = u->next) {
            if(!vis[u->to]) {
                q.push(u->to);
                vis[u->to] = true;
            }
            if(colors[u->to] != -1) {
                which_color[colors[u->to]] = true;
            }
        }
        for(int i = 1; i <=4; i++) {
            if(!which_color[i]) {
                colors[cur_area] = i;
                break;
            }
        }
        if(colors[cur_area] == -1) {
            colors[cur_area] = 0;
        }
    }

    unsigned char re_color[5][3];
    for(int i = 1; i <= 4; i++) {
        re_color[i][0] = 0;
        re_color[i][1] = 0;
        re_color[i][2] = 0;
    }
    re_color[0][0] = 255;
    re_color[0][1] = 255;
    re_color[0][2] = 255;
    re_color[1][0] = 255;
    re_color[2][1] = 255;
    re_color[3][2] = 255;
    re_color[4][0] = 128;
    re_color[4][1] = 128;
    re_color[4][2] = 128;

    recolor_ = cv::Mat::zeros(markers_.size(), CV_8UC3);

    for(int i = 0; i < markers_.rows; i++) {
        for(int j = 0; j < markers_.cols; j++) {
            int idx = markers_.at<int>(i, j);
            if(idx == -1) {
                continue;
            }
            int col = colors[idx];
            if(col == 0) {
                col = 4;
            }
            recolor_.at<cv::Vec3b>(i, j)[0] = re_color[col][0];
            recolor_.at<cv::Vec3b>(i, j)[1] = re_color[col][1];
            recolor_.at<cv::Vec3b>(i, j)[2] = re_color[col][2];
        }
    }
    cv::imshow("recolor", recolor_);
    cv::waitKey(0);
    delete[] colors;
}

void Worker::GetSize() {
    for(int i = 0; i < nareas_; i++) {
        size_[i] = 0;
    }
    for(int i = 0; i < markers_.rows; i++ ) {
        for(int j = 0; j < markers_.cols; j++ ) {
            int index = markers_.at<int>(i, j);
            if(index != -1) {
                size_[index] ++;
            }
        }
    }
}

void Worker::SortbySize() {
    GetSize();
    Utils::mySTL::Heap hp(nareas_);
    for(int i = 0; i < nareas_; i++) {
        hp.Push(size_[i], i);
    }

    for(int i = 0; !hp.Empty(); i++) {
        chunks_[i] = hp.top();
        hp.Pop();
    }
    printf("Min Area: idx: %d, size: %d\n", chunks_[0].idx, chunks_[0].size);
    printf("Max Area: idx: %d, size: %d\n", chunks_[nareas_-1].idx, chunks_[nareas_-1].size);
}

void Worker::Highlight(const int &n, int *idxs) {
    cv::Mat output = recolor_.clone();
    
    unsigned char rgb[] = {0, 0, 0};

    //#pragma omp parallel for
    for(int c = 0; c < output.cols; c++) {
        for(int i = 0; i < n; i++) {
            for(int r = 0; r < output.rows; r++) {
                if(markers_.at<int>(r, c) == idxs[i]) {
                    output.at<cv::Vec3b>(r, c)[0] = rgb[0];
                    output.at<cv::Vec3b>(r, c)[1] = rgb[1];
                    output.at<cv::Vec3b>(r, c)[2] = rgb[2];
                }
            }
        }
    }

    cv::imshow("output", output);
    cv::waitKey(0);
}

int Worker::SearchLowerBound(const int &x) {
    int l = 0, r = nareas_ - 1;
    if(x <= chunks_[l].size) {
        return l;
    }
    while(l < r) {
        int mid = (l+r) >> 1;
        if(chunks_[mid] < x) {
            l = mid + 1;
        } else {
            r = mid;
        }
    }
    return l;
}

int Worker::SearchUpperBound(const int &x) {
    
    int l = 0, r = nareas_ - 1;
    if(x > chunks_[r].size) {
        return r+1;
    }
    while(l < r) {
        int mid = (l+r) >> 1;
        if(chunks_[mid].size <= x) {
            l = mid + 1;
        } else {
            r = mid;
        }
    }
    return l;
}

void Worker::Queries() {
    bool running = true;
    int query_cnt = 0;
    while(running) {
        puts("INFO: Here are 2 operations:");
        puts("INFO: Operation 1: Search & Highlight (input 1)");
        puts("INFO: Operation 2: Exit (input 2)");
        printf("Choose an operator type and input it here: ");
        int op;
        scanf("%d", &op);
        
        switch (op) {
            case 1:
                query_cnt ++;
                int lb, rb;
                puts("INFO: In searching: [lower bound, upper bound)");
                printf("INFO: here input lower bound: ");
                scanf("%d", &lb);
                printf("INFO: here input upper bound: ");
                scanf("%d", &rb);
                if(lb >= rb || lb < 0) {
                    puts("ERROR: Wrong range!");
                } else {
                    int li = SearchLowerBound(lb);
                    int ri = SearchUpperBound(rb);
                    printf("li_size: %d, ri_size: %d\n", chunks_[li].size, chunks_[ri].size);
                    int nelems = ri - li;
                    printf("nelems: %d\n", nelems);
                    int *range = new int[nelems];
                    fprintf(stderr, "Query %d\n", query_cnt);
                    for(int i = 0; i < nelems; i++) {
                        range[i] = chunks_[i+li].idx;
                        fprintf(stderr, "number %d: idx: %d, size: %d\n", i, chunks_[i+li].idx, chunks_[i+li].size);
                    }
                    fprintf(stderr, "END OF A QUERY");
                    fprintf(stderr, "===============================");
                    Highlight(nelems, range);
                    delete[] range;
                }
            
                break;
            case 2:
                running = false;
                break;
            default:
                puts("ERROR: Wrong operation! Exit now...");
                running = false;
        }
        
        
    }
}