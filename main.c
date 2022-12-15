#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "Libraries/SQLite3/sqlite3.h"


// Per SQLITE3 bisogna installare una libreria esterna con il comando: sudo apt install libsqlite3-dev


// define ANSI colors
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

// define ANSI font styles
#define ANSI_FONT_BOLD     "\x1b[1m"
#define ANSI_FONT_UNDERLINE "\x1b[4m"
#define ANSI_FONT_BLINK    "\x1b[5m"
#define ANSI_FONT_REVERSE  "\x1b[7m"

// define ANSI background colors
#define ANSI_BG_RED     "\x1b[41m"
#define ANSI_BG_GREEN   "\x1b[42m"
#define ANSI_BG_YELLOW  "\x1b[43m"
#define ANSI_BG_BLUE    "\x1b[44m"
#define ANSI_BG_MAGENTA "\x1b[45m"
#define ANSI_BG_CYAN    "\x1b[46m"

#define ANSI_RESET   "\x1b[0m"


typedef struct {
    int board[4][4];
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

bool containsParam(int argc, char *argv[], char *param);

void moveUp(Board *board);

void moveDown(Board *board);

void moveLeft(Board *board);

void moveRight(Board *board);

void initializeDB();

int getScore(char *username);

void addScore(char *username, int score);

void updateScore(char *username, int score);

void printLeaderboard();

void saveGame(Board *b);

void loadGame(char *filename, Board *b);

bool debugMode = false;


int main(int argc, char *argv[]) {

    if(containsParam(argc, argv, "-d")) { // se il programma viene lanciato con il parametro -d, attiva la modalità debug
        debugMode = true;
    }
    initializeDB();
    system("read");
    Board *board;
    board = (Board *) malloc(sizeof(Board)); // alloco la memoria per la struttura Board
    system("clear");
    printf(ANSI_FONT_BOLD ANSI_COLOR_BLUE
           "██████╗  ██████╗ ██╗  ██╗ █████╗     ██████╗ ██╗   ██╗     █████╗ ██╗  ██╗██████╗ ██╗  ████████╗\n"
           "╚════██╗██╔═████╗██║  ██║██╔══██╗    ██╔══██╗╚██╗ ██╔╝    ██╔══██╗╚██╗██╔╝╚════██╗██║  ╚══██╔══╝\n"
           " █████╔╝██║██╔██║███████║╚█████╔╝    ██████╔╝ ╚████╔╝     ███████║ ╚███╔╝  █████╔╝██║     ██║   \n"
           "██╔═══╝ ████╔╝██║╚════██║██╔══██╗    ██╔══██╗  ╚██╔╝      ██╔══██║ ██╔██╗  ╚═══██╗██║     ██║   \n"
           "███████╗╚██████╔╝     ██║╚█████╔╝    ██████╔╝   ██║       ██║  ██║██╔╝ ██╗██████╔╝███████╗██║   \n"
           "╚══════╝ ╚═════╝      ╚═╝ ╚════╝     ╚═════╝    ╚═╝       ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝   \n"
           "                                                                                                \n" ANSI_RESET);
    printf(ANSI_COLOR_YELLOW "⮕ " ANSI_RESET "Usa " ANSI_COLOR_GREEN " (W A S D)" ANSI_RESET " o le " ANSI_COLOR_GREEN "freccie " ANSI_RESET "per muovere le caselle\n");
    printf(ANSI_COLOR_YELLOW "⮕ " ANSI_RESET "Premi " ANSI_COLOR_GREEN "R" ANSI_RESET " per resettare il gioco\n");
    printf(ANSI_COLOR_YELLOW "⮕ " ANSI_RESET "Premi " ANSI_COLOR_GREEN "Q" ANSI_RESET " per uscire\n");
    printf(ANSI_COLOR_YELLOW "⮕ " ANSI_RESET "Puoi salvare il gioco premendo " ANSI_COLOR_GREEN "F" ANSI_RESET "\n");
    printf(ANSI_COLOR_YELLOW "⮕ " ANSI_RESET "Per caricare una partita salvata premi "  ANSI_COLOR_GREEN "C" ANSI_RESET " (deve essere presente un file con il nome 2048.dat nella directory del programma)\n");
    printf(ANSI_COLOR_YELLOW "⮕ " ANSI_RESET "Premi "  ANSI_COLOR_GREEN "L" ANSI_RESET " ora per visualizzare la leatherboard\n");
    printf("\n" ANSI_COLOR_RED "⮕ " ANSI_RESET "Premi qualsiasi tasto per iniziare\n");
    char tmp;
    scanf("%c", &tmp);
    if (tmp == 'L' || tmp == 'l') {
        system("clear");
        printLeaderboard();
        exit(0);
    }
    fflush(stdin);
    if(tmp == 'C' || tmp == 'c') {
        loadGame("2048.dat", board); // carica la partita salvata
    } else {
        initializeBoard(board);
        fillInitialValues(board);
    }
    system("clear");

    while (1) {
        bool validMove = true;  // Controllo se possibile fare un movimento valido
        bool restart = false;   // Controllo se è stato premuto R
        printf(ANSI_COLOR_GREEN "🚀 Punteggio: %d" ANSI_RESET "\n\n", board->score);
        printBoard(board);
        printf("• Usa WASD o le freccie per muoverti\n"
                     "• Q per uscire"
                     "• R per ricominciare\n"
                     "• F per salvare\n"
                     "• Per visualizzare la leatherboard oppure per caricare una partita riavvia il programma\n");


        system("/bin/stty -echo");  // Disabilita l'echo dei tasti premuti
        system("/bin/stty raw");    // Disabilita il buffering dei tasti premuti -> legge i tasti premuti immediatamente (senza premere invio)

        int c;
        do {
            c = getchar();

            if (c == 27) {  // Controllo se è stato premuto un tasto di direzione (usano tre codici ASCII)
                c = getchar();
                if (c == 91) {
                    c = getchar();
                    if (c == 68)
                        c = 'a';
                    else if (c == 65)
                        c = 'w';
                    else if (c == 66)
                        c = 's';
                    else if (c == 67)
                        c = 'd';
                }
            }
        } while (c != 'w' && c != 'a' && c != 's' && c != 'd' && c != 'q' && c != 'r' && c != 'W' && c != 'A' &&
                 c != 'S' && c != 'D' && c != 'Q' && c != 'R' && c != 'f' && c != 'F' && c != 'c' && c != 'C');

        system("/bin/stty echo");   // Riabilita l'echo dei tasti premuti
        system("/bin/stty cooked"); // Riabilita il buffering dei tasti premuti


        if(c == 'f' || c == 'F') {
            saveGame(board);
            validMove = false;
            if (debugMode)
                printf("Partita salvata!\n");
        }
        else if (c == 'q' || c == 'Q') { // <-- Terminazione del gioco
            printf(ANSI_BG_RED "\nHai scelto di uscire dal gioco\n" ANSI_RESET);
            exit(0);
        } else if (c == 'r' || c == 'R') { // <-- Reimposto la tabella e rimetto i valori iniziali
            printf("Hai scelto di reiniziare il gioco\n");
            initializeBoard(board);
            fillInitialValues(board);
            restart = true;
            system("clear");
        } else if (c == 'w' || c == 'W') {  // <-- Muovo verso l'alto
            if (canMoveUp(board)) {
                moveUp(board);
            } else {
                validMove = false;
            }
        } else if (c == 'a' || c == 'A') {  // <-- Muovo verso sinistra
            if (canMoveLeft(board)) {
                moveLeft(board);
            } else {
                validMove = false;
            }
        } else if (c == 's' || c == 'S') {  // <-- Muovo verso il basso
            if (canMoveDown(board)) {
                moveDown(board);
            } else {
                validMove = false;
            }
        } else if (c == 'd' || c == 'D') {  // <-- Muovo verso destra
            if (canMoveRight(board)) {
                moveRight(board);
            } else {
                validMove = false;
            }
        }


        if (!restart &&
            validMove) {    // <-- Se non è stato premuto R e se è stato premuto un tasto valido posso aggiungere un nuovo valore
            addNewRandom(board);
        }
        system("clear");
        if (!canMove(board)) {  // <-- Se non è possibile muovere nessuna casella il gioco è finito
            printf(ANSI_COLOR_RED ""
                   "\n"
                   " ██░ ██  ▄▄▄       ██▓    ██▓███  ▓█████  ██▀███    ██████  ▒█████   ▐██▌ \n"
                   "▓██░ ██▒▒████▄    ▓██▒   ▓██░  ██▒▓█   ▀ ▓██ ▒ ██▒▒██    ▒ ▒██▒  ██▒ ▐██▌ \n"
                   "▒██▀▀██░▒██  ▀█▄  ▒██▒   ▓██░ ██▓▒▒███   ▓██ ░▄█ ▒░ ▓██▄   ▒██░  ██▒ ▐██▌ \n"
                   "░▓█ ░██ ░██▄▄▄▄██ ░██░   ▒██▄█▓▒ ▒▒▓█  ▄ ▒██▀▀█▄    ▒   ██▒▒██   ██░ ▓██▒ \n"
                   "░▓█▒░██▓ ▓█   ▓██▒░██░   ▒██▒ ░  ░░▒████▒░██▓ ▒██▒▒██████▒▒░ ████▓▒░ ▒▄▄  \n"
                   " ▒ ░░▒░▒ ▒▒   ▓▒█░░▓     ▒▓▒░ ░  ░░░ ▒░ ░░ ▒▓ ░▒▓░▒ ▒▓▒ ▒ ░░ ▒░▒░▒░  ░▀▀▒ \n"
                   " ▒ ░▒░ ░  ▒   ▒▒ ░ ▒ ░   ░▒ ░      ░ ░  ░  ░▒ ░ ▒░░ ░▒  ░ ░  ░ ▒ ▒░  ░  ░ \n"
                   " ░  ░░ ░  ░   ▒    ▒ ░   ░░          ░     ░░   ░ ░  ░  ░  ░ ░ ░ ▒      ░ \n"
                   " ░  ░  ░      ░  ░ ░                 ░  ░   ░           ░      ░ ░   ░    \n"
                   "                                                                          "
                   ANSI_RESET);

            printf(ANSI_COLOR_RED "\nHai perso!\n Il tuo punteggio è %d\n" ANSI_RESET, board->score);
            // prompt to save the score
            printf("Vuoi salvare il tuo punteggio? (s/n)\n");
            char save;
            scanf(" %c", &save);
            if (save == 's' || save == 'S') {
                // prompt the username
                printf("Inserisci il tuo nome utente: ");
                char username[10];
                scanf("%s", username);
                // save the score
                if (getScore(username) == 0) {
                    addScore(username, board->score);
                } else {
                    if (board->score > getScore(username)) {
                        updateScore(username, board->score);
                    }
                }
            }


            printf("\nPremi qualsiasi tasto per uscire\n");
            // clear the buffer
            while (getchar() != '\n');
            system("/bin/stty -echo");
            system("/bin/stty raw");
            c = getchar();
            system("/bin/stty echo");
            system("/bin/stty cooked");
            exit(0);
        }

        if (debugMode)
            printf("Movimento possibile: %s\n", canMove(board) ? "Si" : "No");
    }
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
    printf("┌────┬────┬────┬────┐\n");
    // Tabella con i valori colorata
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board->board[i][j] == 0)
                printf("│    ");
            else if (board->board[i][j] == 2)
                printf("│" ANSI_COLOR_CYAN " %d  " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 4)
                printf("│" ANSI_COLOR_BLUE " %d  " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 8)
                printf("│" ANSI_COLOR_YELLOW " %d  " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 16)
                printf("│" ANSI_COLOR_GREEN " %d " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 32)
                printf("│" ANSI_COLOR_MAGENTA " %d " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 64)
                printf("│" ANSI_COLOR_RED " %d " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 128)
                printf("│" ANSI_COLOR_CYAN "%d " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 256)
                printf("│" ANSI_COLOR_BLUE "%d " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 512)
                printf("│" ANSI_COLOR_YELLOW " %d" ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 1024)
                printf("│" ANSI_COLOR_GREEN "%d" ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 2048)
                printf("│" ANSI_COLOR_MAGENTA "%d" ANSI_RESET, board->board[i][j]);
            else
                printf("│" ANSI_COLOR_RED " %d " ANSI_RESET, board->board[i][j]);
        }
        printf("│\n");
        if (i != 3) {
            printf("├────┼────┼────┼────┤\n");
        }
    }
    printf("└────┴────┴────┴────┘\n");
}

