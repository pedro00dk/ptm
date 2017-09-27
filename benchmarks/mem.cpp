/*
The compared APIs are:
fstream (easy to use, bad performance)
fread (hard to use, good performance)
mmap (easier than fread and harder than fstream, best performance)
*/
/*
// streams (slow)
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
    ifstream inputFileStream = ifstream("../test/english.txt");
    if (!inputFileStream.is_open())
        printf("Error");
    for (string line; getline(inputFileStream, line);)
    {
        int lineLen = line.length();
        char c;
        for (int j = 0; j < lineLen; j += 5)
            c = line[j];
    }
    inputFileStream.close();
}
//*/

/*
// fread (mid)
#include <stdio.h>

// Tests of powers of 2 in range [1KB -> 128 MB], best -> 64KB
const int BUFFER_SIZE = 64 * 1024;
char BUFFER[BUFFER_SIZE];

int main(int argc, char **argv)
{
    FILE *filePointer = fopen("../test/english.txt", "r");
    if (filePointer == nullptr)
        printf("Error");
    int readable = 0;
    while (readable = fread(BUFFER, 1, BUFFER_SIZE, filePointer))
    {
        char c;
        for (int j = 0; j < readable; j += 5)
            c = BUFFER[j];
    }

    ;
    fclose(filePointer);
}
//*/

//*
// mmap (fast)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fileDescriptor = open("../test/english.txt", O_RDONLY);
    struct stat fileStat;
    fstat(fileDescriptor, &fileStat);
    char *memPointer = (char *)mmap(NULL, fileStat.st_size, PROT_READ, MAP_SHARED, fileDescriptor, 0);
    if (memPointer == MAP_FAILED)
        printf("Error");
    char c;
    for (long i = 0; i < fileStat.st_size; i += 5)
        c = memPointer[i];
}
//*/