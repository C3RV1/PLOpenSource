#ifndef GAME_STATEAWAREFF
#define GAME_STATEAWAREFF

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <cstring>
#include <iostream>
#include "Save.hpp"

class StateAwareFF {
public:
    StateAwareFF(std::string path) : f(path.c_str(), std::ifstream::binary) {
        if (f.good()) {
            open = true;
            load();
        } else {
            open = false;
        }
    }
    bool good();
    void read(char* buffer, int length);
    void seekg(int position, std::ios::seekdir seekdir = std::ios::beg);
    int tellg();
    int getLength();
private:
    bool open;
    void load();
    std::ifstream f;
    std::vector<int> segmentOffsets;
    std::vector<uint16_t> segmentLengths;
    std::vector<int> constructSegments;
    int totalConstructedLength;
    int currentOffset;
    int currentSegment;
    int currentSegmentOffset;
};

#endif
