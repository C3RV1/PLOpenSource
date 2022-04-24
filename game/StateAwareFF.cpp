#include "StateAwareFF.hpp"

void StateAwareFF::load() {
    char header[4];
    f.read((char*)&header, 4);

    Save* save = Save::getInstance();

    if (memcmp(header, "SAFF", 4) != 0) {
        f.close();
        open = false;
        std::cout << "Error opening StateAwareFile. Header does not match." << std::endl;
        return;
    }

    uint8_t segmentCount = 0;
    f.read((char*)&segmentCount, 1);
    segmentOffsets.resize(segmentCount);
    segmentLengths.resize(segmentCount);
    for (uint8_t i = 0; i < segmentCount; i++) {
        f.read((char*)&segmentLengths[i], 2);
        segmentOffsets[i] = f.tellg();
        f.seekg(segmentLengths[i], std::ios::cur);
    }

    std::list<uint8_t> segmentsChosen;
    // 0 - not skipped
    // 1 - skipped because condition false
    // 2 - skipped because previous was executed
    int skipping = 0;
    int skippingLevel = 0;
    int level = 0;
    int currentPos = f.tellg();
    f.seekg(0, std::ios::end);
    int length = f.tellg();
    f.seekg(currentPos);
    for (;;) {
        if (f.tellg() == length)
            break;
        uint8_t conditionByte = 0;
        f.read((char*)&conditionByte, 1);
        bool isCondition = (conditionByte & 0b10000000) >> 7;
        if (isCondition) {
            uint8_t conditionType = (conditionByte & 0b1100000) >> 5;
            if (conditionType > 0) {
                // if it's not "if" we go down a level
                //
                // if
                //   if
                //
                // vs
                //
                // if
                // elif
                level -= 1;
            }
            if (conditionType != 0 && conditionType < 3 && skipping == 0) {
                // if we reach a elif/else having executed the previous on this level we skip to the endif
                skipping = 2;
                skippingLevel = level;
            }
            if (skipping > 0 && skippingLevel == level) {
                if (conditionType == 3) {
                    // if we reach an endif on this level we stop skipping
                    skipping = 0;
                } else if (conditionType > 0 && skipping == 1) {
                    // if we reach an elif/else on this level w/out executing the previous one we can execute this one
                    skipping = 0;
                }
            }
            if (conditionType < 3) {
                // if / elif / else increase level
                level += 1;
            }
            if (conditionType < 2) {
                // if / elif check condition
                bool neg = (conditionByte & 0b10000) >> 4;
                bool is_flag = (conditionByte & 0b1000) >> 3;
                bool result = false;
                if (is_flag) {
                    uint8_t flagType = conditionByte & 0b111;
                    uint16_t flagNum;
                    f.read((char*)&flagNum, 2);
                    if (flagType == 0) {
                        result = save->getCoinCollected(flagNum / 3, flagNum % 3);
                    } else if (flagType == 1) {
                        result = save->getPuzzleFound(flagNum);
                    } else if (flagType == 2) {
                        result = save->getPuzzleSolved(flagNum);
                    } else if (flagType == 3) {
                        result = save->getFlag(flagNum);
                    } else {
                        std::cout << "Flag type " << (int)flagType << " unknown." << std::endl;
                    }
                } else {
                    uint8_t comparator = (conditionByte & 0b110) >> 1;
                    uint8_t field;
                    f.read((char*)&field, 1);
                    uint16_t value = 0;
                    bool is16Bit = conditionByte & 0b1;

                    if (is16Bit) {
                        f.read((char*)&value, 2);
                    } else {
                        uint8_t tmpValue = 0;
                        f.read((char*)&tmpValue, 1);
                        value = (uint16_t)tmpValue;
                    }

                    uint16_t fieldValue = 0;
                    if (field == 0) {
                        fieldValue = save->getChapter();
                    } else if (field == 1) {
                        fieldValue = save->getStage();
                    } else {
                        std::cout << "Unknown field " << (int)field << " for condition" << std::endl;
                    }

                    if (comparator == 0) {
                        //eq
                        result = fieldValue == value;
                    } else if (comparator == 1) {
                        //gt
                        result = fieldValue > value;
                    } else if (comparator == 2) {
                        //gte
                        result = fieldValue >= value;
                    } else {
                        std::cout << "Unknown comparator " << (int)comparator << std::endl;
                    }
                }
                if (neg)
                    result = !result;
                if (!result && skipping == 0) {
                    skipping = 1;
                    skippingLevel = level - 1;
                }
            }
        } else {
            // segment
            if (!skipping) {
                segmentsChosen.push_back(conditionByte);
            }
        }
    }

    // Copy segments chosen to constructSegments
    constructSegments.resize(0);
    constructSegments.reserve(segmentsChosen.size());
    std::copy(std::begin(segmentsChosen), std::end(segmentsChosen), std::back_inserter(constructSegments));
    
    totalConstructedLength = 0;
    for (auto usedSegment : constructSegments) {
        totalConstructedLength += segmentLengths[usedSegment];
    }
    
    currentOffset = 0;
    currentSegment = 0;
    currentSegmentOffset = 0;
    seekg(0);
}

bool StateAwareFF::good() {
    return open && f.good();
}

void StateAwareFF::read(char* buffer, int length) {
    for (;;) {
        if (length == 0)
            return;
        if (currentSegment >= constructSegments.size())
            return;
        int currentReadSize = length;
        int resolvedSegment = constructSegments[currentSegment];
        int segmentLength = segmentLengths[resolvedSegment];
        if (currentReadSize > segmentLength - currentSegmentOffset) {
            currentReadSize = segmentLength - currentSegmentOffset;
        }
        f.read(buffer, currentReadSize);
        buffer += currentReadSize;
        length -= currentReadSize;
        currentSegmentOffset += currentReadSize;
        if (currentSegmentOffset == segmentLength) {
            currentSegment++;
            currentSegmentOffset = 0;
            if (currentSegment >= constructSegments.size())
                return;
            resolvedSegment = constructSegments[currentSegment];
            int segmentOffset = segmentOffsets[resolvedSegment];
            f.seekg(segmentOffset, std::ios::beg);
        }
    }
}

void StateAwareFF::seekg(int position, std::ios::seekdir seekdir) {
    if (seekdir == std::ios::cur) {
        position = currentOffset + position;
    } else if (seekdir == std::ios::end) {
        position = totalConstructedLength - position;
    }
    if (position > currentOffset) {
        currentSegment = 0;
        currentSegmentOffset = 0;
        currentOffset = 0;
    }
    for (;;) {
        if (currentSegment >= constructSegments.size())
            return;
        int resolvedSegment = constructSegments[currentSegment];
        int segmentLength = segmentLengths[resolvedSegment];
        if (position - currentOffset >= segmentLength) {
            currentOffset += segmentLength;
            currentSegment++;
        } else {
            currentSegmentOffset = position - currentOffset;
            currentOffset = position;
            break;
        }
    }
    int resolvedSegment = constructSegments[currentSegment];
    int segmentOffset = segmentOffsets[resolvedSegment];
    f.seekg(segmentOffset + currentSegmentOffset, std::ios::beg);
}

int StateAwareFF::tellg() {
    return currentOffset;
}

int StateAwareFF::getLength() {
    return totalConstructedLength;
}