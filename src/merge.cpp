#include <stdio.h> // P_tmpdir
#include <iostream>
#include <vector>
#include <queue>
#include <utility> // swap, pair

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
void merge(size_t isc, AbstractInputstream * isv[], AbstractOutputstream & os) {
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

/** return the name of the sorted file */
template<typename OS, typename IS>
char * dway_merge(IS & input, size_t memory /* M */, size_t streamsc /* d */) {
    // 1. Read the input file and split it into ceil(N/M) streams, each of size at most M.

    char * filename_template = new char[40];
    snprintf(filename_template, 39, "%s/%s", P_tmpdir, "dway_merge.%ld.tmp");
    char * filename = new char[40];

    OS output;

    // split
    // written file: d_way_merge.%.d with % in [1, splitsc]
    size_t splitsc(0);
    std::priority_queue< int32_t, std::vector<int32_t>, std::greater<int32_t> > heap;
    while(!input.end_of_stream()) {
        while (heap.size() < memory && !input.end_of_stream()) {
            heap.push(std::move(input.read_next()));
        }
        if (!heap.empty()) {
            snprintf(filename, 39, filename_template, ++splitsc);
            output.create(filename);
            while (!heap.empty()) {
                output.write(heap.top());
                heap.pop();
            }
            output.close();
        }
    }

    // 2. Store the references to the ceil(N/M) streams in a queue.

    std::queue<size_t> queue;
    for (size_t i = 1; i <= splitsc; ++i) {
        queue.push(i);
    }

    // 3. Repeatedly until only one stream remains, merge the d first streams in the queue, and
    // put the resulting stream at the end of the queue (if only x < d streams remain in the
    // queue, merge those).

    size_t next_file(0);
    IS * inputs[] = new IS*[streamsc];
    for (size_t i = 0; i < streamsc; ++i) {
        inputs[i] = new IS();
    }
    while (queue.size() > 1) {
        size_t isc = 0;
        while (isc < streamsc && !queue.empty()) {
            snprintf(filename, 39, filename_template, queue.front());
            queue.pop();
            inputs[isc]->open(filename);
            ++isc;
        }

        snprintf(filename, 39, filename_template, next_file);
        output.create(filename);
        queue.push(next_file);
        ++next_file;

        merge(isc, inputs, output);

        for (size_t i = 0; i < isc; ++i) {
            inputs[isc]->close();
        }
    }
    for (size_t i = 0; i < streamsc; ++i) {
        delete inputs[i];
    }

    snprintf(filename, 39, filename_template, queue.front());
    queue.pop();

    delete[] inputs;
    delete[] filename_template;

    return filename;
}

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
