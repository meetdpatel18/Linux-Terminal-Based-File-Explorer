//----------------------------------
//---------SEARCH FILE--------------
//--------OR DIRECTORY--------------
//----------------------------------
bool searchFileDir(string path, string fileToSearch)
{
    DIR *dir;
    struct dirent *x;
    string s;
    bool isD;

    if ((dir = opendir(path.c_str())) != NULL)
    {
        while ((x = readdir(dir)) != NULL)
        {
            s = x->d_name;
            if (s == "." || s == "..")
                continue;

            isD = isDir(path);

            if (fileToSearch == s)
            {
                return true;
            }
            else
            {
                if (isD)
                {
                    if (searchFileDir(path + "/" + s, fileToSearch))
                        return true;
                }
            }
        }
        closedir(dir);
    }
    return false;
}

//----------------------------------
//---------DELETE CHECKER-----------
//-----------FUNCTION---------------
//----------------------------------
bool canDelete(string src)
{
    int ind = currPath.find(src);

    if (ind != string::npos && ind == 0)
    {
        cout << endl
             << "Cannot Delete Directory go to same level as that of directory that you want to delete" << endl;
        usleep(3000000);
        return false;
    }
    return true;
}

//----------------------------------
//-------------MOVE-----------------
//-----------FUNCTION---------------
//----------------------------------
void move(vector<string> path)
{
    int n = path.size();
    string dest = path[n - 1];
    dest = parsePath(dest);

    for (int i = 1; i < n - 1; i++)
    {
        string src = path[i];
        src = parsePath(src);

        struct stat ip;
        if (stat((src).c_str(), &ip) != 0)
        {
            cout << endl
                 << "Unable to find information for specified path" << endl;
            continue;
        }

        if (S_ISDIR(ip.st_mode))
        {
            if (canDelete(src))
            {
                copyDirectory(src, dest);
                delete_entire_dir(src);
            }
        }
        else
        {
            copyFile(src, dest);
            delete_file(src);
        }
    }
}

//----------------------------------
//-------------COPY-----------------
//-----------FUNCTION---------------
//----------------------------------
void copy(vector<string> path)
{
    int n = path.size();
    string dest = path[n - 1];
    dest = parsePath(dest);

    for (int i = 1; i < n - 1; i++)
    {
        string src = path[i];
        src = parsePath(src);

        struct stat ip;
        if (stat((src).c_str(), &ip) != 0)
        {
            cout << "Unable to find information for specified path" << endl;
            continue;
        }

        if (S_ISDIR(ip.st_mode))
        {
            copyDirectory(src, dest);
        }
        else
        {
            copyFile(src, dest);
        }
    }
}

//----------------------------------
//-----------COPY FILE--------------
//-----------FUNCTION---------------
//----------------------------------
void copyFile(string sourceFile, string destFile)
{

    int fd_src, fd_dest;
    mode_t perm = getPerm(sourceFile);
    string fileName = getFileName(sourceFile);
    char ch;

    if (destFile == "/")
    {
        destFile = destFile + fileName;
    }
    else
    {
        destFile = destFile + "/" + fileName;
    }

    if ((fd_src = open(sourceFile.c_str(), O_RDONLY)) == -1)
    {
        cout << "Error Opening File" << endl;
        return;
    }
    if ((fd_dest = open(destFile.c_str(), O_CREAT | O_WRONLY, perm)) == -1)
    {
        cout << "File Already Exists" << endl;
        return;
    }
    while (read(fd_src, &ch, 1))
        write(fd_dest, &ch, 1);

    close(fd_src);
    close(fd_dest);
}

//----------------------------------
//--------COPY DIRECTORY------------
//-----------FUNCTION---------------
//----------------------------------
void copyDirectory(string src, string dest)
{
    string dirName = getFileName(src);
    string s;
    string tempPath;
    mode_t perm = getPerm(src);
    createDir(dirName, dest, perm);

    DIR *directory = opendir(src.c_str());

    if (!directory)
    {
        cout << "Path does not exist" << endl;
        return;
    }

    struct dirent *x;
    while ((x = readdir(directory)) != NULL)
    {
        s = x->d_name;

        if (src == "/")
        {
            tempPath = src + s;
        }
        else
        {
            tempPath = src + "/" + s;
        }
        if (s == "." || s == "..")
            continue;

        struct stat ip;
        if (stat((tempPath).c_str(), &ip) != 0)
        {
            cout << "File/Directory Not Found" << endl;
        }

        if (S_ISDIR(ip.st_mode))
        {
            copyDirectory(tempPath, dest + "/" + dirName);
        }
        else
        {
            copyFile(tempPath, dest + "/" + dirName);
        }
    }
}

//----------------------------------
//------------CREATE----------------
//-----------DIRECTORY--------------
//----------------------------------
void createDir(string dirToCreate, string path, mode_t perm)
{
    if (path == "/")
        path = path + dirToCreate;
    else
        path = path + "/" + dirToCreate;
    if (mkdir(path.c_str(), perm) == -1)
    {
        cout << endl
             << "Failed to Create Directory" << endl;
    }
    else
    {
        cout << endl
             << "Directory created successfully" << endl;
    }
}

