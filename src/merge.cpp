#include <iostream>
#include <vector>
#include <queue>

#include "common.hpp"

#include "merge.hpp"

using namespace std;

// merges arrays of k sub-arrays using a min-heap
vector<int_least32_t> merge(vector<vector<int_least32_t>>& vect) {
    int k = vect.size();
    vector<int_least32_t> result;
    priority_queue<int_least32_t, vector<int_least32_t>, greater<int_least32_t>> heap;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < vect[i].size(); j++) {
            heap.push(vect[i][j]);
            // this part was used to have a heap of maximal k streams, but seems like it's not working
            // so we will have a full size and the heap will automatically order our input
            // TODO Should we change?
//            if (heap.size() == k) {
//                result.push_back(heap.top());
//                heap.pop();
//            }
        }
    }
    while (heap.size()) {
        result.push_back(heap.top());
        heap.pop();
    }
    return result;
}


void dway_merge(AbstractInputstream* is, AbstractOutputstream* os){
    os->create("TestMerge1");
    for (int i = 0; i <10; ++i){
        os->write(i*2);
    }
    os->close();
    os->create("TestMerge2");
    for (int i = 0; i <10; ++i){
        os->write((i*2)+1);
    }
    os->close();


    vector<vector<int_least32_t>> vect;
    is->open("random.16");
    vector<int_least32_t> x;
    while (!is->end_of_stream()){
        x.push_back(is->read_next());
    }
    vect.push_back(x);
    is->open("random.256");
    x.clear();
    while (!is->end_of_stream()){
        x.push_back(is->read_next());
    }
    vect.push_back(x);
    for (int i = 0; i < vect.size(); ++i){
        for (int j = 0; j < vect[i].size(); ++j){
            cout << vect[i][j] << " ";
        }
        cout << endl;
    }

    cout << "ICI " << endl;
    vector<int_least32_t> result = merge(vect);
    for (int_least32_t value: result)
        cout << value << " ";
    cout << endl;
}

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
