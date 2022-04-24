#include <SDL.h>
#include "game/StateAwareFF.hpp"

int main(int argc, char* argv[]) {
    Save* save = Save::getInstance();
    save->clear();
    save->incrementChapter();
    save->incrementChapter();
    save->incrementStage();
    StateAwareFF saff("game_data/saff_test/constructed.dat");
    if (!saff.good()) {
        std::cout << "Error opening saff!" << std::endl;
        return 0;
    }
    int length = saff.getLength();
    char buffer[length];
    saff.read(buffer, length);
    std::cout << "Got value " << buffer << " " << std::endl;
    return 0;
}