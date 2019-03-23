//Search a string-Open a file in vim-Handle non canonical and non echo mode-Handle size of terminal-Percentage of file contents displayed-I/O redirection handing-reverse video feature using control sequence introducer \033[-read and print one page then pause for a few special commands ('q', ' ' , '\n')

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
void search(FILE*);
int get_input(FILE*);
void get_total_num_of_lines(FILE*);
void winch_handler(int);
void set_input_mode(struct termios attr);
char* filename[5];
struct termios attr, saved_attr;
int n = 0, total_noOfLines = 0;
int main(int argc , char *argv[])
{
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
void get_total_num_of_lines(FILE* fp)
{
	char buffer[LINELEN];
	while(fgets(buffer, LINELEN, fp))
		total_noOfLines++;
}
void do_more(FILE *fp)
{
	n = 0;	
	int num_of_lines = 0;
   	total_noOfLines = 0;
	int rv;
	char str[20];
	get_total_num_of_lines(fp);
	fseek(fp, 0, SEEK_SET);
   	char buffer[LINELEN];
   	FILE* fp_tty = fopen("/dev//tty", "r");
   	while (fgets(buffer, LINELEN, fp)){
      		fputs(buffer, stdout);
      		num_of_lines++;
      		n++;
	      	if (num_of_lines == PAGELEN){
		 	rv = get_input(fp_tty);
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
			else if(rv == 4)
		       	{ //search a given string
				tcsetattr(0, TCSANOW, &saved_attr);
				search(fp);
				tcgetattr(0, &saved_attr);
				attr = saved_attr;
				set_input_mode(attr);
				num_of_lines-=PAGELEN -1;

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
int get_input(FILE* cmdstream)
{
   	int c;
	int percentage = (float)n/(float)total_noOfLines*100;
	printf("\033[7m --more--(%d%) \033[m", percentage);	
	c = getc(cmdstream);
	if(c == 'q')
	 	return 0;
      	if ( c == ' ' )			
	 	return 1;
      	if ( c == '\n' )	
      	if ( c == '\n' )	
	 	return 2;
	if ( c == '/' ){
		printf("\n/");
		return 4;
	}
	if ( c == 'v' )
		return 5;
      	return 3;
}
void search(FILE* fp)
{
	char str[LINELEN];
	int chars_read = 0;
	int prev_offset = fseek(fp, 0, SEEK_CUR);
	char buffer[LINELEN];
	int temp = n;
	fgets(str, LINELEN, stdin);
	while(fgets(buffer, LINELEN, fp)) {
		chars_read = strlen(buffer);
		n++;
		//prev_offset += chars_read;
		if(strstr(buffer, str))
		{
			printf("\033[1;2H \033[2J");
			printf("skipping...\n");
			fseek(fp, chars_read*(-1), SEEK_CUR);
			return;
		}
	}
	n = temp;
	fseek(fp, prev_offset, SEEK_SET);
	printf("\033[2K \033[1G \033[7m Pattern not found \033[m");
	printf("\n");
	return;
}
