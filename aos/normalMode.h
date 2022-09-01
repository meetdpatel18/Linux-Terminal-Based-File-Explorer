void normalMode()
{
    CLEAR_SCREEN;
    initializeNewDirectoryPointers();

    listDirectory(currPath);

    int c;
    string xb;

    while (1)
    {
        if (mode == NORMAL_MODE)
        {
            c = readInputNormalMode();
            switch (c)
            {
            case KEY_COLON:
                goToCommandMode();
                break;
            case KEY_ESCAPE:
                xb="ESCAPE PRESSED";
                write(STDOUT_FILENO, xb.c_str(), xb.size());
                break;
            case KEY_DOWN:
                if (moveDown())
                    display();
                break;
            case KEY_UP:
                if (moveUp())
                    display();
                break;
            case KEY_ENTER:
                if (goInDirectory())
                    listDirectory(currPath);
                break;
            case KEY_LEFT:
                if (goBackward())
                    listDirectory(currPath);
                break;
            case KEY_RIGHT:
                if (goForward())
                    listDirectory(currPath);
                break;
            case KEY_BACKSPACE:
                if (goParent())
                    listDirectory(currPath);
                break;
            case KEY_HOME:
                if (goHome())
                    listDirectory(currPath);
                break;
            case KEY_EXIT:
                CLEAR_SCREEN;
                exit(1);
                break;

            default:
                break;
            }
        }
        else
        {
            goToCommandMode();
        }
    }

    dirVect.clear();
}