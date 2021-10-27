#ifndef DS_UTILS_C_
#define DS_UTILS_C_

#include "utils.hh"
#include <exception>
#include <iostream>

using namespace Utils::ADJACENT;
using namespace Utils::mySTL;

AdjacentTables::AdjacentTables(const int &nelems): nelems_(nelems){
    heads_ = new node*[nelems_];
    size_ = new int[nelems_];
    for(int i = 0; i < nelems_; i++) {
        heads_[i] = nullptr;
        size_[i] = 0;
    }
}

AdjacentTables::~AdjacentTables() {
    delete [] size_;
    for(int i = 0; i < nelems_; i++) {
        if(heads_[i] != nullptr) {
            node *cur = heads_[i];
            node *next;
            while(cur != nullptr) {
                next = cur->next;
                delete cur;
                cur = next;
            }
        }
    }
    delete [] heads_;
}


void AdjacentTables::Link(const int &idx, const int &to) {
    if(idx < 0 || idx >= nelems_) {
        std::cerr << idx << std::endl;
        throw std::exception();
    }
    size_[idx] ++;
    node *cur = new node(to);
    cur->next = heads_[idx];
    heads_[idx] = cur;
}

void AdjacentTables::Link(const int &idx, const node &next) {
    if(idx < 0 || idx >= nelems_) {
        std::cerr << idx << std::endl;
        throw std::exception();
    }
    size_[idx] ++;
    node *cur = new node(next);
    cur->next = heads_[idx];
    heads_[idx] = cur;
}

node *AdjacentTables::head(const int &idx) const {
    if(idx <= 0 || idx > nelems_) {
        return nullptr;
    }
    return heads_[idx];
}


void Heap::swap(MapChunk &elem_a, MapChunk &elem_b) {
    MapChunk t = elem_a;
    elem_a = elem_b;
    elem_b = t;
}

Heap::Heap(const int &nelems) : nelems_(nelems), heaps_(new MapChunk[nelems_]) {}

Heap::~Heap() { 
    delete[] heaps_;
}

bool Heap::Empty() const { 
    return heap_size_ == 0;
}

void Heap::Push(const int &size, const int &idx) {
    heaps_[heap_size_++] = MapChunk(size, idx);
    Up();
}

const MapChunk &Heap::top() const { 
    return heaps_[0];
}

void Heap::Pop() {
    heaps_[0] = heaps_[--heap_size_];
    Down();
}


#define LSON (cur<<1|1)
#define RSON (LSON+1)
#define FATHER ((cur >> 1) - !(cur&1))

void Heap::Down() {
    int cur = 0;
    while(true) {
        int ls = LSON;
        int rs = ls+1;
        if(ls >= heap_size_) {
            break;
        }
        int mn = rs < heap_size_ && heaps_[rs] < heaps_[ls] ? rs : ls;
        if(heaps_[mn] < heaps_[cur]) {
            swap(heaps_[mn], heaps_[cur]);
        } else {
            break;
        }
        cur = mn;
    }
}

void Heap::Up() {
    int cur = heap_size_ - 1;
    while(true) {
        if(cur == 0) {
            break;
        }
        int fa = FATHER;
        if(heaps_[cur] < heaps_[fa]) {
            swap(heaps_[cur], heaps_[fa]);
        }
        cur = fa;
    }
}

#undef LSON
#undef RSON
#undef FATHER

#endif