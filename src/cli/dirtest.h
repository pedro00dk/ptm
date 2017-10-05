#include "../lib/tinydir.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <set>
#include <map>
#include <vector>

using namespace std;

map<pair<int,int>, bool > findMemo;

bool findWildcard(string fileName, string fileRegex, int fileNamePos, int fileRegexPos){
    if (fileNamePos == fileName.length() && fileRegexPos == fileRegex.length()) return true;
    if (fileNamePos == fileName.length() || fileRegexPos == fileRegex.length()) return false;

    pair<int,int> posPair = pair<int,int>(fileNamePos, fileRegexPos);
    map<pair<int,int>, bool >::iterator findMemoIt = findMemo.find(posPair);
    if (findMemoIt != findMemo.end()) return findMemoIt -> second;

    bool found = false;
    if (fileName[fileNamePos] == fileRegex[fileRegexPos]) {
        found |= findWildcard(fileName, fileRegex, fileNamePos + 1, fileRegexPos + 1);
    }

    if (fileRegex[fileRegexPos] == '*') {
        found |= findWildcard(fileName, fileRegex, fileNamePos + 1, fileRegexPos);
        found |= findWildcard(fileName, fileRegex, fileNamePos + 1, fileRegexPos + 1);
        found |= findWildcard(fileName, fileRegex, fileNamePos, fileRegexPos + 1);
    }

    findMemo[posPair] = found;
    return found;
}

bool findWildcard(string fileName, string fileRegex){
  bool found = findWildcard(fileName, fileRegex, 0, 0);
  findMemo.clear();
  return found;
}

void findFiles(string fullPath, set<string> &fileSet){
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

        if (!file.is_dir && findWildcard(file.name, fileRegex)) {
            fileSet.insert(dirPath + file.name);
        }
        tinydir_next(&dir);
    }

    tinydir_close(&dir);
}

vector<string> findFiles(const vector<string> &filePaths) {
    set<string> fileSet;
    for (string filePath : filePaths) {
      findFiles(filePath, fileSet);
    }
    return vector<string>(fileSet.begin(), fileSet.end());
}
