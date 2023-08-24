#include <bits/stdc++.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <termios.h>
#include <iostream>
#include <pwd.h>
#include <grp.h>
#include <sys/ioctl.h>
#include <signal.h>


#define esc 27
#define set_pos(x, y) printf("%c[%d;%dH", esc, x, y)

#define KEY_ESCAPE 27
#define KEY_ENTER 10
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68
#define KEY_BACKSPACE 127
#define KEY_HOME 104
#define KEY_EXIT 113
#define KEY_COLON 58
#define CLEAR_SCREEN cout << "\33c"
#define NORMAL_MODE 0
#define COMMAND_MODE 1

using namespace std;

struct winsize w;
static struct termios orig_termios;
int startRow=0, currRow=0, rows, cols;
string currPath;
vector<vector<string>> dirVect;
stack<string> backwardSTK, forwardSTK, searchSTK;
bool pathSet=false;
bool mode;
mode_t globalPerm=0755;


string findParent(string path, int n);
void initializeNewDirectoryPointers();
void clearStack(stack<string> &s);
bool moveDown();
bool moveUp();
bool goInDirectory();
bool goBackward();
bool goForward();
bool goParent();
bool goHome();
string getFileSize(double size);
void findDirectoryContents(string path, string f_d_name);
string tolower(string s);
bool myComp(vector<string> v1, vector<string> v2);
void display();
void listDirectory(string path);
bool searchFileDir(string path, string fileToSearch);
bool isDirPresent(string path, string fileToSearch);
string parsePath(string path);
void normalMode();
void disableRawMode();
void enableRawMode();
void createDir(string fileDirToCreate, string destDir,  mode_t perm);
void createFile(string fileToCreate, string destDir);
void rname(string oldFileName, string newFileName);
void copy(vector<string> path);
bool isDir(string path);
void copyFile(string sourceFile, string destFile);
string getFileName(string path);
void copyDirectory(string src, string dest);
void delete_file(string fname);
void delete_entire_dir(string dirName);
void deleteSingleDirectory(string dirName);
void goToCommandMode();
mode_t getPerm(string src);
void move(vector<string> path);
void moveFile(string src, string dest);
void moveDirectory(string src, string dest);
bool canDelete(string src);
void dynamicWindowResize(int t);