#include "lib/tinydir.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <set>

using namespace std;

bool findWildcard(string fileName, string fileRegex, int fileNamePos, int fileRegexPos){
    if (fileNamePos == fileName.length() && fileRegexPos == fileRegex.length()) return true;
    if (fileNamePos == fileName.length() || fileRegexPos == fileRegex.length()) return false;

    bool found = false;
    if (fileName[fileNamePos] == fileRegex[fileRegexPos]) {
        found |= findWildcard(fileName, fileRegex, fileNamePos + 1, fileRegexPos + 1);
    }

    if (fileRegex[fileRegexPos] == '*') {
        found |= findWildcard(fileName, fileRegex, fileNamePos + 1, fileRegexPos);
        found |= findWildcard(fileName, fileRegex, fileNamePos + 1, fileRegexPos + 1);
        found |= findWildcard(fileName, fileRegex, fileNamePos, fileRegexPos + 1);
    }

    return found;
}

void searchFiles(string fullPath, set<string> &fileSet){
    tinydir_dir dir;
    string dirPath, fileRegex;

    if (fullPath.find_last_of("/") != string::npos) {
        dirPath = fullPath.substr(0, fullPath.find_last_of("/") + 1);
        fileRegex = fullPath.substr(fullPath.find_last_of("/") + 1);
    } else {
        dirPath = "./";
        fileRegex = fullPath;
    }

    tinydir_open(&dir, dirPath.c_str());
    while (dir.has_next) {
        tinydir_file file;
        tinydir_readfile(&dir, &file);

        if (!file.is_dir && findWildcard(file.name, fileRegex, 0, 0)) {
            printf("%s\n", file.name);
            fileSet.insert(dirPath + file.name);
        }
        tinydir_next(&dir);
    }

    tinydir_close(&dir);
}

int main() {
    string fullPath;
    set<string> fileSet;

    while (cin >> fullPath && fullPath[0] != '|') {
        searchFiles(fullPath, fileSet);
    }

    set<string>::iterator it;
    for (it = fileSet.begin(); it != fileSet.end(); it++) {
         cout << *it << endl;
    }
}