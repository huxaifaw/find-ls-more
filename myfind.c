#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

//extern int errno = 0;
void myfind(char*, char*, char*);
int main(int argc, char* argv[]) {
	if(argc <= 3 || argc > 4) {
		printf("Incomplete number of arguments\n");
		exit(1);	
	}	
	myfind(argv[1], argv[2], argv[3]);
	return 0;	
}

void myfind(char* dir, char* type, char* name)
{
	//printf("%s\n", dir);
    	DIR * dp = opendir(dir);
   	if (dp == NULL){
      		printf("Cannot open directory:%s\n", dir);
      		return;
   	}
	struct dirent* entry;
	//errno = 0;
	while((entry = readdir(dp)) != NULL) {
		const char* dirName = dir;
		chdir(dirName);
		if(entry->d_type == DT_DIR && entry->d_name[0] != '.' )
		{
			char path[1024];
			strcpy(path, dir);
			strcat(path,"/");
			strcat(path, entry->d_name);
			myfind(path, type, name);
		}
		if(!strcmp(entry->d_name, name))
			printf("%s/%s\n", dir, entry->d_name);	
	}
//	closedir(dp);
}
