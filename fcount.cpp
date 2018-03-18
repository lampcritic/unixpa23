#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
//comment for week 09 homework
int dirs;
int files;
int bytes;

char *getType(unsigned char type)
{
	switch (type)
	{
		case DT_DIR:
			return "Directory";
		case DT_REG:
			return "File";
		default:
			return "Unknown";
	}
}

void fcount(char *inputPath)
{
	DIR *dir;
	FILE *file;
	struct dirent *de;
	char fullPath [1024];

	dir = opendir (inputPath);
	if(!dir)
	{
		printf("Error opening directory: %s\n", inputPath);
		return;
	}

	while(de = readdir(dir))
	{
		if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
		{
			continue;
		}	
		if(!strcmp(getType(de->d_type), "Directory"))
		{
			dirs++;
			strcat(fullPath, inputPath);
			strcat(fullPath, "/");
			strcat(fullPath, de->d_name);
			fcount(fullPath);
			*fullPath = '\0';
		}
		else if(!strcmp(getType(de->d_type), "File"))
		{
			strcat(fullPath, inputPath);
			strcat(fullPath, "/");
			strcat(fullPath, de->d_name);
			file = fopen(fullPath, "r");
			if(file)
			{
				files++;
				fseek(file, 0L, SEEK_END);
				bytes += ftell(file);
				fclose(file);
			}
			*fullPath = '\0';
		}
	}
	closedir(dir);
}

int main(int argc, char **argv)
{
	dirs = files = 0;
	bytes = 0L;
	fcount(*(argv + 1));
	printf("The total number of directories in directory %s is %d\n", *(argv+1)		, dirs);
	printf("The total number of files in the directory %s is %d\n", *(argv+1),
		files);
	printf("The total number of bytes occupied by all files in the directory %s is %ld\n", *(argv+1), bytes);
}
