#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
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

void myfind(char* dir, char* criteria, char* name)
{
	DIR * dp = opendir(dir);
	if (dp == NULL){
      		printf("Cannot open directory:%s\n", dir);
      		exit(0);
   	}
	struct dirent* entry;
	//errno = 0;
	chdir(entry->d_name);
	while((entry = readdir(dp)) != NULL) {
		if(entry->d_type == DT_DIR && entry->d_name[0] != '.' )
		{
			char path[1024];
			strcpy(path, dir);
			strcat(path,"/");
			strcat(path, entry->d_name);
			//printf("PATH: %s\n", path);
			myfind(path, criteria, name);
		}
		if(!strcmp(criteria, "-name") && !strcmp(entry->d_name, name))
			printf("%s/%s\n", dir, entry->d_name);
		else if (!strcmp(criteria, "-type"))
		{
			if(!strcmp(name, "f") && entry->d_type == DT_FIFO)
				printf("%s/%s\n", dir, entry->d_name);
			else if(!strcmp(name, "d") && entry->d_type == DT_DIR)
				printf("%s/%s\n", dir, entry->d_name);
			else if(!strcmp(name, "b") && entry->d_type == DT_BLK)
				printf("%s/%s\n", dir, entry->d_name);
			else if(!strcmp(name, "c") && entry->d_type == DT_CHR)
				printf("%s/%s\n", dir, entry->d_name);
			else if(!strcmp(name, "p") && entry->d_type == DT_FIFO)
				printf("%s/%s\n", dir, entry->d_name);
			else if(!strcmp(name, "l") && entry->d_type == DT_LNK)
				printf("%s/%s\n", dir, entry->d_name);
			else if(!strcmp(name, "s") && entry->d_type == DT_SOCK)
				printf("%s/%s\n", dir, entry->d_name);

		}	
	}
	closedir(dp);
	return;
}
