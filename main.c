#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "zip.h"
#include <inttypes.h>
#include <string.h>
#include <dirent.h>

// Use this for debugging
//#define DEBUG

#define openZip(name) zipOpen64(name, 0)
#define closeZip(name) zipClose(name, NULL)
#define ull_int unsigned long long int

#define WRITEBUFFERSIZE (16384)


int buffer_size = WRITEBUFFERSIZE; // Buffer Size
void *buffer; //The buffer used to read files
struct stat s;


uint64_t GetFileSize(FILE* fp)
{
    uint64_t tmp;
    fseeko64(fp, 0, SEEK_END);
    tmp = ftello64(fp);
    rewind(fp);
    return tmp;
}

void ZipTheFile(zipFile* zf, char *filename)
{
	uint64_t size_read = 1; //  Ammount of bytes read
	uint64_t flsize = 0;
	int written = 0;
	FILE* fp;
	
	fp = fopen64(filename, "rb");
	
	if(fp == NULL)
	{
		puts("ERROR");
		exit(1);
	}
	#ifdef DEBUG
	
	#else
	printf("File %s opened successfuly.\n", filename);
	#endif
	flsize = GetFileSize(fp);
	
	
	zipOpenNewFileInZip(*zf, filename, NULL, NULL, 0, NULL,0, NULL, 0, Z_DEFAULT_COMPRESSION);
	
	
	while(1)
	{
	
		
		size_read = fread(buffer, 1, buffer_size, fp); // Reading the file, buf get the contents size_read the ammount of bytes read
		
		if(size_read)//If different than 0
		{
			written+=size_read;
			zipWriteInFileInZip(*zf, buffer, size_read);//Writting the file
		}
		else
		{
			
			break;
		}
	
	
	}
	if(written == flsize)
	{
		#ifdef DEBUG
	
	#else
		printf("File %s successfully writen to zip file.\n", filename);
		#endif
	}
	
	zipCloseFileInZip(*zf);
	
	fclose(fp);
	
	
	
	
	
	
}

void RealZipDir(zipFile *zp, DIR **dir, char* filen)
{	
	struct dirent *ent;
	DIR* tmp;
	while((ent = readdir(*dir)))
	{
		if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
		{	
		
		}
		else
		{
		
			char name[256];
			strcpy(name, filen);
			strcat(name, "/");
			strcat(name, ent->d_name);
			if( stat(name,&s) == 0 )
			{
    				if( s.st_mode & S_IFDIR )
    				{
    					tmp = opendir(name);
    					if(tmp != NULL)
    					{
    						RealZipDir(zp, &tmp, name);//Recursive if finds another dir 
    					}
    					
        				
    				}
    				else
    				{
    					
    					ZipTheFile(zp, name);//Zip that shit up
    				}
   			
			}	
			
		}
	
	}
	
	closedir(*dir);



}

int main (int argc, char *argv[])
{
	zipFile zf;
	DIR *dir;
	char path[256];
	
	
	if(argc < 3)
	{
		puts("HELL NO");
		exit(2);
	}
	
	zf = openZip(argv[1]);
	
	buffer = malloc(buffer_size);
	if(buffer == NULL)
	{
		exit(1);	
	}
	
	for(int i = 2; i<argc;i++)
	{
		if( stat(argv[i],&s) == 0 )
			{
    				if( s.st_mode & S_IFDIR )
    				{
    					dir = opendir(argv[i]);
    					if(dir != NULL)
    					{
    						strcpy(path, argv[i]);
						RealZipDir(&zf, &dir, path);
    						
    					}
    					else
    					{
						#ifdef DEBUG
						#else
						printf("Error opening dir %s\n",argv[2]);
						#endif
					}
    				}
    				else
    				{
    					
    					ZipTheFile(&zf, argv[i]);//Zip that shit up
    				}
			}	
	}
	

	
	
	closeZip(zf);
	free(buffer);

	return 0;
}
