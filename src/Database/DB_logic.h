#ifndef INC_2048_MAC_DB_LOGIC_H
#define INC_2048_MAC_DB_LOGIC_H

#endif //INC_2048_MAC_DB_LOGIC_H

#include "../libs/SQLite3/sqlite3.h"


void initializeDB();

int getScore(char *username, int boardDim);

void addScore(char *username, int score, int boardDim);

void updateScore(char *username, int score, int boardDim);

void printLeaderboard();