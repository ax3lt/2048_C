#include "Game_logic.h"


void gameHandler(Board *board) {
    initializeDB();
    printWelcomeMessage();
    board->dimX = 4; // Valori di default
    board->dimY = 4;
    char cmd;
    system("stty raw");
    system("stty -echo");
    cmd = getchar();
    system("stty cooked");
    system("stty echo");
    if (cmd == 's' || cmd == 'S') {
        setGridDimension(board);
    }
    if (cmd == 'L' || cmd == 'l') {
        system("clear");
        printLeaderboard();
        exit(0);
    }

    if (cmd == 'C' || cmd == 'c') {
        loadGame("2048.dat", board); // carica la partita salvata
    } else {
        initializeBoard(board);
        fillInitialValues(board);
    }
    fflush(stdin); // Cancello enter dal buffer
    system("clear");

    Vector messageBuffer;
    vector_init(&messageBuffer);

    char current_score[20];
    while (1) {
        bool validMove = false;  // Controllo se possibile fare un movimento valido
        bool restart = false;   // Controllo se è stato premuto R
        printf(ANSI_COLOR_GREEN
               "🚀 Punteggio: %d\n"
               "ℹ️ Round: %d"
               ANSI_RESET
               "\n\n", board->score, board->round);


        printBoard(board);

        if (vector_size(&messageBuffer) > 0) {  // Stampo i messaggi delle funzioni se presenti
            printf("\n");
            for (int i = 0; i < vector_size(&messageBuffer); i++) {
                printf("%s\n", (char *) vector_get(&messageBuffer, i));
            }
            vector_reset(&messageBuffer);
        }

        printf("\U0001F449 Usa WASD o le freccie per muoverti\n"
               "\U0001F449 Q per uscire\n"
               "\U0001F449 R per ricominciare\n"
               "\U0001F449 F per salvare\n"
               "\U0001F449 Z per ripristinare l'ultima mossa\n"
               "❔ Per visualizzare la leaderboard oppure per caricare una partita riavvia il programma\n");


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
                 c != 'S' && c != 'D' && c != 'Q' && c != 'R' && c != 'f' && c != 'F' && c != 'z' && c != 'Z');

        system("/bin/stty echo");   // Riabilita l'echo dei tasti premuti
        system("/bin/stty cooked"); // Riabilita il buffering dei tasti premuti

        if (c == 'f' || c == 'F') {
            saveGame(board);
            vector_push(&messageBuffer, ANSI_COLOR_GREEN
                                        "✅ Partita salvata con successo\n"
                                        ANSI_RESET);
        } else if (c == 'q' || c == 'Q') { // <-- Terminazione del gioco
            printf(ANSI_BG_RED
                   "\nHai scelto di uscire dal gioco\n"
                   ANSI_RESET);
            memoryFree(board);
            exit(1);
        } else if (c == 'r' || c == 'R') { // <-- Reimposto la tabella e rimetto i valori iniziali
            printf("Hai scelto di reiniziare il gioco\n");
            initializeBoard(board);
            fillInitialValues(board);
            restart = true;
            system("clear");
        } else if (c == 'z' || c == 'Z') {
            int tmpScr = atoi(current_score);
            handleRollback(board, &messageBuffer, tmpScr);
        } else if (c == 'w' || c == 'W' || c == 'a' || c == 'A' || c == 's' || c == 'S' || c == 'd' || c == 'D') {
            // Eseguo la mossa
            sprintf(current_score, "%d",
                    board->score);  // Converto il punteggio in stringa per poterlo ripristinare se serve
            validMove = handleMove(board, c);
            if (!validMove) {
                vector_push(&messageBuffer, ANSI_COLOR_RED
                                            "❌ Mossa non valida\n"
                                            ANSI_RESET
                                            "");
            }
        }

        if (!restart &&
            validMove) {    // <-- Se non è stato premuto R e se è stato premuto un tasto valido posso aggiungere un nuovo valore
            addNewRandom(board);
            board->round++;
        }
        system("clear");


        if (!canMove(board)) {  // <-- Se non è possibile muovere nessuna casella il gioco è finito
            loseHandler(board);
        }

        if (debugMode)
            printf("Movimento possibile: %s\n", canMove(board) ? "Si" : "No");
        // Print current round
    }
}

