#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "Libraries/SQLite3/sqlite3.h"

#define DOC
#define ENDDOC

DOC
/*
 *
 *   • Il programma funziona su sistemi UNIX/Linux
 *   • Per compilare il programma è necessario installare la libreria SQLite3 (sudo apt-get install libsqlite3-dev) e compilare con il comando gcc 2048.c -o 2048 -lsqlite3
 *   • Il programma crea un database SQLite3 chiamato "2048.db" nella cartella in cui è stato eseguito
 *   • Codici di uscita:
 *       • 0: Esecuzione terminata correttamente
 *       • 1: Uscita dal programma tramite tasto 'q'
 *       • 2: Uscita per sconfitta
 *
 *       • 10 Errore generico SQLITE3
 *       • 20 Errore apertura DB
 *       • 30 Errore preparazione query
 *       • 40 Errore nella query (DB side)
 *       • 100 Errore apertura file di salvataggio
 *       • 110 File di salvataggio non valido (numero non possibile in 2048)
 *       • 120 File di salvataggio non valido (quantita' numeri non corretta)
*/
ENDDOC



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
//    int board[4][4];
    int **board;
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

void setGridDimension(Board *b);

void printWelcomeMessage();

void printLoseMessage();

void gameHandler(Board *board);

void loseHandler(Board *board);

void memoryFree(Board *board);

bool debugMode = false;


int main(int argc, char *argv[]) {
    if (containsParam(argc, argv, "-d")) // se il programma viene lanciato con il parametro -d, attiva la modalità debug
        debugMode = true;

    initializeDB();

    Board *board = malloc(sizeof(Board));
    gameHandler(board);
}

void initializeBoard(Board *board) {
    board->board = calloc(board->dimX, sizeof(int *));
    board->score = 0;
    for (int i = 0; i < board->dimX; i++) {
        for (int j = 0; j < board->dimY; j++) {
            board->board[i] = calloc(board->dimY, sizeof(int));
            board->board[i][j] = 0;
        }
    }
}

