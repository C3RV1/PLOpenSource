#include "Save.hpp"

bool Save::getFlag(int flagNum) {
    if (flagNum >= FLAG_COUNT)
        return false;
    return getFlagFromArray(flags, flagNum);
}

void Save::setFlag(int flagNum, bool flagValue) {
    if (flagNum >= FLAG_COUNT)
        return;
    setFlagAtArray(flags, flagNum, flagValue);
}

int Save::getChapter() {
    return chapter;
}

void Save::incrementChapter() {
    chapter++;
    stage = 0;
}

int Save::getStage() {
    return stage;
}

void Save::incrementStage() {
    stage++;
}

bool Save::getPuzzleFound(int puzzleNum) {
    if (puzzleNum >= PUZZLE_COUNT)
        return false;
    return getFlagFromArray(puzzleFlags, puzzleNum * 2);
}

void Save::puzzleFound(int puzzleNum) {
    if (puzzleNum >= PUZZLE_COUNT)
        return;
    setFlagAtArray(puzzleFlags, puzzleNum * 2, true);
}

bool Save::getPuzzleSolved(int puzzleNum) {
    if (puzzleNum >= PUZZLE_COUNT)
        return false;
    return getFlagFromArray(puzzleFlags, puzzleNum * 2 + 1);
}

void Save::puzzleSolved(int puzzleNum) {
    if (puzzleNum >= PUZZLE_COUNT)
        return;
    setFlagAtArray(puzzleFlags, puzzleNum * 2 + 1, true);
}

bool Save::getFlagFromArray(uint8_t *array, int flagNum) {
    uint8_t flagBlockValue = array[flagNum / 8];
    uint8_t flagOffset = flagNum % 8;
    flagBlockValue >>= flagOffset;
    return flagBlockValue & 1;
}

void Save::setFlagAtArray(uint8_t *array, int flagNum, bool flagValue) {
    uint8_t flagBlockValue = array[flagNum / 8];
    uint8_t flagMask = 1 << (flagNum % 8);
    flagBlockValue &= ~flagMask;
    if (flagValue)
        flagBlockValue |= flagMask;
    array[flagNum / 8] = flagBlockValue;
}

void Save::clear() {
    chapter = 0;
    stage = 0;
    for (int i = 0; i < (FLAG_COUNT + 7) / 8; i++) {
        flags[i] = 0;
    }
    for (int i = 0; i < (PUZZLE_COUNT * 2 + 7) / 8; i++) {
        puzzleFlags[i] = 0;
    }
}

bool Save::load() {
    std::ifstream f("game_data/saves/save.sav");
    if (!f.good()) {
        std::cout << "Error loading save data. File couldn't be opened" << std::endl;
        return false;
    }

    f.read((char*)&chapter, 1);
    f.read((char*)&stage, 1);
    f.read((char*)flags, (FLAG_COUNT + 7) / 8);
    f.read((char*)puzzleFlags, (PUZZLE_COUNT * 2 + 7) / 8);

    f.close();
    return true;
}

bool Save::save() {
    std::ofstream f("game_data/saves/save.sav");
    if (!f.good()) {
        std::cout << "Error saving save data. File couldn't be opened" << std::endl;
        return false;
    }

    f.write((char*)&chapter, 1);
    f.write((char*)&stage, 1);
    f.write((char*)flags, (FLAG_COUNT + 7) / 8);
    f.write((char*)puzzleFlags, (PUZZLE_COUNT * 2 + 7) / 8);

    f.close();
    return true;
}

bool Save::existsSave() {
    std::ifstream f("game_data/saves/save.sav");
    if (!f.good()) {
        return false;
    }
    return true;
}
