#include "incs.h"

// Use this for debugging
//#define DEBUG

#define openZip(name) zipOpen64(name, 0)
#define closeZip(name) zipClose(name, NULL)

#define WRITEBUFFERSIZE (16384)

int buffer_size = WRITEBUFFERSIZE; // Buffer Size
void *buffer; //The buffer used to read files
struct stat s;
int compress_level = Z_DEFAULT_COMPRESSION;// If user doesn't choose any compression level the default will be used
FILE *logs;
int last_pos = 0;
char path[256];
zipFile zf;
bool should_continue = true;

uint64_t GetFileSize(FILE* fp)
{
    uint64_t tmp;
    fseeko64(fp, 0, SEEK_END);
    tmp = ftello64(fp);
    rewind(fp);
    return tmp;
}

void InitZip(char *argv[],int argc, int pos,uint8_t zipdir)
{
	DIR* dir,*tmp;
	struct dirent* ent;
	
	if(zipdir)
	{
		dir = opendir(".");
		if(dir == NULL)
		{
			if(logs != stdout){
							fprintf(logs,"Error opening dir %s\n",ent->d_name);}
							fprintf(stdout,"Error opening dir %s\n",ent->d_name);
							exit(3);
		}
		while((ent=readdir(dir)) != NULL)
		{
			
			if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 && strcmp(ent->d_name, argv[pos-1]) != 0)
			{
				
				if( stat(ent->d_name,&s) == 0 )
				{
    				if( s.st_mode & S_IFDIR )
    				{
    					tmp = opendir(ent->d_name);
    					if(dir != NULL)
    					{
    						strcpy(path, ent->d_name);
							RealZipDir(&zf, &tmp, path);
    						closedir(tmp);
    					}
    					else
    					{
						#ifdef DEBUG
						#else
							if(logs != stdout){
							fprintf(logs,"Error opening dir %s\n",ent->d_name);}
							fprintf(stdout,"Error opening dir %s\n",ent->d_name);
						#endif
						}
    				}
    				else
    				{
    					
    					ZipTheFile(&zf, ent->d_name);//Zip that shit up
    				}
				}
			}
		}
			
			
	}
		
	else{
	for(int i = last_pos; i<argc;i++)
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
							fprintf(logs,"Error opening dir %s\n",argv[i]);}
							fprintf(stdout,"Error opening dir %s\n",argv[i]);
						#endif
					}
    				}
    				else
    				{
    					
    					ZipTheFile(&zf, argv[i]);//Zip that shit up
    				}
			}
			else
			{
				if(logs != stdout)
				{
		
					fprintf(logs,"Couldn't open file %s. %s", argv[i], (should_continue ? "Continuing..\n" : "Stopping.\n" ));
				}
				fprintf(stdout,"Couldn't open file %s. %s", argv[i], (should_continue ? "Continuing..\n" : "Stopping.\n" ));
				if(should_continue == 0)
				{
					exit(1);
				}
			}	
	}
}
	
	
	}

int main (int argc, char *argv[])
{
	logs = stdout;
	int want_log = 0;
	uint8_t zip_all_dir = 0;
	
	
	if(argc < 3)
	{
		puts("Usage: ./realzip [flags] file.zip [files to zip]");
		puts("Flags:\n \t -k Stop when a file can't be openned \n \t -c [0-9] Compress level\n\t -l To create a log file. \nThe log file name will be [zipfilename].txt. It will append if it already exists");
		exit(2);
	}
	
	
	
	buffer = malloc(buffer_size);
	if(buffer == NULL)
	{
		exit(1);	
	}
	for(int i = 1; i<=5; i++)
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
				case 'k':
					should_continue = false;
					break;
				case 'a':
					zip_all_dir = 1;
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
	
	
	InitZip(argv,argc, last_pos,zip_all_dir);
	
	
	

	
	
	closeZip(zf);
	free(buffer);

	return 0;
}


