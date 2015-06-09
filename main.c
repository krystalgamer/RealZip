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
int compress_level = Z_DEFAULT_COMPRESSION;// If user doesn't choose any compression level the default will be used
FILE *logs;

uint64_t GetFileSize(FILE* fp)
{
    uint64_t tmp;
    fseeko64(fp, 0, SEEK_END);
    tmp = ftello64(fp);
    rewind(fp);
    return tmp;
}

void SetCompressLevel(char* level)
{
		int tmp;
		tmp = atoi(level);
		if(tmp>=0 && tmp<= 9)
		{
		compress_level = tmp;
		if(logs != stdout){
		fprintf(logs,"Compression level set to: %d\n", compress_level);
		}
		fprintf(stdout,"Compression level set to: %d\n", compress_level);
		}
		
}

void AllowLog(char* name)
{
	char coise[strlen(name)+4];
	strcpy(coise, name);
	strcat(coise,".txt");
	logs = fopen(coise, "a");
	

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
	if(logs != stdout){
		
		fprintf(logs,"File %s opened successfuly.\n", filename);
		}
	fprintf(stdout,"File %s opened successfuly.\n", filename);
	#endif
	flsize = GetFileSize(fp);
	
	
	zipOpenNewFileInZip(*zf, filename, NULL, NULL, 0, NULL,0, NULL, (compress_level ? Z_DEFLATED : Z_NO_COMPRESSION), compress_level);
	
	
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
		if(logs != stdout){
		fprintf(logs,"File %s successfully writen to zip file.\n", filename);}
		fprintf(stdout,"File %s successfully writen to zip file.\n", filename);
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
	logs = stdout;
	zipFile zf;
	DIR *dir;
	char path[256];
	int last_pos = 0;
	
	
	if(argc < 3)
	{
		puts("HELL NO");
		exit(2);
	}
	
	
	
	buffer = malloc(buffer_size);
	if(buffer == NULL)
	{
		exit(1);	
	}
	int want_log = 0;
	for(int i = 1; i<=3; i++)
	{
		
		if(argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
				case 'c':
					SetCompressLevel(argv[i+1]);
					i++;
					break;
				case 'l':
					want_log = 1;
					
					break;
						
			}
		
		}
		else
		{
			last_pos = i;
			if(want_log)
				{AllowLog(argv[i]);}
			break;
		}
		
	}
	
		zf = openZip(argv[last_pos]);
		last_pos++;
	
	
	
	
	for(int i = last_pos; i<argc;i++)//Need to change
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
							if(logs != stdout){
						fprintf(logs,"Error opening dir %s\n",argv[2]);}
						fprintf(stdout,"Error opening dir %s\n",argv[2]);
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