//----------------------------------
//------------CREATE----------------
//-------------FILE-----------------
//----------------------------------
void createFile(string fileToCreate, string destDir)
{
    string path = parsePath(destDir);
    DIR *d = opendir(path.c_str());

    if (d == NULL)
    {
        cout << endl
             << "Failed to Create file" << endl;
        return;
    }

    if (path == "/")
        path = path + fileToCreate;
    else
        path = path + "/" + fileToCreate;

    if (creat(path.c_str(), 0777) == -1)
        cout << "Can't Create File";
}

//----------------------------------
//------------RENAME----------------
//-----------FUNCTION---------------
//----------------------------------
void rname(string oldFileName, string newFileName)
{
    oldFileName = parsePath(oldFileName);
    newFileName = parsePath(newFileName);
    int stat = rename(oldFileName.c_str(), newFileName.c_str());
    if (stat != 0)
        cout << endl
             << "Error renaming file" << endl;
    else
        cout << endl
             << "File renamed successfully" << endl;
}

//----------------------------------
//----------DELETE FILE-------------
//-----------FUNCTION---------------
//----------------------------------
void delete_file(string fname)
{
    fname = parsePath(fname);

    if (unlink(fname.c_str()) == -1)
    {
        cout << "Can't Delete File" << endl;
        return;
    }
    cout << endl
         << "File Deleted Successfully" << endl;
}

//----------------------------------
//----------DELETE SINGLE-----------
//-----------DIRECTORY--------------
//----------------------------------
void deleteSingleDirectory(string dirName)
{
    if (rmdir(dirName.c_str()) == -1)
    {
        cerr << "Error in deleting directory" << dirName << " " << strerror(errno) << endl;
    }
    else
    {
        cout << endl
             << "Directory Removed" << endl;
    }
}

//----------------------------------
//----------DELETE ENTIRE-----------
//-----------DIRECTORY--------------
//----------------------------------
void delete_entire_dir(string dirName)
{
    DIR *dir;
    struct dirent *x;
    string s, tempPath;
    bool isD;

    if ((dir = opendir(dirName.c_str())) != NULL)
    {
        while ((x = readdir(dir)) != NULL)
        {
            s = x->d_name;
            if (s == "." || s == "..")
                continue;

            if (dirName == "/")
            {
                tempPath = dirName + s;
            }
            else
            {
                tempPath = dirName + "/" + s;
            }
            isD = isDir(tempPath);

            if (isD)
            {
                delete_entire_dir(tempPath);
            }
            else
            {
                delete_file(tempPath);
            }
        }
        deleteSingleDirectory(dirName);
    }
}

//----------------------------------
//----------GET FILENAME------------
//-----------FROM PATH--------------
//----------------------------------
string getFileName(string path)
{
    int n = path.size();
    string fname = "";
    for (int i = n - 1; i >= 0; i--)
    {
        if (path[i] == '/')
        {
            fname = path.substr(i + 1);
            break;
        }
    }
    return fname;
}

//----------------------------------
//----------GET ABSOLUTE------------
//-------------PATH-----------------
//----------------------------------
string parsePath(string path)
{
    stack<string> stk;

    int n = path.size();
    string tempPath = "";
    tempPath += path[0];

    for (int i = 1; i < n; i++)
    {
        if (path[i] == '/')
        {
            tempPath = path[i - 1] == '/' ? tempPath : tempPath + path[i];
        }
        else
        {
            tempPath += path[i];
        }
    }
    path = tempPath;

    if (path[0] == '~')
    {
        const char *homePath;
        homePath = getenv("HOME");

        stringstream ss(homePath);
        string res;
        while (getline(ss, res, '/'))
        {
            if (res != "")
                stk.push(res);
        }
        path = path.substr(2);
    }

    else if (path[0] != '/')
    {
        stringstream ss(currPath);
        string res;

        while (getline(ss, res, '/'))
        {
            if (res != "")
                stk.push(res);
        }
    }

    stringstream ss(path);
    string res = "";
    while (getline(ss, res, '/'))
    {
        if (res != "" && res != ".")
        {
            if (res == "..")
            {
                if (!stk.empty())
                {
                    stk.pop();
                }
            }
            else
                stk.push(res);
        }
    }
    if (stk.size() == 0)
    {
        return "/";
    }
    res = "";
    while (!stk.empty())
    {
        res = "/" + stk.top() + res;
        stk.pop();
    }
    return res;
}

//----------------------------------
//----------CHECK IF PATH-----------
//-----------IS DIRECTORY-----------
//----------------------------------
bool isDir(string path)
{
    struct stat ip;
    stat(path.c_str(), &ip);
    return S_ISDIR(ip.st_mode) ? true : false;
}

mode_t getPerm(string src)
{
    struct stat inode;
    stat(src.c_str(), &inode);
    mode_t p = 0;

    p = p | ((inode.st_mode & S_IRUSR) ? 0400 : 0);
    p = p | ((inode.st_mode & S_IWUSR) ? 0200 : 0);
    p = p | ((inode.st_mode & S_IXUSR) ? 0100 : 0);
    p = p | ((inode.st_mode & S_IRGRP) ? 0040 : 0);
    p = p | ((inode.st_mode & S_IWGRP) ? 0020 : 0);
    p = p | ((inode.st_mode & S_IXGRP) ? 0010 : 0);
    p = p | ((inode.st_mode & S_IROTH) ? 0004 : 0);
    p = p | ((inode.st_mode & S_IWOTH) ? 0002 : 0);
    p = p | ((inode.st_mode & S_IXOTH) ? 0001 : 0);

    return p;
}