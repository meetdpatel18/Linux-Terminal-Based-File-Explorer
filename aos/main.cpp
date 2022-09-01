#include "headerFiles.h"
#include "getchNonCanonical.h"
#include "moveCursor.h"
#include "normalMode.h"
#include "commandMenuDisplay.h"
#include "commandModeFunctions.h"
#include "listDir.h"

int main()
{
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