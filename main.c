#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    int board[4][4];
    int score;
} Board;

void initializeBoard(Board *board);

void fillInitialValues(Board *board);

void addNewRandom(Board *board);

void printBoard(Board *board);

bool canMove(Board *board);

void moveUp(Board *board);

void moveDown(Board *board);

void moveLeft(Board *board);

void moveRight(Board *board);

int main() {
    Board *board;
    board = (Board *) malloc(sizeof(Board));
    printf("2048 by ax3lt\n");
    printf("Usa WASD per muovere le caselle\n");
    printf("Premi R per resettare il gioco\n");
    printf("Premi Q per uscire\n");
    printf("Premi qualsiasi tasto per iniziare\n");
    char tmp;
    scanf("%c", &tmp);
    initializeBoard(board);
    fillInitialValues(board);

    while (canMove(board)) {

        printf("\nPunteggio: %d\n\n", board->score);
        printBoard(board);
        printf("Inserisci la mossa (WASD) oppure Q per uscire o R per reiniziare: ");
        char move;
        scanf("%c", &move);
        fflush(stdin);
        switch (move) {
            case 'w':
                moveUp(board);
                break;
            case 'a':
                moveLeft(board);
                break;
            case 's':
                moveDown(board);
                break;
            case 'd':
                moveRight(board);
                break;
            case 'q':
                printf("Uscita dal gioco!\n");
                return 0;
            case 'r':
                printf("Reinizializzo il gioco...\n");
                initializeBoard(board);
                fillInitialValues(board);
                break;
            default:
                printf("Invalid move\n");
                break;
        }
        if(canMove(board)) {
            addNewRandom(board);
            system("cls");
        } else{
            printf("Hai perso!\n");
            printf("Premi un tasto per uscire\n");
            scanf("%c", &tmp);
            return 0;
        }

    }


    return 0;
}

void initializeBoard(Board *board) {
    board->score = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            board->board[i][j] = 0;
        }
    }
}

void fillInitialValues(Board *board) {
    srand(time(NULL)); // Inizializza il generatore di numeri casuali
    int i1, j1;
    i1 = rand() % 4; // Genera un numero casuale tra 0 e 3 per la coordinata
    j1 = rand() % 4;

    int value = rand() % 2 + 1; // Genera un numero casuale tra 1 e 2 per il valore della casella
    if (value == 1) {
        value = 2;
    } else {
        value = 4;
    }
    board->board[i1][j1] = value;

    // Scelta seconda casella
    int i2 = rand() % 4;
    int j2 = rand() % 4;
    while (i1 == i2 && j2 == j1) {
        i2 = rand() % 4;
        j2 = rand() % 4;
    }
    value = rand() % 2 + 1;
    if (value == 1) {
        value = 2;
    } else {
        value = 4;
    }
    board->board[i2][j2] = value;
}

void printBoard(Board *board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", board->board[i][j]);
        }
        printf("\n");
    }
}

bool canMove(Board *board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board->board[i][j] == 0) {
                return true;
            }
            if (i < 3 && board->board[i][j] == board->board[i + 1][j]) {
                return true;
            }
            if (j < 3 && board->board[i][j] == board->board[i][j + 1]) {
                return true;
            }
        }
    }
    return false;
}

void moveUp(Board *board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board->board[i][j] != 0) {
                int k = i;
                while (k > 0 && board->board[k - 1][j] == 0) {
                    board->board[k - 1][j] = board->board[k][j];
                    board->board[k][j] = 0;
                    k--;
                }
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (board->board[i][j] == board->board[i + 1][j]) {
                board->board[i][j] *= 2;
                board->board[i + 1][j] = 0;
                board->score += board->board[i][j];
            }
        }
    }
}

void moveDown(Board *board) {
    for (int i = 3; i >= 0; i--) {
        for (int j = 0; j < 4; j++) {
            if (board->board[i][j] != 0) {
                int k = i;
                while (k < 3 && board->board[k + 1][j] == 0) {
                    board->board[k + 1][j] = board->board[k][j];
                    board->board[k][j] = 0;
                    k++;
                }
            }
        }
    }

    for (int i = 3; i > 0; i--) {
        for (int j = 0; j < 4; j++) {
            if (board->board[i][j] == board->board[i - 1][j]) {
                board->board[i][j] *= 2;
                board->board[i - 1][j] = 0;
                board->score += board->board[i][j];
            }
        }
    }
}

void moveLeft(Board *board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board->board[i][j] != 0) {
                int k = j;
                while (k > 0 && board->board[i][k - 1] == 0) {
                    board->board[i][k - 1] = board->board[i][k];
                    board->board[i][k] = 0;
                    k--;
                }
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if (board->board[i][j] == board->board[i][j + 1]) {
                board->board[i][j] *= 2;
                board->board[i][j + 1] = 0;
                board->score += board->board[i][j];
            }
        }
    }
}

void moveRight(Board *board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 3; j >= 0; j--) {
            if (board->board[i][j] != 0) {
                int k = j;
                while (k < 3 && board->board[i][k + 1] == 0) {
                    board->board[i][k + 1] = board->board[i][k];
                    board->board[i][k] = 0;
                    k++;
                }
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 3; j > 0; j--) {
            if (board->board[i][j] == board->board[i][j - 1]) {
                board->board[i][j] *= 2;
                board->board[i][j - 1] = 0;
                board->score += board->board[i][j];
            }
        }
    }
}

void addNewRandom(Board *board) {
    int i, j;
    i = rand() % 4;
    j = rand() % 4;
    while (board->board[i][j] != 0) {
        i = rand() % 4;
        j = rand() % 4;
    }
    int value = rand() % 2 + 1;
    if (value == 1) {
        value = 2;
    } else {
        value = 4;
    }
    board->board[i][j] = value;
}
