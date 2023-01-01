#include "src/Global_Declarations.h"
#include "src/Game/Game_logic.c"


DOC
/*
 *
 *   • Il programma funziona su sistemi UNIX/Linux
 *   • Per compilare il programma è necessario installare la libreria SQLite3 (sudo apt-get install libsqlite3-dev) e compilare con il comando gcc main.c -o 2048 -lsqlite3
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


int main(int argc, char *argv[]) {
    if (containsParam(argc, argv, "-d")) // se il programma viene lanciato con il parametro -d, attiva la modalità debug
        debugMode = true;

    Board *board = malloc(sizeof(Board));
    gameHandler(board);

//    system("chmod +x script.sh");
//    system("./script.sh");
}



bool containsParam(int argc, char *argv[], char *param) { // Implementazione per possibile ampliamento programma
    for (int i = 0; i < argc; i++) {
        if (strcmp(param, argv[i]) == 0) {
            return true;
        }
    }
    return false;
}