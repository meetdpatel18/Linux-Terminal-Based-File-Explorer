void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

char readInputNormalMode()
{
    char key;
    read(STDIN_FILENO, &key, 1);

    if (key == '\x1b')
    {
        char sq[3];
        if (read(STDIN_FILENO, &sq[0], 1) != 1)
            return '-';
        if(sq[0]==KEY_COLON){
            return KEY_COLON;
        }
        if (read(STDIN_FILENO, &sq[1], 1) != 1)
            return '-';
        if(sq[1]==KEY_COLON){
            return KEY_COLON;
        }
        if (sq[0] == '[')
        {
            switch (sq[1])
            {
            case KEY_UP:
                return KEY_UP;
                break;
            case KEY_DOWN:
                return KEY_DOWN;
                break;
            case KEY_LEFT:
                return KEY_LEFT;
                break;
            case KEY_RIGHT:
                return KEY_RIGHT;
                break;
            }
        }
    }

    return key;
}

char readInputCommandMode()
{
    char key;
    read(STDIN_FILENO, &key, 1);

    if (key == '\x1b')
    {
        return KEY_ESCAPE;
        char sq[3];
        if (read(STDIN_FILENO, &sq[0], 1) != 1)
            return '.';
        if (read(STDIN_FILENO, &sq[1], 1) != 1)
            return '.';
        if (sq[0] == '[')
        {
            switch (sq[1])
            {
            case KEY_UP:
                return KEY_UP;
                break;
            case KEY_DOWN:
                return KEY_DOWN;
                break;
            case KEY_LEFT:
                return KEY_LEFT;
                break;
            case KEY_RIGHT:
                return KEY_RIGHT;
                break;
            }
        }
    }
    return key;
}


string setPrec(double s, int p)
{
    stringstream stream;

    stream.precision(p);
    stream << fixed;

    stream << s;
    return stream.str();
}