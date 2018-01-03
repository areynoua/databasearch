#ifndef __MERGE_HPP__
#define __MERGE_HPP__

#include <queue>
#include <stdio.h> // P_tmpdir
#include <vector>

// TODO: Make template
void merge(size_t isc, AbstractInputstream * const isv[], AbstractOutputstream & os);

/** return the name of the sorted file */
template<typename OS, typename IS>
char * dway_merge_sort(IS & input, size_t memory /* M */, size_t streamsc /* d */) {
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
    AbstractInputstream ** inputs = new AbstractInputstream*[streamsc]; // (TODO) Because merge is not template but relies to inheritance
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
        output.close();

        for (size_t i = 0; i < isc; ++i) {
            inputs[i]->close();
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

#endif // __MERGE_HPP__

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
