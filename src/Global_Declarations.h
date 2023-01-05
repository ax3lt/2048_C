#ifndef INC_2048_MAC_ANSI_DECLARATIONS_H
#define INC_2048_MAC_ANSI_DECLARATIONS_H

#endif //INC_2048_MAC_ANSI_DECLARATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

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




// Docs
#define DOC
#define ENDDOC


bool debugMode = false;
bool containsParam(int argc, char *argv[], char *param);