bool canMoveRight(Board *board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if (board->board[i][j] == board->board[i][j + 1] || board->board[i][j] == 0 ||
                board->board[i][j + 1] == 0) {  // Se due caselle adiacenti sono uguali o una è vuota
                if (debugMode)
                    printf("Can move right\n");
                return true;
            }
        }
    }
    return false;
}

bool canMoveLeft(Board *board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 3; j > 0; j--) {
            if (board->board[i][j] == board->board[i][j - 1] || board->board[i][j] == 0 ||
                board->board[i][j - 1] == 0) {
                if (debugMode)
                    printf("Can move left\n");
                return true;
            }
        }
    }
    return false;
}

bool canMoveUp(Board *board) {
    for (int i = 3; i > 0; i--) {
        for (int j = 0; j < 4; j++) {
            if (board->board[i][j] == board->board[i - 1][j] || board->board[i][j] == 0 ||
                board->board[i - 1][j] == 0) {
                if (debugMode)
                    printf("Can move up\n");
                return true;
            }
        }
    }
    return false;
}

bool canMoveDown(Board *board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (board->board[i][j] == board->board[i + 1][j] || board->board[i][j] == 0 ||
                board->board[i + 1][j] == 0) {
                if (debugMode)
                    printf("Can move down\n");
                return true;
            }
        }
    }
    return false;
}

