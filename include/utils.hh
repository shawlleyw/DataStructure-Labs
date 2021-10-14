#ifndef DS_UTILS_H_
#define DS_UTILS_H_

#include <cstdlib>
#include <ctime>

namespace Utils {

namespace mySTL{
// ! mySTL std::queue<>
template<class T>
class Queue {
  private :

    int tail_{};

    int head_{};

    T *q;

    int size_;

  public :

    explicit Queue(int size) : size_(size) {
        q = new T[size];
    }

    void push(T elem) {
        q[tail_++] = elem;
    }

    bool empty() {
        return head_ >= tail_;
    }

    T front() {
        return q[head_];
    }

    void pop() {
        head_ ++;
    }

    ~Queue() {
      delete [] q;
    }

    void erase(int idx) {
        idx += head_;
        if(idx < head_ || idx >= tail_) {
            return;
        }
        if(idx == head_) {
            head_ ++;
            return;
        }
        for(int i = idx; i < tail_ - 1; i++) {
           q[i] = q[i+1];
        }
        tail_ --;
    }

    int len() {
        return tail_ - head_;
    }

    const T &at(const int &idx) const {
        return q[head_ + idx];
    }

    T &at(const int &idx) {
        return q[head_ + idx];
    }
};

};


namespace ADJACENT {

struct node {

    int row, col, idx;

    node *next;

    explicit node() = default;

    explicit node(const node &other) = default;

    explicit node(int irow, int icol, int index, node *inext = nullptr) : row(irow), col(icol), idx(index), next(inext) {}

};

class AdjacentTables {
  private:

    int nelems_;

    node **heads_;

    int *size_;

  public:

    explicit AdjacentTables() = default;

    explicit AdjacentTables(const int &nelems);

    ~AdjacentTables();

    void Link(int idx, const node &next);

};

};


}

#endif