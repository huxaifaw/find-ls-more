#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

extern int errno;
void do_ls(char*);
char* uidToUname(int);
char* gidToGname(int);
int cstring_cmp(const void*, const void*);
int main(int argc, char* argv[])
{
	if(argc==1)
		do_ls(".");
	else {
		int i = 0;
		while(++i < argc)
		{
			printf("Directory listing of &s:\n", argv[i]);
			do_ls(argv[i]);
		}
	}
	return 0;
}
void do_ls(char* dir)
{
	struct dirent * entry;
   	DIR * dp = opendir(dir);
   	if (dp == NULL){
      	fprintf(stderr, "Cannot open directory:%s\n",dir);
      	return;
   	}
   	errno = 0;
	int numOfNames = 0;
   	while((entry = readdir(dp)) != NULL) {
        	if(entry == NULL && errno != 0) {
  			perror("readdir failed");
			exit(errno);
  		}
		else {                
			if(entry->d_name[0] != '.')
        			numOfNames++;
        	}     	
  	}
	char** names;
	names = (char**)malloc(sizeof(char**)*numOfNames);
	rewinddir(dp);
	int i = 0;
	while((entry = readdir(dp)) != NULL) {
		if(entry == NULL && errno != 0) {
			perror("readdir failed");
			exit(errno);
		}
		else {
			if(entry->d_name[0] != '.')
			{
				names[i] = (char*)malloc(sizeof(char) * (strlen(entry->d_name) + 1));
				strcpy(names[i], entry->d_name);
				i++;
			}
		}
	}
	qsort(names, numOfNames, sizeof(char*), cstring_cmp);
	for(int j = 0; j<numOfNames; j++)
		printf("%s\n", names[j]);
   	closedir(dp);
}
char* uidToUname(int uid)
{
	errno = 0;
	struct passwd * pwd = getpwuid(uid);
	if(pwd == NULL)
	{
		if(errno == 0)
			return "Unknown User\n";
		else
			printf("getpwuid failed\n");
	}
	return pwd->pw_name;
}
char* gidToGname(int gid)
{
	errno = 0;
	struct group * grp = getgrgid(gid);
	if(grp == NULL)
	{
		if(errno == 0)
			return "Unknown Group\n";
		else
			printf("getgrgid failed\n");
	}
	return grp->gr_name;
}
int cstring_cmp(const void* a, const void* b)
{
	const char **str1 = (const char **)a;
	const char **str2 = (const char **)b;
	return strcmp(*str1, *str2);
}