void handleRollback(Board *board, Vector *messageBuffer, int current_score) {
    // Ripristina l'ultima mossa
    if (board->round < 1) {
        vector_push(messageBuffer, ANSI_COLOR_RED
                                   "❌ Non hai ancora fatto la prima mossa!\n"
                                   ANSI_RESET
                                   "");
    } else {
        // Check if the current board is the same as the previous one
        bool sameBoard = true;
        for (int i = 0; i < board->dimX; i++) {
            for (int j = 0; j < board->dimY; j++) {
                if (board->board[i][j] != board->lastBoard[i][j]) {
                    sameBoard = false;
                    break;
                }
            }
        }
        if (sameBoard) {
            vector_push(messageBuffer, ANSI_COLOR_RED
                                       "❌ Hai già ripristinato l'ultima mossa!\n"
                                       ANSI_RESET
                                       "");
        } else {
            // Restore the previous board
            for (int i = 0; i < board->dimX; i++) {
                for (int j = 0; j < board->dimY; j++) {
                    board->board[i][j] = board->lastBoard[i][j];
                }
            }
            board->score = current_score;
            board->round--;
            vector_push(messageBuffer, ANSI_COLOR_GREEN
                                       "✅ Ultima mossa ripristinata!\n"        // Fastidioso?
                                       ANSI_RESET
                                       "");
        }
    }

}

bool handleMove(Board *board, char move) {
    bool validMove = false;
    if (move == 'w' || move == 'W') {  // <-- Muovo verso l'alto
        if (canMoveUp(board)) {
            saveBoard(board);
            moveUp(board);
            validMove = true;
        }
    } else if (move == 'a' || move == 'A') {  // <-- Muovo verso sinistra
        if (canMoveLeft(board)) {
            saveBoard(board);
            moveLeft(board);
            validMove = true;
        }
    } else if (move == 's' || move == 'S') {  // <-- Muovo verso il basso
        if (canMoveDown(board)) {
            saveBoard(board);
            moveDown(board);
            validMove = true;
        }
    } else if (move == 'd' || move == 'D') {  // <-- Muovo verso destra
        if (canMoveRight(board)) {
            saveBoard(board);
            moveRight(board);
            validMove = true;
        }
    }
    return validMove;
}

void saveBoard(Board *board) {
    for (int i = 0; i < board->dimX; i++) {
        for (int j = 0; j < board->dimY; j++) {
            board->lastBoard[i][j] = board->board[i][j];
        }
    }
}

void initializeBoard(Board *board) {
    board->board = calloc(board->dimX, sizeof(int *));
    board->score = 0;
    board->round = 0;
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




    // Salvo anche la tabella secondaria
    board->lastBoard = calloc(board->dimX, sizeof(int *));
    for (int i = 0; i < board->dimX; i++) {
        for (int j = 0; j < board->dimY; j++) {
            board->lastBoard[i] = calloc(board->dimY, sizeof(int));
            board->lastBoard[i][j] = board->board[i][j];
        }
    }
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
            else if (board->board[i][j] == 2)
                printf("│"
                       ANSI_COLOR_CYAN
                       " %d  "
                       ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 4)
                printf("│"
                       ANSI_COLOR_BLUE
                       " %d  "
                       ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 8)
                printf("│"
                       ANSI_COLOR_YELLOW
                       " %d  "
                       ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 16)
                printf("│"
                       ANSI_COLOR_GREEN
                       " %d "
                       ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 32)
                printf("│"
                       ANSI_COLOR_MAGENTA
                       " %d "
                       ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 64)
                printf("│"
                       ANSI_COLOR_RED
                       " %d "
                       ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 128)
                printf("│"
                       ANSI_COLOR_CYAN
                       "%d "
                       ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 256)
                printf("│"
                       ANSI_COLOR_BLUE
                       "%d "
                       ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 512)
                printf("│"
                       ANSI_COLOR_YELLOW
                       "%d "
                       ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 1024)
                printf("│"
                       ANSI_COLOR_GREEN
                       "%d"
                       ANSI_RESET, board->board[i][j]);
            else if (board->board[i][j] == 2048)
                printf("│"
                       ANSI_COLOR_MAGENTA
                       "%d"
                       ANSI_RESET, board->board[i][j]); // Il gioco finisce qui no?
            else
                printf("│"
                       ANSI_COLOR_RED
                       "%d"
                       ANSI_RESET, board->board[i][j]);
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

    // Salvo anche la tabella secondaria
    b->lastBoard = calloc(b->dimX, sizeof(int *));
    for (int i = 0; i < b->dimX; i++) {
        for (int j = 0; j < b->dimY; j++) {
            b->lastBoard[i] = calloc(b->dimY, sizeof(int));
            b->lastBoard[i][j] = b->board[i][j];
        }
    }
}

