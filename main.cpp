#include "headerFiles.h"
#include "getchNonCanonical.h"
#include "moveCursor.h"
#include "normalMode.h"
#include "commandMenuDisplay.h"
#include "commandModeFunctions.h"
#include "listDir.h"


int main()
{
    // // string s="Hello";
    // // cout<<"\x1B[31m" + s + "\033[0m\t\t";

    // printf("\n");
    // printf("\x1B[31mTexting\033[0m\t\t");printf("\n");
    // printf("\x1B[32mTexting\033[0m\t\t");printf("\n");
    // printf("\x1B[33mTexting\033[0m\t\t");printf("\n");
    // printf("\x1B[34mTexting\033[0m\t\t");printf("\n");
    // printf("\x1B[35mTexting\033[0m\n");printf("\n");
    
    // printf("\x1B[36mTexting\033[0m\t\t");printf("\n");
    // printf("\x1B[36mTexting\033[0m\t\t");printf("\n");
    // printf("\x1B[36mTexting\033[0m\t\t");printf("\n");
    // printf("\x1B[37mTexting\033[0m\t\t");printf("\n");
    // printf("\x1B[93mTexting\033[0m\n");printf("\n");
    
    // printf("\033[3;42;30mTexting\033[0m\t\t");printf("\n");
    // printf("\033[3;43;30mTexting\033[0m\t\t");printf("\n");
    // printf("\033[3;44;30mTexting\033[0m\t\t");printf("\n");
    // printf("\033[3;104;30mTexting\033[0m\t\t");printf("\n");
    // printf("\033[3;100;30mTexting\033[0m\n");printf("\n");

    // printf("\033[3;47;35mTexting\033[0m\t\t");printf("\n");
    // printf("\033[1;47;35mTexting\033[0m\t\t");printf("\n");
    // printf("\033[1;107;30mTexting\033[0m\t\t");printf("\n");
    // printf("\t\t");
    // printf("\n");
    // return 0;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &w);
    rows=w.ws_row;
    cols=w.ws_col;
    signal(SIGWINCH, dynamicWindowResize);
    currPath = getenv("HOME");
    backwardSTK.push(currPath);
    mode=NORMAL_MODE;
    enableRawMode();
    normalMode();
    disableRawMode();
    return 0;
}