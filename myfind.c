#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

extern int errno = 0;
void myfind(char*, char*, char*);
int main(int argc, char* argv[]) {
	if(argc <= 3 || argc > 4) {
		printf("Incomplete number of arguments\n");
		return 1;	
	}	
	else {
		myfind(argv[1], argv[2], argv[3]);	
	}
	return 0;	
}

void myfind(char* dir, char* type, char* name)
{
    	struct dirent * entry;
	DIR * dp = opendir(dir);
   	if (dp == NULL){
      		fprintf(stderr, "Cannot open directory:%s\n", dir);
      		return;
   	}
	errno = 0;
	while((entry = readdir(dp)) != NULL) {
		if(entry->d_type == DT_DIR)
		{
			char path[1024];
			
		}	
	}
	closedir(dir);
}
