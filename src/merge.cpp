#include <iostream>
#include <queue>
#include <utility> // swap, pair
#include <vector>

#include "common.hpp"
#include "merge.hpp"

/**
 * Merge isc sorted Input Streams in in one Output Stream.
 * isc: (input streams count) the number of streams to merge (must be the size of isv)
 * isv: (input streams values) an array of pointer to the streams to merge.
 *      The function will read theses streams to the end but will not open nor close any.
 *      !! Input have to be in increasing order !!
 * os:  the output stream to use to store the result.
 *      The function will write using it, but will not open nor close it.
 */
void merge(size_t isc, AbstractInputstream * const isv[], AbstractOutputstream & os) {
    // Initialise a priority queue used to store the first (i.e. smallest) item of each
    // input stream in increasing order (i.e. top is the smallest element).
    // We need to keep track of the origin stream of each element.
    std::priority_queue<
        std::pair<int32_t, AbstractInputstream*>,
        std::vector<std::pair<int32_t, AbstractInputstream*> >,
        std::greater<std::pair<int32_t, AbstractInputstream*> >
    > heap;

    for (size_t i = 0; i < isc; ++i) {
        if (!isv[i]->end_of_stream()) {
            heap.push(std::pair<int32_t, AbstractInputstream*>(isv[i]->read_next(), isv[i]));
        }
    }

    // At each iteration, write the smallest element on the output stream and
    // insert the next element of the origin stream in the heap.
    while (!heap.empty()) {
        std::pair<int32_t, AbstractInputstream*> least(heap.top());
        os.write(least.first);
        heap.pop();
        if (!least.second->end_of_stream()) {
            heap.push(std::pair<int32_t, AbstractInputstream*>(least.second->read_next(), least.second));
        }
    }
}

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