void setGridDimension(Board *b) {   // Imposta la dimensione della griglia
    char input[100];
    char *endptr;
    int dim;
    bool validInput = false;

    // Controllo se e' veramente un numero
    while (!validInput) {
        printf("Inserisci la larghezza della griglia: ");
        if (fgets(input, sizeof(input), stdin) ==
            NULL) { // Legge una riga intera (incluso il \n), se non riesce a leggere nulla (EOF) ritorna NULL
            printf("Input non valido.\n");
        }
        dim = strtol(input, &endptr,
                     10); // Converte la stringa in un int, primo parametro la stringa, secondo il puntatore alla fine della stringa (punta a quello che c'e dopo il numero (qualcosa di non valido), quindi volendo si puo usare per verificare piu numeri in basi diverse in un solo input), terzo la base (10 = decimale)
        if (!(endptr == input || *endptr !=
                                 '\n')) // Se endptr punta alla fine della stringa (non c'e' altro dopo il numero) allora e' un numero valido
            validInput = true; // Esco dal ciclo
    }
    b->dimX = dim;
    b->dimY = dim;
}

void printWelcomeMessage() {
    system("clear");
    printf(ANSI_FONT_BOLD
           ANSI_COLOR_RED
           "██████╗  ██████╗ ██╗  ██╗ █████╗     ██████╗ ██╗   ██╗     █████╗ ██╗  ██╗██████╗ ██╗  ████████╗\n"
           "╚════██╗██╔═████╗██║  ██║██╔══██╗    ██╔══██╗╚██╗ ██╔╝    ██╔══██╗╚██╗██╔╝╚════██╗██║  ╚══██╔══╝\n"
           " █████╔╝██║██╔██║███████║╚█████╔╝    ██████╔╝ ╚████╔╝     ███████║ ╚███╔╝  █████╔╝██║     ██║   \n"
           "██╔═══╝ ████╔╝██║╚════██║██╔══██╗    ██╔══██╗  ╚██╔╝      ██╔══██║ ██╔██╗  ╚═══██╗██║     ██║   \n"
           "███████╗╚██████╔╝     ██║╚█████╔╝    ██████╔╝   ██║       ██║  ██║██╔╝ ██╗██████╔╝███████╗██║   \n"
           "╚══════╝ ╚═════╝      ╚═╝ ╚════╝     ╚═════╝    ╚═╝       ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝   \n"
           "                                                                                                \n"
           ANSI_RESET);
    printf(ANSI_COLOR_YELLOW
           "➡️ "
           ANSI_RESET
           "Usa "
           ANSI_COLOR_GREEN
           " (W A S D)"
           ANSI_RESET
           " o le "
           ANSI_COLOR_GREEN
           "freccie "
           ANSI_RESET
           "per muovere le caselle\n");
    printf(ANSI_COLOR_YELLOW
           "➡️ "
           ANSI_RESET
           "Premi "
           ANSI_COLOR_GREEN
           "R"
           ANSI_RESET
           " per resettare il gioco\n");
    printf(ANSI_COLOR_YELLOW
           "➡️ "
           ANSI_RESET
           "Premi "
           ANSI_COLOR_GREEN
           "Q"
           ANSI_RESET
           " per uscire\n");
    printf(ANSI_COLOR_YELLOW
           "➡️ "
           ANSI_RESET
           "Puoi salvare il gioco premendo "
           ANSI_COLOR_GREEN
           "F"
           ANSI_RESET
           "\n");
    printf(ANSI_COLOR_YELLOW
           "➡️ "
           ANSI_RESET
           "Puoi ripristinare l'ultima mossa con "
           ANSI_COLOR_GREEN
           "Z"
           ANSI_RESET
           "\n");
    printf(ANSI_COLOR_YELLOW
           "➡️ "
           ANSI_RESET
           "Per caricare una partita salvata premi "
           ANSI_COLOR_GREEN
           "C"
           ANSI_RESET
           " (deve essere presente un file con il nome 2048.dat nella directory del programma)\n");
    printf(ANSI_COLOR_YELLOW
           "➡️ "
           ANSI_RESET
           "Premi "
           ANSI_COLOR_GREEN
           "L"
           ANSI_RESET
           " ora per visualizzare la leaderboard\n");
    printf(ANSI_COLOR_YELLOW
           "➡️ "
           ANSI_RESET
           "Premi "
           ANSI_COLOR_GREEN
           "S"
           ANSI_RESET
           " per cambiare le dimensioni della griglia\n");
    printf("\n"
           ANSI_COLOR_RED
           "✅ "
           ANSI_RESET
           "Premi qualsiasi tasto per iniziare\n");
}

void printLoseMessage() {
    printf(ANSI_COLOR_RED
           ""
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


void loseHandler(Board *board) {
    printLoseMessage();
    printf(ANSI_COLOR_RED
           "\nIl tuo punteggio finale è di: %d\n\n"
           ANSI_RESET, board->score);
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
        if (getScore(username, board->dimX) == 0) {
            addScore(username, board->score, board->dimX);
        } else {
            if (board->score > getScore(username, board->dimX)) {
                updateScore(username, board->score, board->dimX);
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
    for (int i = 0; i < board->dimX; i++) {
        free(board->board[i]);
    }
    free(board->board);
    free(board);
}