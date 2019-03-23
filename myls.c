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
char* typesAndPermissions(int);
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
char* typesAndPermissions(int mode)
{
	char *str=malloc(sizeof(char)*10);
	strcpy(str,"----------");
	if((mode & 0170000) == 0010000)
		str[0]='p';
	else if((mode & 0170000) == 0020000)
		str[0]='c';
	else if((mode & 0170000) == 0040000)
		str[0]='d';
	else if((mode & 0170000) == 0060000)
		str[0]='b';
	else if((mode & 0170000) == 0100000)
		str[0]='-';
	else if((mode & 0170000) == 0120000)
		str[0]='l';
	else if((mode & 0170000) == 0140000)
		str[0]='s';
	else 
		str[0]='~';		//unknown
	if((mode & 0000400) == 0000400)
		str[1]='r';
	if((mode & 0000200) == 0000200)
		str[2]='w';
	if((mode & 0000100) == 0000100)
		str[3]='x';
	if((mode & 0000040) == 0000040)
		str[4]='r';
	if((mode & 0000020) == 0000020)
		str[5]='w';
	if((mode & 0000010) == 0000010)
		str[6]='x';
	if((mode & 0000004) == 0000004)
		str[7]='r';
	if((mode & 0000002) == 0000002)
		str[8]='w';
	if((mode & 0000001) == 0000001)
		str[9]='x';
	if((mode & 0004000) == 0004000)
		str[3]='s';
	if((mode & 0002000) == 0002000)
		str[6]='s';
	if((mode & 0001000) == 0001000)
		str[9]='t';	
	return str;
}
