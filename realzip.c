#include "incs.h"


extern struct stat s;
extern FILE* logs;
extern int compress_level;
extern int buffer_size;
extern void* buffer;
extern bool should_continue;

void ZipTheFile(zipFile* zf, char *filename)
{
	uint64_t size_read = 1; //  Ammount of bytes read
	uint64_t flsize = 0;
	uint64_t written = 0;
	FILE* fp;
	
	fp = fopen64(filename, "rb");
	
	if(fp == NULL)
	{
		
	if(logs != stdout){
		
		fprintf(logs,"Couldn't open file %s. %s", filename, (should_continue ? "Continuing..\n" : "Stopping.\n" ));
		}
	fprintf(stdout,"Couldn't open file %s. %s", filename, (should_continue ? "Continuing..\n" : "Stopping.\n" ));
		if(should_continue == 0)
		{
			return;
		}
		
	}
		#ifdef DEBUG
		#else
		if(logs != stdout){
		fprintf(logs,"File %s opened successfuly.\n", filename);
		}
		fprintf(stdout,"File %s opened successfuly.\n", filename);
		#endif
	flsize = GetFileSize(fp);
	
	
	zipOpenNewFileInZip64(*zf, filename, NULL, NULL, 0, NULL,0, NULL, (compress_level ? Z_DEFLATED : Z_NO_COMPRESSION), compress_level, (flsize >= 0xffffffff) ? 1 : 0);
	//Added 64 bit support
	
	while(1)
	{
	
		
		size_read = fread(buffer, 1, buffer_size, fp); // Reading the file, buf get the contents size_read the ammount of bytes read
		
		if(size_read)//If different than 0
		{
			written+=size_read;//Adding to ammount read
			zipWriteInFileInZip(*zf, buffer, size_read);//Writting the file
		}
		else
		{
			break;
		}
		fprintf(stdout,"Writting %s to zip file...%" PRIu64 " %% \r", filename, ((written*100)/flsize));
	
	
	}
		fprintf(stdout,"\n");
	
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