// Zippo le 4 funzioni in una sola
bool canMove(Board *board) {
    if (canMoveRight(board) || canMoveLeft(board) || canMoveUp(board) || canMoveDown(board)) {
        return true;
    }
    return false;
}


void moveUp(Board *board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board->board[i][j] != 0) { // Se è 0 è Ok
                int k = i; // k è la riga sopra
                while (k > 0 && board->board[k - 1][j] == 0) { // Se la riga sopra è vuota
                    board->board[k - 1][j] = board->board[k][j]; // Sposta il valore
                    board->board[k][j] = 0; // La riga sotto diventa vuota
                    k--; // Sposta la riga sopra
                }
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (board->board[i][j] == board->board[i + 1][j]) { // Se due caselle adiacenti sono uguali
                board->board[i][j] *= 2; // Moltiplica per 2
                board->board[i + 1][j] = 0; // La casella sotto diventa vuota
                board->score += board->board[i][j]; // Aggiungi il punteggio
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
    while (board->board[i][j] != 0) { // Se la casella non è vuota genera nuove coordinate
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


void initializeDB() { // Creo la tabella per salvare i punteggi se non esiste gia'
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("2048.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nell'apertura del DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    char *sql = "CREATE TABLE IF NOT EXISTS leatherboard (" // Salvataggio dati dei punteggi
                "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "   score INTEGER NOT NULL,"
                "   name TEXT NOT NULL,"
                "   date datetime default current_timestamp"
                ");";
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_close(db);
}


int getScore(char *username) {  // Ottengo il punteggio massimo di un utente
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("2048.db", &db);
    if (rc != SQLITE_OK) {  // Controllo se il DB esiste
        fprintf(stderr, "Errore nell'apertura del DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    char *sql = "SELECT score FROM leatherboard WHERE name = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);  // Preparo la query  (sqlinj)
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nella preparazione della query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);    // Sostituisco il ? con username
    rc = sqlite3_step(stmt);    // Eseguo la query
    if (rc == SQLITE_ROW) { // Se esiste un punteggio per l'utente
        int score = sqlite3_column_int(stmt, 0);    // Ottengo il punteggio
        sqlite3_finalize(stmt); // "Chiudo" la query
        sqlite3_close(db);  // Chiudo la connessione al DB
        return score;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

void addScore(char *username, int score) {  // Aggiungo un punteggio per un utente (username o quello che si inserisce)
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("2048.db", &db);  // Controllo se il DB esiste
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nell'apertura del DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    char *sql = "INSERT INTO leatherboard (score, name) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nella preparazione della query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_bind_int(stmt, 1, score);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Errore nella query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void updateScore(char *username, int score) {   // Se esiste gia' un punteggio per l'utente lo aggiorno (la funzione non controlla, controllo necessario prima di chiamare la funzione)
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("2048.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nell'apertura del DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    char *sql = "UPDATE leatherboard SET score = ? WHERE name = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nella preparazione della query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_bind_int(stmt, 1, score);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Errore nella query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void printLeaderboard() {   // Stampa la classifica
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("2048.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nell'apertura del DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    char *sql = "SELECT name, score, date FROM leatherboard ORDER BY score DESC, date ASC LIMIT 10;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nella preparazione della query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    int cont = 0;
    printf("  Punti   |   Nome   |        Data  \n");
    printf("----------+----------+--------------------\n");
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        char *name = (char *) sqlite3_column_text(stmt, 0);
        int score = sqlite3_column_int(stmt, 1);
        char *date = (char *) sqlite3_column_text(stmt, 2);
        printf("%9d | %8s | %s \n", score, name, date);
        cont++;
    }
    if(cont == 0) {
        printf("      Nessun punteggio salvato.\n");
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void saveGame(Board *b) // Salva la partita in corso
{
    FILE *fp;
    fp = fopen("2048.dat", "w");
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        exit(1);
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // save b.board[i][j]
            fprintf(fp, "%d\n", b->board[i][j]);
        }
    }
}

bool containsParam(int argc, char *argv[], char *param) { // Implementazione per possibile ampliamento programma
    for (int i = 0; i < argc; i++) {
        if (strcmp(param, argv[i]) == 0) {
            return true;
        }
    }
    return false;
}

void loadGame(char *filename, Board *b){    // Carica una partita salvata (dal file 2048.dat, possibile ampliamento)
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        exit(1);
    }
    char line[10];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // load b.board[i][j]
            fgets(line, 10, fp);
            b->board[i][j] = atoi(line); // salvo il valore letto in board
        }
    }
}