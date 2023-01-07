#include "DB_logic.h"

void initializeDB() { // Creo la tabella per salvare i punteggi se non esiste gia'
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("2048.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nell'apertura del DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(20);
    }
    char *sql = "CREATE TABLE IF NOT EXISTS leaderboard (" // Salvataggio dati dei punteggi
                "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "   score INTEGER NOT NULL,"
                "   name TEXT NOT NULL,"
                "   gridLayout INTEGER NOT NULL,"
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


int getScore(char *username, int boardDim) {  // Ottengo il punteggio massimo di un utente
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("2048.db", &db);
    if (rc != SQLITE_OK) {  // Controllo se il DB esiste
        fprintf(stderr, "Errore nell'apertura del DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(20);
    }
    char *sql = "SELECT score FROM leaderboard WHERE name = ? AND gridLayout = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);  // Preparo la query  (sqlinj)
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nella preparazione della query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(30);
    }
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);    // Sostituisco il ? con username
    sqlite3_bind_int(stmt, 2, boardDim);    // Sostituisco il ? boardDim
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

void addScore(char *username, int score, int boardDim) {  // Aggiungo un punteggio per un utente (username o quello che si inserisce)
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("2048.db", &db);  // Controllo se il DB esiste
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nell'apertura del DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(20);
    }
    char *sql = "INSERT INTO leaderboard (score, name, gridLayout) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nella preparazione della query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(30);
    }
    sqlite3_bind_int(stmt, 1, score);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, boardDim);
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
                 int score, int boardDim) {   // Se esiste gia' un punteggio per l'utente lo aggiorno (la funzione non controlla, controllo necessario prima di chiamare la funzione)
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("2048.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nell'apertura del DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(20);
    }
    char *sql = "UPDATE leaderboard SET score = ? WHERE name = ? AND gridLayout = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nella preparazione della query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(30);
    }
    sqlite3_bind_int(stmt, 1, score);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, boardDim);
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
    char *sql = "SELECT name, score, gridLayout, date FROM leaderboard ORDER BY score DESC, gridLayout DESC, date ASC LIMIT 10;"; // Solo i primi 10, ordinati per punteggio e data
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Errore nella preparazione della query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(30);
    }
    int cont = 0;
    printf("              " ANSI_BG_MAGENTA "LEADERBOARD 2048\n\n" ANSI_RESET);
    printf("  Punti   |   Nome   | Griglia |        Data         \n");
    printf("----------+----------+---------+--------------------+\n");
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        char *name = (char *) sqlite3_column_text(stmt, 0);
        char name2[5];
        strncpy(name2, name, 4);
        int score = sqlite3_column_int(stmt, 1);
        int gridLayout = sqlite3_column_int(stmt, 2);
        char *date = (char *) sqlite3_column_text(stmt, 3);
        printf("%9d | %8.*s | %6dx | %s\n", score, 5, name, gridLayout, date); // I numeri servono per allineare la stampa (9 -> 9 spazi occupati, poi prende quelli del numero e li rimpiazza, se usavo -9 allieanemnto a sinistra)
        // Invice il .* prende i primi 5 caratteri della stringa (se la stringa e' piu' corta prende meno)
        cont++;
    }
    if (cont == 0) {
        printf("      Nessun punteggio salvato.\n");
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}