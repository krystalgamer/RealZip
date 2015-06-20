#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "zip.h"
#include <inttypes.h>
#include <string.h>
#include <dirent.h>

#define bool int
#define true 1
#define false 0

void InitZip(char *argv[],int argc, int pos,uint8_t zipdir);
void SetCompressLevel(char* level);
void AllowLog(char* name);
void ZipTheFile(zipFile* zf, char *filename);
void RealZipDir(zipFile *zp, DIR **dir, char* filen);
uint64_t GetFileSize(FILE* fp);