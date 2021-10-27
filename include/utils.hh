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

struct MapChunk {
    int size, idx;

    MapChunk() = default;
    MapChunk(const int &size, const int &idx) : size(size), idx(idx) {}

    bool operator < (const MapChunk &other) const {
        return size < other.size;
    }

    bool operator < (const int &int_size) const {
        return size < int_size;
    }
};

class Heap {
  private:

    int nelems_;

    int heap_size_{};

    MapChunk *heaps_;

    void swap(MapChunk &elem_a, MapChunk &elem_b);

    void Down();

    void Up();

  public:

    explicit Heap() = default;
    
    explicit Heap(const int &nelems);

    ~Heap();

    bool Empty() const;

    void Push(const int &size, const int &idx);

    const MapChunk &top() const;

    void Pop();

};

};


namespace ADJACENT {

struct node {

    int to;

    node *next;

    explicit node() = default;

    explicit node(const node &other) = default;

    explicit node(const int &index, node *const &inext = nullptr) : to(index), next(inext) {}

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

    void Link(const int &idx, const node &next);

    void Link(const int &idx, const int &to);

    node *head(const int &idx) const;

};

};


}

#endif