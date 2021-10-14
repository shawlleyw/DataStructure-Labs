#ifndef DS_UTILS_C_
#define DS_UTILS_C_

#include "utils.hh"

using namespace Utils::ADJACENT;

AdjacentTables::AdjacentTables(const int &nelems): nelems_(nelems){
    heads_ = new node*[nelems_+1];
    size_ = new int[nelems_+1];
    for(int i = 0; i <= nelems_; i++) {
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

void AdjacentTables::Link(int idx, const node &next) {
    size_[idx] ++;
    node *cur = new node(next);
    cur->next = heads_[idx];
    heads_[idx] = cur;
}

#endif