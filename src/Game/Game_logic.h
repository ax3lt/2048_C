#ifndef INC_2048_MAC_GAME_LOGIC_H
#define INC_2048_MAC_GAME_LOGIC_H

#endif //INC_2048_MAC_GAME_LOGIC_H

typedef struct {
//    int board[4][4];
    int **board;
    int **lastBoard;
    int dimX;
    int dimY;
    int score;
} Board;

void initializeBoard(Board *board);

void fillInitialValues(Board *board);

void addNewRandom(Board *board);

void printBoard(Board *board);

bool canMove(Board *board);

bool canMoveLeft(Board *board);

bool canMoveRight(Board *board);

bool canMoveUp(Board *board);

bool canMoveDown(Board *board);

void moveUp(Board *board);

void moveDown(Board *board);

void moveLeft(Board *board);

void moveRight(Board *board);

void saveGame(Board *b);

void loadGame(char *filename, Board *b);

void setGridDimension(Board *b);

void printWelcomeMessage();

void printLoseMessage();

void gameHandler(Board *board);

void saveBoard(Board *board);

void loseHandler(Board *board);

void memoryFree(Board *board);