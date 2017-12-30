#include <unistd.h>
#include <iostream>
#include <vector>


#include "common.hpp"
using namespace std;



void benchmarkStream1(vector<AbstractInputstream*> isVector){
    int vectorSize = static_cast<int> (isVector.size());
    // We open our K stream based on the quantity of inputstreams
    for (int i = 0; i < vectorSize; ++i){
        AbstractInputstream* is = isVector[i];
        char DATA_FILENAME[]{"data files/data."};
        char str[10];
        sprintf(str, "%d", i);
        is->open(strcat(DATA_FILENAME,str));
    }

    bool finished = false;
    while (finished){
        vectorSize = static_cast<int>(isVector.size());
        vector<int> vectorDelete;
        for (int i = 0; i < vectorSize; ++i){
            AbstractInputstream* is = isVector[i];
            is->read_next();
            if (is->end_of_stream()){
                vectorDelete.push_back(i);
            }
        }
        // close and delete finished stream
        for (int i = static_cast<int> (vectorDelete.size()); i >= 0; --i){
            AbstractInputstream* is = isVector[i];
            is->close();
            isVector.erase(isVector.begin()+i);
            delete is;
        }
    }


}
