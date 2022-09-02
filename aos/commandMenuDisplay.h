void goToCommandMode()
{
    cout<<"\033[H\033[J";
    mode = COMMAND_MODE;
    initializeNewDirectoryPointers();
    listDirectory(currPath);
    cout << "Enter Command: $ ";
    char ch;
    string ip;
    int size;
    bool prev=false;

    ip = "";
    size = 0;
    while (1)
    {
        ch = getchar();
        if(ch=='['){
            prev=true;
            continue;
        }
        if((ch=='A' || ch=='B' || ch=='C' || ch=='D') && prev){
            prev=false;
            continue;
        }
        if (ch == KEY_ESCAPE)
        {
            mode = NORMAL_MODE;
            initializeNewDirectoryPointers();
            listDirectory(currPath);
            return;
        }

        else if (ch == '\n')
        {
            break;
        }

        else if (ch == KEY_BACKSPACE)
        {
            if (size > 0)
            {
                cout << "\b \b";
                ip.pop_back();
                size--;
            }
        }
        else
        {
            ip = ip + ch;
            size++;
        }
        cout << ch;
    }

    vector<string> cmd;
    stringstream ss(ip);
    string tok;

    while (getline(ss, tok, ' '))
    {
        cmd.push_back(tok);
    }

    if (ip == "")
        return;

    if (cmd[0] == "search")
    {
        string fileToSearch = cmd[1];
        if (searchFileDir(currPath, fileToSearch))
        {
            cout << endl
                 << "True" << endl;
        }
        else
        {
            cout << endl
                 << "False" << endl;
        }
        usleep(2000000);
        initializeNewDirectoryPointers();
        listDirectory(currPath);
    }

    else if (cmd[0] == "goto")
    {
        string newPath = parsePath(cmd[1]);
        
        DIR *d = opendir(newPath.c_str());

        if (d == NULL)
        {
            cout << endl
                 << "Invalid Path" << endl;
            usleep(2000000);
        }
        else
        {
            currPath = newPath;
            clearStack(forwardSTK);
            if (backwardSTK.top() != currPath)
                backwardSTK.push(currPath);
        }
        initializeNewDirectoryPointers();
        listDirectory(currPath);
    }

    else if (cmd[0] == "copy")
    {
        if (cmd.size() < 3)
        {
            cout << endl
                 << "Too few arguments" << endl;
        }
        else
        {
            copy(cmd);
        }
        usleep(2000000);
        initializeNewDirectoryPointers();
        listDirectory(currPath);
    }

    else if (cmd[0] == "move")
    {
        if (cmd.size() < 3)
        {
            cout << endl
                 << "Too few arguments" << endl;
        }
        else
        {
            move(cmd);
        }
        usleep(2000000);
        initializeNewDirectoryPointers();
        listDirectory(currPath);
    }

    else if (cmd[0] == "create_dir")
    {
        if (cmd.size() < 3)
        {
            cout << endl
                 << "Too few arguments" << endl;
        }
        else if (cmd.size() > 3)
        {
            cout << endl
                 << "Too many arguments" << endl;
        }
        else
        {
            string dirToCreate = cmd[1];
            string destDir = cmd[2];
            destDir = parsePath(destDir);
            createDir(dirToCreate, destDir, globalPerm);
        }
        usleep(2000000);
        initializeNewDirectoryPointers();
        listDirectory(currPath);
    }

    else if (cmd[0] == "create_file")
    {
        if (cmd.size() < 3)
        {
            cout << endl
                 << "Too few arguments" << endl;
        }
        else if (cmd.size() > 3)
        {
            cout << endl
                 << "Too many arguments" << endl;
        }
        else
        {
            string fileToCreate = cmd[1];
            string destDir = cmd[2];
            createFile(fileToCreate, destDir);
        }
        usleep(2000000);
        initializeNewDirectoryPointers();
        listDirectory(currPath);
    }

    else if (cmd[0] == "rename")
    {
        if (cmd.size() < 3)
        {
            cout << endl
                 << "Too few arguments" << endl;
        }
        else if (cmd.size() > 3)
        {
            cout << endl
                 << "Too many arguments" << endl;
        }
        else
        {
            string from = cmd[1];
            string to = cmd[2];
            rname(from, to);
        }
        usleep(2000000);
        initializeNewDirectoryPointers();
        listDirectory(currPath);
    }

    else if (cmd[0] == "delete_file")
    {
        if (cmd.size() < 2)
        {
            cout << endl
                 << "Too few arguments" << endl;
        }
        else if (cmd.size() > 2)
        {
            cout << endl
                 << "Too many arguments" << endl;
        }
        else
        {
            delete_file(cmd[1]);
        }
        usleep(2000000);
        initializeNewDirectoryPointers();
        listDirectory(currPath);
    }

    else if (cmd[0] == "delete_dir")
    {

        if (cmd.size() < 2)
        {
            cout << endl
                 << "Too few arguments" << endl;
        }
        else if (cmd.size() > 2)
        {
            cout << endl
                 << "Too many arguments" << endl;
        }
        else
        {
            string dirToDelete = parsePath(cmd[1]);
            if(!canDelete(dirToDelete)){
                return;
            }
            delete_entire_dir(dirToDelete);
        }
        usleep(2000000);
        initializeNewDirectoryPointers();
        listDirectory(currPath);
    }

    else if (cmd[0] == "quit")
    {
        CLEAR_SCREEN;
        exit(1);
    }

    else
    {
        cout << endl
             << "Command Invalid" << endl;
        usleep(2000000);
        initializeNewDirectoryPointers();
        listDirectory(currPath);
    }
}