string findParent(string path, int n)
{
    if (path == "/")
        return "/";
    string s = path.substr(0, n - 3);
    n = n - 3;
    int i = 0, lastSlash = INT_MIN;
    while (i != n)
    {
        if (s[i] == '/')
            lastSlash = i;
        i++;
    }
    s = s.substr(0, lastSlash);
    if (s == "")
        return "/";
    return s;
}

void initializeNewDirectoryPointers()
{
    startRow = 0;
    currRow = 0;
    dirVect.clear();
    CLEAR_SCREEN;
}

void clearStack(stack<string> &s)
{
    while (!s.empty())
    {
        s.pop();
    }
}

bool moveDown()
{
    if (currRow == dirVect.size() - 1)
        return false;
    if (currRow == startRow + rows - 1)
    {
        startRow++;
    }
    currRow++;
    CLEAR_SCREEN;
    return true;
}

bool moveUp()
{
    if (currRow == 0)
        return false;
    if (currRow == startRow)
    {
        startRow--;
    }
    currRow--;
    CLEAR_SCREEN;
    return true;
}

bool goInDirectory()
{
    if (currRow == 0 || currRow == 1)
        return false;
    else if (dirVect[currRow][0][0] == 'd')
    {
        clearStack(forwardSTK);
        currPath = dirVect[currRow][6];
        backwardSTK.push(currPath);
        initializeNewDirectoryPointers();
        return true;
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        string file = dirVect[currRow][6];
        execl("/usr/bin/xdg-open", "xdg-open", file.c_str(), NULL);
        exit(1);
    }

    return false;
}

bool goBackward()
{
    if (!backwardSTK.empty())
    {
        forwardSTK.push(backwardSTK.top());
        backwardSTK.pop();
        if (backwardSTK.empty())
        {
            backwardSTK.push(forwardSTK.top());
            forwardSTK.pop();
            return false;
        }
        currPath = backwardSTK.top();
        initializeNewDirectoryPointers();
        return true;
    }
    return false;
}

bool goForward()
{
    if (forwardSTK.empty())
    {
        return false;
    }
    currPath = forwardSTK.top();
    backwardSTK.push(forwardSTK.top());
    forwardSTK.pop();
    initializeNewDirectoryPointers();
    return true;
}

bool goParent()
{
    if (currPath == "/")
        return false;
    clearStack(forwardSTK);
    currPath = findParent(currPath, currPath.size());
    backwardSTK.push(currPath);
    initializeNewDirectoryPointers();
    return true;
}

bool goHome()
{
    const char *homePath;

    if ((homePath = getenv("HOME")) == NULL)
    {
        homePath = getpwuid(getuid())->pw_dir;
    }
    if (currPath == homePath)
        return false;
    currPath = homePath;
    clearStack(forwardSTK);
    backwardSTK.push(currPath);
    initializeNewDirectoryPointers();
    return true;
}