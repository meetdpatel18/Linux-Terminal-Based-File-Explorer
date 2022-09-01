string getFileSize(double size)
{
    string res;
    // File size in GB
    if ((size / (1073741824)) >= 1)
    {
        size = size / (1073741824);
        return setPrec(size, 2) + "G";
    }

    // File size in MB
    else if ((size / (1048576)) >= 1)
    {
        size = size / 1048576;
        return setPrec(size, 2) + "M";
    }

    // File size in KB
    else if ((size / (1024)) >= 1)
    {
        size = size / 1024;
        return setPrec(size, 2) + "K";
    }

    // File size in B
    else
        return setPrec(size, 2) + "B";
}

void dynamicWindowResize(int t){
    ioctl(STDIN_FILENO, TIOCGWINSZ, &w);
    rows=w.ws_row;
    cols=w.ws_col;
    startRow=0;
    currRow=0;
    CLEAR_SCREEN;
    display();
}

void findDirectoryContents(string path, string f_d_name)
{
    vector<string> curDir;
    struct stat inode;
    struct tm local_time;
    char mtime[50], temp[20];

    string f_perm, f_size, f_last_modified, perm, uid, gid;
    time_t s_time;

    stat((path).c_str(), &inode);

    // get file & directory permissions
    f_perm = "";
    f_perm += (S_ISDIR(inode.st_mode) ? "d" : "-");
    f_perm += ((inode.st_mode & S_IRUSR) ? "r" : "-");
    f_perm += ((inode.st_mode & S_IWUSR) ? "w" : "-");
    f_perm += ((inode.st_mode & S_IXUSR) ? "x" : "-");
    f_perm += ((inode.st_mode & S_IRGRP) ? "r" : "-");
    f_perm += ((inode.st_mode & S_IWGRP) ? "w" : "-");
    f_perm += ((inode.st_mode & S_IXGRP) ? "x" : "-");
    f_perm += ((inode.st_mode & S_IROTH) ? "r" : "-");
    f_perm += ((inode.st_mode & S_IWOTH) ? "w" : "-");
    f_perm += ((inode.st_mode & S_IXOTH) ? "x" : "-");

    // get user id
    uid = getpwuid(inode.st_uid)->pw_name;

    // get group id
    gid = getgrgid(inode.st_gid)->gr_name;

    // get file size
    f_size = getFileSize(inode.st_size);

    time_t t = inode.st_mtime;
    struct tm lt;
    localtime_r(&t, &lt);
    strftime(mtime, sizeof mtime, "%b %d %Y %H:%M", &lt);

    curDir.push_back(f_perm.c_str());
    curDir.push_back(uid);
    curDir.push_back(gid);
    curDir.push_back(f_size);
    curDir.push_back(mtime);
    curDir.push_back(f_d_name);
    curDir.push_back(path);

    dirVect.push_back(curDir);
}

string tolower(string s)
{
    int n = s.size();
    for (int i = 0; i < n; i++)
    {
        if (s[i] >= 65 && s[i] <= 90)
            s[i] = tolower(s[i]);
    }
    return s;
}

bool myComp(vector<string> v1, vector<string> v2)
{
    string s1 = tolower(v1[5]);
    string s2 = tolower(v2[5]);

    if (s1 < s2)
        return true;
    return false;
}

void display()
{
    ioctl(STDIN_FILENO, TIOCGWINSZ, &w);

    int totalRows=w.ws_row;
    rows = totalRows * 0.7;

    int n = dirVect.size();
    int upperLimit = min(n, startRow + rows);
    int rowsPrinted=0;
    for (int i = startRow; i < upperLimit; i++)
    {
        if (i == currRow)
            cout << left << setw(4) << "->";
        else
            cout << left << setw(4) << setfill(' ') << "";

        cout << left << setw(5) << setfill(' ') << to_string(i + 1) + ")";
        if(cols>70)
            cout << right << setw(13) << setfill(' ') << dirVect[i][0];
        if(cols>115)
            cout << right << setw(10) << setfill(' ') << dirVect[i][1];
        if(cols>115)
            cout << right << setw(10) << setfill(' ') << dirVect[i][2];
        if(cols>115)
            cout << right << setw(10) << dirVect[i][3];
        if(cols>80)
            cout << right << setw(25) << dirVect[i][4];
        cout << "\t"<<dirVect[i][5] << endl;
        rowsPrinted++;
    }

    for(int i=1;i<=totalRows-rowsPrinted-5;i++){
        cout<<endl;
    }
    cout<< "Current Path: " << currPath<<endl;
    if (mode == NORMAL_MODE)
        cout << endl
             << "-------------NORMAL MODE-------------" << endl;
    else
        cout << endl
             << "-------------COMMAND MODE-------------" << endl;
}


void listDirectory(string path)
{
    startRow = 0;
    int n = path.size();
    string newP;
    if (n >= 2 && path[n - 1] == '.' && path[n - 2] == '.')
    {
        path = findParent(path, n);
    }

    DIR *d;
    struct dirent *dir;
    d = opendir(path.c_str());

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            string newP;
            int n = path.size();
            if (path[n - 1] == '/')
            {
                newP = path + dir->d_name;
            }
            else if (path[n - 1] == '.')
            {
                newP = path.substr(0, n - 1) + dir->d_name;
            }
            else
            {
                newP = path + "/" + dir->d_name;
            }
            findDirectoryContents(newP, dir->d_name);
        }
        closedir(d);
        sort(dirVect.begin(), dirVect.end(), myComp);
    }

    else
    {
        cout << "Error in opening directory" << endl;
        exit(1);
    }

    display();
}