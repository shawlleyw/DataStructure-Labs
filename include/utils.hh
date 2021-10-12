#ifndef DS_UTILS_H_
#define DS_UTILS_H_

#include <cstdlib>
#include <ctime>

namespace Utils {

template<class T>
class Queue {
  private :

    int tail_{};

    int head_{};

    T *q;

    int size_;

  public :

    Queue(int size) : size_(size) {
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

    T at(int idx) {
        return q[head_ + idx];
    }
};


class RandomGenerator {
  public:

    RandomGenerator ();

    ~RandomGenerator () = default;

    int RangeInt(int base, int mod);
};

}

#endif