void fillInitialValues(Board *board) {
    srand(time(NULL)); // Inizializza il generatore di numeri casuali
    int i1, j1;
    i1 = rand() % board->dimX; // Genera un numero casuale tra 0 e 3 per la coordinata
    j1 = rand() % board->dimY;

    int value = rand() % 2 + 1; // Genera un numero casuale tra 1 e 2 per il valore della casella
    if (value == 1) {
        value = 2;
    } else {
        value = 4;
    }
    board->board[i1][j1] = value;

    // Scelta seconda casella
    int i2 = rand() % board->dimX;
    int j2 = rand() % board->dimY;
    while (i1 == i2 && j2 == j1) {
        i2 = rand() % board->dimX;
        j2 = rand() % board->dimY;
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
    printf("┌");
    for (int i = 0; i < board->dimX - 1; i++) {
        printf("────┬");
    }
    printf("────┐\n");


    // Tabella con i valori colorata
    for (int i = 0; i < board->dimX; i++) {
        for (int j = 0; j < board->dimY; j++) {
            if (board->board[i][j] == 0) printf("│    ");
            else if (board->board[i][j] == 2) printf("│" ANSI_COLOR_CYAN " %d  " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 4) printf("│" ANSI_COLOR_BLUE " %d  " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 8) printf("│" ANSI_COLOR_YELLOW " %d  " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 16) printf("│" ANSI_COLOR_GREEN " %d " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 32) printf("│" ANSI_COLOR_MAGENTA " %d " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 64) printf("│" ANSI_COLOR_RED " %d " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 128) printf("│" ANSI_COLOR_CYAN "%d " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 256) printf("│" ANSI_COLOR_BLUE "%d " ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 512) printf("│" ANSI_COLOR_YELLOW " %d" ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 1024) printf("│" ANSI_COLOR_GREEN "%d" ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 2048) printf("│" ANSI_COLOR_MAGENTA "%d" ANSI_RESET, board->board[i][j]); // Il gioco finisce qui no?
            else printf("│" ANSI_COLOR_RED "%d" ANSI_RESET, board->board[i][j]);
        }
        printf("│\n");
        if (i != board->dimX - 1) {
            printf("├");
            for (int l = 0; l < board->dimX - 1; l++) {
                printf("────┼");
            }
            printf("────┤\n");
        }
    }
    printf("└");
    for (int i = 0; i < board->dimX - 1; i++) {
        printf("────┴");
    }
    printf("────┘\n");

}

bool canMoveRight(Board *board) {
    for (int i = 0; i < board->dimX; i++) {
        for (int j = 0; j < board->dimY - 1; j++) {
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
    for (int i = 0; i < board->dimX; i++) {
        for (int j = board->dimY - 1; j > 0; j--) {
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
    for (int i = board->dimX - 1; i > 0; i--) {
        for (int j = 0; j < board->dimY; j++) {
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
    for (int i = 0; i < board->dimX - 1; i++) {
        for (int j = 0; j < board->dimY; j++) {
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
    for (int i = 0; i < board->dimX; i++) {
        for (int j = 0; j < board->dimY; j++) {
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

    for (int i = 0; i < board->dimX - 1; i++) {
        for (int j = 0; j < board->dimY; j++) {
            if (board->board[i][j] == board->board[i + 1][j]) { // Se due caselle adiacenti sono uguali
                board->board[i][j] *= 2; // Moltiplica per 2
                board->board[i + 1][j] = 0; // La casella sotto diventa vuota
                board->score += board->board[i][j]; // Aggiungi il punteggio
            }
        }
    }
}

void moveDown(Board *board) {
    for (int i = board->dimX - 1; i >= 0; i--) {
        for (int j = 0; j < board->dimY; j++) {
            if (board->board[i][j] != 0) {
                int k = i;
                while (k < board->dimX - 1 && board->board[k + 1][j] == 0) {
                    board->board[k + 1][j] = board->board[k][j];
                    board->board[k][j] = 0;
                    k++;
                }
            }
        }
    }

    for (int i = board->dimX - 1; i > 0; i--) {
        for (int j = 0; j < board->dimY; j++) {
            if (board->board[i][j] == board->board[i - 1][j]) {
                board->board[i][j] *= 2;
                board->board[i - 1][j] = 0;
                board->score += board->board[i][j];
            }
        }
    }
}

void moveLeft(Board *board) {
    for (int i = 0; i < board->dimX; i++) {
        for (int j = 0; j < board->dimY; j++) {
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

    for (int i = 0; i < board->dimX; i++) {
        for (int j = 0; j < board->dimY - 1; j++) {
            if (board->board[i][j] == board->board[i][j + 1]) {
                board->board[i][j] *= 2;
                board->board[i][j + 1] = 0;
                board->score += board->board[i][j];
            }
        }
    }
}

void moveRight(Board *board) {
    for (int i = 0; i < board->dimX; i++) {
        for (int j = board->dimY - 1; j >= 0; j--) {
            if (board->board[i][j] != 0) {
                int k = j;
                while (k < board->dimX - 1 && board->board[i][k + 1] == 0) {
                    board->board[i][k + 1] = board->board[i][k];
                    board->board[i][k] = 0;
                    k++;
                }
            }
        }
    }

    for (int i = 0; i < board->dimX; i++) {
        for (int j = board->dimY - 1; j > 0; j--) {
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
    i = rand() % board->dimX;
    j = rand() % board->dimY;
    while (board->board[i][j] != 0) { // Se la casella non è vuota genera nuove coordinate
        i = rand() % board->dimX;
        j = rand() % board->dimY;
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
        exit(20);
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
        exit(10);
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
        exit(20);
    }
    char *sql = "SELECT score FROM leatherboard WHERE name = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);  // Preparo la query  (sqlinj)
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nella preparazione della query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(30);
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
        exit(20);
    }
    char *sql = "INSERT INTO leatherboard (score, name) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nella preparazione della query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(30);
    }
    sqlite3_bind_int(stmt, 1, score);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Errore nella query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(40);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void updateScore(char *username,
                 int score) {   // Se esiste gia' un punteggio per l'utente lo aggiorno (la funzione non controlla, controllo necessario prima di chiamare la funzione)
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("2048.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nell'apertura del DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(20);
    }
    char *sql = "UPDATE leatherboard SET score = ? WHERE name = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nella preparazione della query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(30);
    }
    sqlite3_bind_int(stmt, 1, score);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Errore nella query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(40);
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
        exit(20);
    }
    char *sql = "SELECT name, score, date FROM leatherboard ORDER BY score DESC, date ASC LIMIT 10;"; // Solo i primi 10, ordinati per punteggio e data
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nella preparazione della query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(30);
    }
    int cont = 0;
    printf("        " ANSI_BG_MAGENTA "LEATHERBOARD 2048\n\n" ANSI_RESET);
    printf("  Punti   |   Nome   |        Data  \n");
    printf("----------+----------+--------------------\n");
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        char *name = (char *) sqlite3_column_text(stmt, 0);
        char name2[5];
        strncpy(name2, name, 4);
        int score = sqlite3_column_int(stmt, 1);
        char *date = (char *) sqlite3_column_text(stmt, 2);
        printf("%9d | %8.*s | %s \n", score, 5, name,
               date); // I numeri servono per allineare la stampa (9 -> 9 spazi occupati, poi prende quelli del numero e li rimpiazza, se usavo -9 allieanemnto a sinistra)
        // Invice il .* prende i primi 5 caratteri della stringa (se la stringa e' piu' corta prende meno)
        cont++;
    }
    if (cont == 0) {
        printf("      Nessun punteggio salvato.\n");
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void saveGame(Board *b) // Salva la partita in corso
{
    FILE *fp;
    fp = fopen("2048.dat", "w");
//    if (fp == NULL)
//    {
//        printf("Errore nell'apertura del file.\n");       // Non necessario? Con w crea il file se non esiste
//        exit(100);
//    }
    fprintf(fp, "%d\n", b->dimX);
    fprintf(fp, "%d\n", b->dimY);
    fprintf(fp, "%d\n", b->score);
    for (int i = 0; i < b->dimX; i++) {
        for (int j = 0; j < b->dimY; j++) {
            fprintf(fp, "%d\n", b->board[i][j]);
        }
    }
    fclose(fp);
}


void loadGame(char *filename, Board *b) {    // Carica una partita salvata (dal file 2048.dat, possibile ampliamento)
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore nell'apertura del file.\n");
        exit(100);
    }
    int cont = 0;
    char line[10];

    // Le prime due righe sono per le dimensioni della tabella
    fgets(line, 10, fp);
    b->dimX = atoi(line);
    fgets(line, 10, fp);
    b->dimY = atoi(line);
    fgets(line, 10, fp);
    b->score = atoi(line);

    b->board = calloc(b->dimX, sizeof(int *));
    for (int i = 0; i < b->dimX; i++) {
        for (int j = 0; j < b->dimY; j++) {
            b->board[i] = calloc(b->dimY, sizeof(int));
        }
    }

    for (int i = 0; i < b->dimX; i++) {
        for (int j = 0; j < b->dimY; j++) {
            fgets(line, 10, fp);
            if (atoi(line) != 0 && atoi(line) != 2 && atoi(line) != 4 && atoi(line) != 8 && atoi(line) != 16 &&
                atoi(line) != 32 &&
                atoi(line) != 64 && atoi(line) != 128 && atoi(line) != 256 && atoi(line) != 512 && atoi(line) != 1024 &&
                atoi(line) != 2048) {
                printf("File di salvataggio del gioco non valido.%d\n", atoi(line));
                exit(110);
            }

            b->board[i][j] = atoi(line);
            cont++;
        }
    }
    fclose(fp);
    if (cont != (b->dimX * b->dimY)) { // Se il file e' piu' lungo della quantita di celle non e' valido
        printf("File di salvataggio del gioco non valido.\n");
        exit(120);
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

void setGridDimension(Board *b) {   // Imposta la dimensione della griglia
    int dim;
    printf("Inserisci la dimensione verticale della griglia: ");
    scanf("%d", &dim);
    b->dimX = dim;
    printf("Inserisci la dimensione orizzontale della griglia: ");
    scanf("%d", &dim);
    b->dimY = dim;
}

void printWelcomeMessage() {
    system("clear");
    printf(ANSI_FONT_BOLD ANSI_COLOR_RED
           "██████╗  ██████╗ ██╗  ██╗ █████╗     ██████╗ ██╗   ██╗     █████╗ ██╗  ██╗██████╗ ██╗  ████████╗\n"
           "╚════██╗██╔═████╗██║  ██║██╔══██╗    ██╔══██╗╚██╗ ██╔╝    ██╔══██╗╚██╗██╔╝╚════██╗██║  ╚══██╔══╝\n"
           " █████╔╝██║██╔██║███████║╚█████╔╝    ██████╔╝ ╚████╔╝     ███████║ ╚███╔╝  █████╔╝██║     ██║   \n"
           "██╔═══╝ ████╔╝██║╚════██║██╔══██╗    ██╔══██╗  ╚██╔╝      ██╔══██║ ██╔██╗  ╚═══██╗██║     ██║   \n"
           "███████╗╚██████╔╝     ██║╚█████╔╝    ██████╔╝   ██║       ██║  ██║██╔╝ ██╗██████╔╝███████╗██║   \n"
           "╚══════╝ ╚═════╝      ╚═╝ ╚════╝     ╚═════╝    ╚═╝       ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝   \n"
           "                                                                                                \n" ANSI_RESET);
    printf(ANSI_COLOR_YELLOW "➡️ " ANSI_RESET "Usa " ANSI_COLOR_GREEN " (W A S D)" ANSI_RESET " o le " ANSI_COLOR_GREEN "freccie " ANSI_RESET "per muovere le caselle\n");
    printf(ANSI_COLOR_YELLOW "➡️ " ANSI_RESET "Premi " ANSI_COLOR_GREEN "R" ANSI_RESET " per resettare il gioco\n");
    printf(ANSI_COLOR_YELLOW "➡️ " ANSI_RESET "Premi " ANSI_COLOR_GREEN "Q" ANSI_RESET " per uscire\n");
    printf(ANSI_COLOR_YELLOW "➡️ " ANSI_RESET "Puoi salvare il gioco premendo " ANSI_COLOR_GREEN "F" ANSI_RESET "\n");
    printf(ANSI_COLOR_YELLOW "➡️ " ANSI_RESET "Per caricare una partita salvata premi "  ANSI_COLOR_GREEN "C" ANSI_RESET " (deve essere presente un file con il nome 2048.dat nella directory del programma)\n");
    printf(ANSI_COLOR_YELLOW "➡️ " ANSI_RESET "Premi "  ANSI_COLOR_GREEN "L" ANSI_RESET " ora per visualizzare la leatherboard\n");
    printf(ANSI_COLOR_YELLOW "➡️ " ANSI_RESET "Premi "  ANSI_COLOR_GREEN "S" ANSI_RESET " per cambiare le dimensioni della griglia\n");
    printf("\n" ANSI_COLOR_RED "✅ " ANSI_RESET "Premi qualsiasi tasto per iniziare\n");
}

void printLoseMessage() {
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
}

void gameHandler(Board *board) {
    printWelcomeMessage();
    board->dimX = 4; // Valori di default
    board->dimY = 4;
    char tmp;
    system("stty raw");
    system("stty -echo");
    tmp = getchar();
    //scanf("%c", &tmp);
    system("stty cooked");
    system("stty echo");
    if (tmp == 's' || tmp == 'S') {
        setGridDimension(board);
    }
    if (tmp == 'L' || tmp == 'l') {
        system("clear");
        printLeaderboard();
        exit(0);
    }

    if (tmp == 'C' || tmp == 'c') {
        loadGame("2048.dat", board); // carica la partita salvata
    } else {
        initializeBoard(board);
        fillInitialValues(board);
    }
    fflush(stdin); // Cancello enter dal buffer
    system("clear");


    while (1) {
        bool validMove = true;  // Controllo se possibile fare un movimento valido
        bool restart = false;   // Controllo se è stato premuto R
        printf(ANSI_COLOR_GREEN "🚀 Punteggio: %d" ANSI_RESET "\n\n", board->score);
        printBoard(board);
        printf("\U0001F449 Usa WASD o le freccie per muoverti\n"
               "\U0001F449 Q per uscire\n"
               "\U0001F449 R per ricominciare\n"
               "\U0001F449 F per salvare\n"
               "❔ Per visualizzare la leatherboard oppure per caricare una partita riavvia il programma\n");


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


        if (c == 'f' || c == 'F') {
            saveGame(board);
            validMove = false;
            if (debugMode)
                printf("Partita salvata!\n");
        } else if (c == 'q' || c == 'Q') { // <-- Terminazione del gioco
            printf(ANSI_BG_RED "\nHai scelto di uscire dal gioco\n" ANSI_RESET);
            memoryFree(board);
            exit(1);
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
            loseHandler(board);
        }

        if (debugMode)
            printf("Movimento possibile: %s\n", canMove(board) ? "Si" : "No");
    }
}

void loseHandler(Board *board) {
    printLoseMessage();
    printf(ANSI_COLOR_RED "\nIl tuo punteggio finale è di: %d\n\n" ANSI_RESET, board->score);
    // prompt to save the score
    printf("Vuoi salvare il tuo punteggio? (S/n): ");
    char save;
    scanf(" %c", &save);
    if (save == 's' || save == 'S') {
        // prompt the username
        printf("Inserisci il tuo nome utente (massimo 5 caratteri): ");
        char username[5];
        scanf("%5s", username); // 5 caratteri max
        // Salvo il punteggio
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
    char c;
    system("/bin/stty -echo");
    system("/bin/stty raw");
    c = getchar();
    system("/bin/stty echo");
    system("/bin/stty cooked");

    memoryFree(board);
    exit(2);
}

void memoryFree(Board *board) {

}