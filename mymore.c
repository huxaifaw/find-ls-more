//Open a file in vim-Handle non canonical and non echo mode-Handle size of terminal-Percentage of file contents displayed-I/O redirection handing-reverse video feature using control sequence introducer \033[-read and print one page then pause for a few special commands ('q', ' ' , '\n')

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
//#define PAGELEN 20
#define	LINELEN	512
int PAGELEN = 0;
void do_more(FILE*);
void mygrep(FILE*, char*);
int get_input(FILE*, int, int);
int get_total_num_of_lines(FILE*);
void winch_handler(int);
void set_input_mode(struct termios attr);
char* filename[5];
int main(int argc , char *argv[])
{
	struct termios attr, saved_attr;
	struct winsize ts;
	ioctl(0, TIOCGWINSZ, &ts);
	signal(SIGWINCH, winch_handler);
	PAGELEN = ts.ws_row - 1;
	tcgetattr(0, &saved_attr);
	attr = saved_attr;
	set_input_mode(attr);
	int i=0;
	if (argc == 1){
		do_more(stdin);
   	}
   	FILE * fp;
   	while(++i < argc){
      		fp = fopen(argv[i] , "r");
      		if (fp == NULL){
        		perror("Can't open file");
         		exit (1);
      		}
	filename[0] = "vim";
	filename[1] = argv[i];
	filename[2] = NULL;
      	do_more(fp);
      	fclose(fp);
   	}
      	tcsetattr(0, TCSANOW, &saved_attr);	
	return EXIT_SUCCESS;
}
void set_input_mode (struct termios attr)
{
	attr.c_lflag = attr.c_lflag & ~(ICANON|ECHO);
  	attr.c_cc[VMIN] = 1;
  	attr.c_cc[VTIME] = 0;
  	tcsetattr(0, TCSANOW, &attr);
}
void winch_handler(int signo)
{
	struct winsize ts;
	ioctl(0, TIOCGWINSZ, &ts);
	PAGELEN = ts.ws_row - 1;
}
int get_total_num_of_lines(FILE* fp)
{
	int n = 0;
	char buffer[LINELEN];
	while(fgets(buffer, LINELEN, fp))
		n++;
	return n;
}
void do_more(FILE *fp)
{
	int num_of_lines = 0;
   	int rv, n = 0;
	char str[20];
	int total_noOfLines = get_total_num_of_lines(fp);
	fseek(fp, 0, SEEK_SET);
   	char buffer[LINELEN];
   	FILE* fp_tty = fopen("/dev//tty", "r");
   	while (fgets(buffer, LINELEN, fp)){
      		fputs(buffer, stdout);
      		num_of_lines++;
      		n++;
	      	if (num_of_lines == PAGELEN){
		 	rv = get_input(fp_tty, n, total_noOfLines);
			if (rv == 0) {//user pressed q
		    		printf("\033[2K \033[1G");
		    		break;
		 	}
		 	else if (rv == 1) {//user pressed space bar
		    		printf("\033[2K \033[1G");
		    		num_of_lines -= PAGELEN;
		 	}
		 	else if (rv == 2) {//user pressed return/enter
		    		printf("\033[2K \033[1G");
			 	num_of_lines -= 1; //show one more line
		 	}
		 	else if (rv == 3) {//invalid character
		    		printf("\033[2K \033[1G");
		    		break; 
		 	}
			else if(rv == 4) { //search a given string
				printf("\033[1A \033[2K \033[1G");
			}
			else if(rv == 5) { //open file in vim editor
				pid_t pid = fork();
				if(!pid) {
					execv("/bin/vim", filename);
				}
				else {
					waitpid(pid, 0, 0);
				}
				printf("\033[1A \033[2K \033[1G");
				printf("\033[1K \033[2G");
				num_of_lines--;
			}
	      	}
	}
}
int get_input(FILE* cmdstream, int num_of_lines, int total_noOfLines)
{
   	int c;
	int percentage = (float)num_of_lines/(float)total_noOfLines*100;
	printf("\033[7m --more--(%d%) \033[m", percentage);	
	c = getc(cmdstream);
	if(c == 'q')
	 	return 0;
      	if ( c == ' ' )			
	 	return 1;
      	if ( c == '\n' )	
	 	return 2;
	if ( c == '/' )
		return 4;
	if ( c == 'v' )
		return 5;	
      	return 3;
}
void mygrep(FILE* fp, char* str)
{
	char c1[100], c2[100];
	while(fgets(c1, 100, fp))
	{
		char* temp;
		int i = 0;
		strncpy(c2, c1, 100);
		temp = strtok(c1, " ");
		while(temp)
		{
			if(!strcmp(temp, str))
			{
				fputs(c2, stdout);
				printf("/n");
				break;
			}
			temp = strtok(NULL, " ");

		}
	}
}
