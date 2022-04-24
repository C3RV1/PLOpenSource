#ifndef GAME_SAVE
#define GAME_SAVE

#define FLAG_COUNT 100
#define PUZZLE_COUNT 200
#define ROOM_COUNT 200

#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>

enum GameMode {
    TITLESCREEN,
    ROOM,
    EVENT,
    PUZZLE,
    TRUNK
};

class Save {
public:
    static Save* getInstance()
    {
        static Save instance;
        return &instance;
    }

    bool getFlag(int flagNum);
    void setFlag(int flagNum, bool flagValue);
    
    int getChapter();
    void incrementChapter();
    int getStage();
    void incrementStage();

    bool getPuzzleFound(int puzzleNum);
    void puzzleFound(int puzzleNum);
    bool getPuzzleSolved(int puzzleNum);
    void puzzleSolved(int puzzleNum);

    bool getCoinCollected(int room, int coinNum);
    void collectCoin(int room, int coinNum);

    bool load();
    bool save();
    void clear();
    bool existsSave();

    GameMode gameMode = GameMode::TITLESCREEN;
    int gameModeId;
    GameMode nextGameMode;
    int nextGameModeId;

private:
    bool getFlagFromArray(uint8_t *array, int flagNum);
    void setFlagAtArray(uint8_t *array, int flagNum, bool flagValue);

    uint8_t chapter = 0;
    uint8_t stage = 0;
    uint8_t flags[(FLAG_COUNT + 7) / 8];
    uint8_t puzzleFlags[(PUZZLE_COUNT * 2 + 7) / 8];
    uint8_t coinFlags[(ROOM_COUNT * 3 + 7) / 8];
    Save() {}
};

#endif
