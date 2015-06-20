#include "incs.h"

extern FILE* logs;
extern int compress_level;

void SetCompressLevel(char* level)//Function used to set the compression level
{
		int tmp;
		tmp = atoi(level);//Turns string into int
		if(tmp>=0 && tmp<= 9)//Check if the tmp is valid for compression_level
		{
		compress_level = tmp;//IT IS!!
		if(logs != stdout){
		fprintf(logs,"Compression level set to: %d\n", compress_level);
		}
		fprintf(stdout,"Compression level set to: %d\n", compress_level);
		}
		
}

void AllowLog(char* name)
{
	char coise[strlen(name)+4];//Allocate enough space ofr the file name
	strcpy(coise, name);//Get the name
	strcat(coise,".txt");//Add the extension
	logs = fopen(coise, "a");//VOILA
	

}